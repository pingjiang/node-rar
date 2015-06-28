#include <node.h>
#include <v8.h>
#include "rar.hpp"
#include <iostream>
#include <nan.h>

#ifdef _DEBUG
#define _D(msg) do {\
  std::cout << __FILE__ << ":" << __LINE__ << ">> " << msg << std::endl;\
} while(0)

#else

#define _D(msg)

#endif // _DEBUG

static void reset_RARHeaderDataEx(struct RARHeaderDataEx* s) {
  memset(s, 0, sizeof(struct RARHeaderDataEx));
}
static void reset_RAROpenArchiveDataEx(struct RAROpenArchiveDataEx* s) {
  memset(s, 0, sizeof(struct RAROpenArchiveDataEx));
}

using namespace v8;

/// mode: 0 list, 1 extract, 2 list inc split
/// op: 0 skip, 1 test, 2 extract
int _processArchive(int mode, int op, char* filepath, char* toDir, char* password, Local<Function> cb) {
  struct RAROpenArchiveDataEx archiveData;
  reset_RAROpenArchiveDataEx(&archiveData);
  archiveData.ArcName = filepath;
  archiveData.OpenMode = mode;

  HANDLE handler = RAROpenArchiveEx(&archiveData);
  if (archiveData.OpenResult != ERAR_SUCCESS) {
    _D("open archive error: " << archiveData.OpenResult);
    return archiveData.OpenResult;
  }

  if (password != NULL) {
    _D("password: " << password);
    RARSetPassword(handler, password);
  }

  int result = 0;
  while (result == 0) {
      struct RARHeaderDataEx entry;
      reset_RARHeaderDataEx(&entry);
      result = RARReadHeaderEx(handler, &entry);
      if (result == 0) {
        result = RARProcessFile(handler, op, toDir, NULL);
      }
      if (result != 0)
          break;
      Local<Object> entryObj = NanNew<Object>();
      entryObj->Set(NanNew<String>("FileName"), NanNew<String>(entry.FileName));
      _D("FileName: " << entry.FileName);
      if (!cb.IsEmpty()) {
        const unsigned argc = 1;
        Local<Value> argv[argc] = { entryObj };
        NanMakeCallback(NanGetCurrentContext()->Global(), cb, argc, argv);
      } else {
        _D("cb is empty");
      }
  }
  if (result == ERAR_END_ARCHIVE) {
      result = 0;
  }
  _D("list archive result: " << result);
  return result;
}

NAN_METHOD(DUMMY) {
  NanScope();
  NanReturnUndefined();
}

NAN_METHOD(processArchive) {
  NanScope();

  if (args.Length() < 1) {
    NanThrowError("Wrong arguments");
    NanReturnUndefined();
  }

  int openMode = 0;
  bool isTest = false;
  Local<Object> options = args[0]->IsString() ? NanNew<Object>() : args[0]->ToObject();
  if (args[0]->IsString()) {
    options->Set(NanNew<String>("filepath"), args[0]->ToString());
  }
  Local<Value> openModeValue = options->Get(NanNew<String>("openMode"));
  if (openModeValue->IsNumber()) {
    openMode = openModeValue->NumberValue();
  }
  Local<Value> filepathValue = options->Get(NanNew<String>("filepath"));
  if (!filepathValue->IsString()) {
    NanThrowError("Wrong arguments `filepath`");
    NanReturnUndefined();
  }
  String::Utf8Value value(filepathValue);
  const char* filepathStr = (const char*)*value;
  char archiveFilePath[2048];
  strncpy(archiveFilePath, filepathStr, 2048);

  Local<Value> passwordValue = options->Get(NanNew<String>("password"));
  char passwordBuf[128];
  if (passwordValue->IsString()) {
    String::Utf8Value value1(passwordValue);
    const char* passwordStr = (const char*)*value1;
    strncpy(passwordBuf, passwordStr, 128);
  }
  Local<Function> cb = (args.Length() > 1 && args[1]->IsFunction()) ? args[1].As<Function>() : NanNew<FunctionTemplate>(DUMMY)->GetFunction();

  Local<Value> isTestValue = options->Get(NanNew<String>("test"));
  if (!isTestValue->IsUndefined()) {
    isTest = true;
  }

  char toDirBuf[1024] = { 0 };
  Local<Value> toDirValue = options->Get(NanNew<String>("toDir"));
  if (openMode == 1) {
    if (toDirValue->IsString()) {
      String::Utf8Value value2(toDirValue);
      const char* toDirStr = (const char*)*value2;
      strncpy(toDirBuf, toDirStr, 1024);
    } else {
      if (!isTest) {
        NanThrowError("Wrong arguments `toDir` for extract mode");
        NanReturnUndefined();
      }
    }
  }

  int ret = _processArchive(openMode, isTest ? 1 : (openMode == 0 ? 0 : 2), archiveFilePath,
    toDirValue->IsString() ? toDirBuf : NULL,
    passwordValue->IsString() ? passwordBuf : NULL, cb);

  if (ret != 0) {
    NanThrowError("Process archive error");
    _D("error code is " << ret);
  }

  NanReturnUndefined();
}

void init(Handle<Object> exports) {
  setlocale(LC_ALL,"");
  exports->Set(NanNew<String>("processArchive"), NanNew<FunctionTemplate>(processArchive)->GetFunction());
}

NODE_MODULE(unrar, init);

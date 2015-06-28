#include <node.h>
#include <v8.h>
#include "rar.hpp"
#include <iostream>

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
int _processArchive(Isolate* isolate, int mode, int op, char* filepath, char* toDir, char* password, Local<Function> cb) {
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
      Local<Object> entryObj = Object::New(isolate);
      entryObj->Set(String::NewFromUtf8(isolate, "FileName", String::kInternalizedString), String::NewFromUtf8(isolate, entry.FileName));
      _D("FileName: " << entry.FileName);
      if (!cb.IsEmpty()) {
        const unsigned argc = 1;
        Local<Value> argv[argc] = { entryObj };
        cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
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

void DUMMY(const FunctionCallbackInfo<Value>& info) {

}

// processArchive(options, cb)
void processArchive(const FunctionCallbackInfo<Value>& info) {

  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (info.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  int openMode = 0;
  bool isTest = false;
  Local<Object> options = info[0]->IsString() ? Object::New(isolate) : info[0]->ToObject();
  if (info[0]->IsString()) {
    options->Set(String::NewFromUtf8(isolate, "filepath", String::kInternalizedString), info[0]->ToString());
  }
  Local<Value> openModeValue = options->Get(String::NewFromUtf8(isolate, "openMode", String::kInternalizedString));
  if (openModeValue->IsNumber()) {
    openMode = openModeValue->NumberValue();
  }
  Local<Value> filepathValue = options->Get(String::NewFromUtf8(isolate, "filepath", String::kInternalizedString));
  if (!filepathValue->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments `filepath`")));
    return;
  }
  String::Utf8Value value(filepathValue);
  const char* filepathStr = (const char*)*value;
  char archiveFilePath[2048];
  strncpy(archiveFilePath, filepathStr, 2048);

  Local<Value> passwordValue = options->Get(String::NewFromUtf8(isolate, "password", String::kInternalizedString));
  char passwordBuf[128];
  if (passwordValue->IsString()) {
    String::Utf8Value value1(passwordValue);
    const char* passwordStr = (const char*)*value1;
    strncpy(passwordBuf, passwordStr, 128);
  }
  Local<Function> cb = (info.Length()> 1 && info[1]->IsFunction()) ? Local<Function>::Cast(info[1]) : FunctionTemplate::New(isolate, DUMMY)->GetFunction();

  Local<Value> isTestValue = options->Get(String::NewFromUtf8(isolate, "test", String::kInternalizedString));
  if (!isTestValue->IsUndefined()) {
    isTest = true;
  }

  char toDirBuf[1024] = { 0 };
  Local<Value> toDirValue = options->Get(String::NewFromUtf8(isolate, "toDir", String::kInternalizedString));
  if (openMode == 1) {
    if (toDirValue->IsString()) {
      String::Utf8Value value2(toDirValue);
      const char* toDirStr = (const char*)*value2;
      strncpy(toDirBuf, toDirStr, 1024);
    } else {
      if (!isTest) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments `toDir` for extract mode")));
        return;
      }
    }
  }

  int ret = _processArchive(isolate, openMode, isTest ? 1 : (openMode == 0 ? 0 : 2), archiveFilePath,
    toDirValue->IsString() ? toDirBuf : NULL,
    passwordValue->IsString() ? passwordBuf : NULL, cb);

  if (ret != 0) {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Process archive error")));
    _D("error code is " << ret);
  }
}

void init(Handle<Object> exports) {
  setlocale(LC_ALL,"");
  NODE_SET_METHOD(exports, "processArchive", processArchive);
}

NODE_MODULE(unrar, init);

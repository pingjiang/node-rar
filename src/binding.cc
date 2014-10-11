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
      Local<Object> entryObj = Object::New();
      entryObj->Set(String::NewSymbol("FileName"), String::New(entry.FileName));
      _D("FileName: " << entry.FileName);
      if (!cb.IsEmpty()) {
        const unsigned argc = 1;
        Local<Value> argv[argc] = { entryObj };
        cb->Call(Context::GetCurrent()->Global(), argc, argv);
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

Handle<Value> DUMMY(const Arguments& args) {
  HandleScope scope;
  
  return scope.Close(Undefined());
}

// processArchive(options, cb)
Handle<Value> processArchive(const Arguments& args) {
  HandleScope scope;
  
  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    return scope.Close(Undefined());
  }
  
  int openMode = 0;
  bool isTest = false;
  Local<Object> options = args[0]->IsString() ? Object::New() : args[0]->ToObject();
  if (args[0]->IsString()) {
    options->Set(String::NewSymbol("filepath"), args[0]->ToString());
  }
  Local<Value> openModeValue = options->Get(String::NewSymbol("openMode"));
  if (openModeValue->IsNumber()) {
    openMode = openModeValue->NumberValue();
  }
  Local<Value> filepathValue = options->Get(String::NewSymbol("filepath"));
  if (!filepathValue->IsString()) {
    ThrowException(Exception::TypeError(String::New("Wrong arguments `filepath`")));
    return scope.Close(Undefined());
  }
  String::Utf8Value value(filepathValue);
  const char* filepathStr = (const char*)*value;
  char archiveFilePath[2048]; 
  strncpy(archiveFilePath, filepathStr, 2048);
  
  Local<Value> passwordValue = options->Get(String::NewSymbol("password"));
  char passwordBuf[128];
  if (passwordValue->IsString()) {
    String::Utf8Value value1(passwordValue);
    const char* passwordStr = (const char*)*value1;
    strncpy(passwordBuf, passwordStr, 128); 
  }
  Local<Function> cb = (args.Length()> 1 && args[1]->IsFunction()) ? Local<Function>::Cast(args[1]) : FunctionTemplate::New(DUMMY)->GetFunction();
  
  Local<Value> isTestValue = options->Get(String::NewSymbol("test"));
  if (!isTestValue->IsUndefined()) {
    isTest = true;
  }
  
  char toDirBuf[1024] = { 0 }; 
  Local<Value> toDirValue = options->Get(String::NewSymbol("toDir"));
  if (openMode == 1) {
    if (toDirValue->IsString()) { 
      String::Utf8Value value2(toDirValue);
      const char* toDirStr = (const char*)*value2;
      strncpy(toDirBuf, toDirStr, 1024); 
    } else {
      ThrowException(Exception::TypeError(String::New("Wrong arguments `toDir` for extract mode")));
      return scope.Close(Undefined());
    }
  }
  
  int ret = _processArchive(openMode, isTest ? 1 : (openMode == 0 ? 0 : 2), archiveFilePath, 
    toDirValue->IsString() ? toDirBuf : NULL, 
    passwordValue->IsString() ? passwordBuf : NULL, cb);
  
  if (ret != 0) {
    ThrowException(Exception::Error(String::New("Process archive error")));
    _D("error code is " << ret);
  }
  return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
  setlocale(LC_ALL,"");
  
  exports->Set(String::NewSymbol("processArchive"), FunctionTemplate::New(processArchive)->GetFunction());
}

NODE_MODULE(unrar, init);

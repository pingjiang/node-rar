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
int _processArchive(int mode, const char* filepath, const char* toDir, Local<Function> cb) {
  struct RAROpenArchiveDataEx archiveData;
  reset_RAROpenArchiveDataEx(&archiveData);
  char archiveFilePath[2048]; strcpy(archiveFilePath, filepath);
  archiveData.ArcName = archiveFilePath;
  archiveData.OpenMode = mode;
  _D(archiveFilePath);
  HANDLE handler = RAROpenArchiveEx(&archiveData);
  if (archiveData.OpenResult != ERAR_SUCCESS) {
    _D("open archive error: " << archiveData.OpenResult);
    return archiveData.OpenResult;
  }
  
  int result = 0;
  while (result == 0) {
      struct RARHeaderDataEx entry;
      reset_RARHeaderDataEx(&entry);
      result = RARReadHeaderEx(handler, &entry);
      if (result == 0) {
        char toDirBuf[1024]; if (toDir != NULL) { strcpy(toDirBuf, toDir); }
        _D("toDir: " << toDirBuf);
        result = RARProcessFile(handler, mode == 0 ? 0 : 2, toDir != NULL ? toDirBuf : NULL, NULL);
      }
      if (result != 0)
          break;
      Local<Object> entryObj = Object::New();
      entryObj->Set(String::NewSymbol("FileName"), String::New(entry.FileName));
      // onEntry(entryObj);
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

int _listArchive(const char* filepath, Local<Function> cb) {
  return _processArchive(0, filepath, NULL, cb);
}
int _extractArchive(const char* filepath, const char* toDir, Local<Function> cb) {
  return _processArchive(1, filepath, toDir, cb);
}

Handle<Value> listArchive(const Arguments& args) {
  HandleScope scope;
  
  if (args.Length()> 0 && args[0]->IsString()) {
    Local<String> filepath = args[0]->ToString();
    if (args.Length()> 1 && args[1]->IsFunction()) {
      Local<Function> cb = Local<Function>::Cast(args[1]);
      String::Utf8Value value(filepath);
      const char* filepathStr = (const char*)*value;
      _listArchive(filepathStr, cb);
    }
  }
  // ThrowException(Exception::TypeError(String::New("Wrong arguments")));
  return scope.Close(Undefined());
}

Handle<Value> extractArchive(const Arguments& args) {
  HandleScope scope;
  
  if (args.Length()> 1 && args[0]->IsString() && args[1]->IsString()) {
    Local<String> filepath = args[0]->ToString();
    Local<String> toDir = args[1]->ToString();
    if (args.Length()> 2 && args[2]->IsFunction()) {
      Local<Function> cb = Local<Function>::Cast(args[2]);
      String::Utf8Value value(filepath);
      const char* filepathStr = (const char*)*value;
      String::Utf8Value toDirValue(toDir);
      const char* toDirStr = (const char*)*toDirValue;
      _extractArchive(filepathStr, toDirStr, cb);
    }
  }
  // ThrowException(Exception::TypeError(String::New("Wrong arguments")));
  return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
  setlocale(LC_ALL,"");
  
  exports->Set(String::NewSymbol("listArchive"), FunctionTemplate::New(listArchive)->GetFunction());
  exports->Set(String::NewSymbol("extractArchive"), FunctionTemplate::New(extractArchive)->GetFunction());
}

NODE_MODULE(unrar, init);

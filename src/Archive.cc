// #define BUILDING_NODE_EXTENSION
#include <node.h>
#include "Archive.h"

using namespace v8;
using v8::Handle;
using v8::Value;

static void reset_RARHeaderDataEx(struct RARHeaderDataEx* s) {
  memset(s->ArcName, 0, 1024);
  memset(s->ArcNameW, 0, 1024);
  memset(s->FileName, 0, 1024);
  memset(s->FileNameW, 0, 1024);
}
static void reset_RAROpenArchiveDataEx(struct RAROpenArchiveDataEx* s) {
  s->ArcName = NULL;
}

namespace NODE_RAR {

  Persistent<Function> Archive::constructor;

  void Archive::Init(Handle<v8::Object> exports) {
    // Prepare constructor template
    v8::Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol("Archive"));
    tpl->InstanceTemplate()->SetInternalFieldCount(5);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewSymbol("open"), FunctionTemplate::New(open)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("isOpen"), FunctionTemplate::New(isOpen)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("list"), FunctionTemplate::New(list)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("extract"), FunctionTemplate::New(extract)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("close"), FunctionTemplate::New(close)->GetFunction());

    constructor = Persistent<Function>::New(tpl->GetFunction());
    exports->Set(String::NewSymbol("Archive"), constructor);
  }

  v8::Handle<Value> Archive::New(const Arguments& args) {
    HandleScope scope;

    // Invoked as constructor: `new Archive()`
    Archive* obj = NULL;
    if (args.Length() > 0 && args[0]->IsString()) {
      v8::Local<String> filepath = args[0]->ToString();
      int openMode = 0;
      if (args.Length() > 1 && args[0]->IsNumber()) {
        openMode = (int)args[1]->NumberValue();
      }
      if (!args.IsConstructCall()) {
        const int argc = 2;
        v8::Local<Value> argv[argc] = { filepath, v8::Integer::New(openMode) };
        return scope.Close(constructor->NewInstance(argc, argv));
      }
      String::Utf8Value value(filepath);
      obj = new Archive((const char*)*value, openMode);
    } else {
      // Invoked as plain function `Archive(...)`, turn into construct call.
      if (!args.IsConstructCall()) {
        return scope.Close(constructor->NewInstance());
      }
      obj = new Archive();
    }
    obj->Wrap(args.This());
    return args.This();
  }

  v8::Handle<v8::Value> Archive::open(const v8::Arguments& args) {
    HandleScope scope;
  
    if (args.Length() > 0 && args[0]->IsString()) {
      v8::Local<String> filepath = args[0]->ToString();
      int openMode = 0;
      if (args.Length() > 1) {
        openMode = (int)args[1]->NumberValue();
      }
    
      Archive* obj = ObjectWrap::Unwrap<Archive>(args.This());
      String::Utf8Value value(filepath);
      int ret = obj->open((const char*)*value, openMode);
      return scope.Close(Integer::New(ret));
    }
  
    ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    return scope.Close(Undefined());
  }

  v8::Handle<v8::Value> Archive::isOpen(const v8::Arguments& args) {
    HandleScope scope;
  
    Archive* obj = ObjectWrap::Unwrap<Archive>(args.This());
    return scope.Close(Boolean::New(obj->isOpen()));
  }

  v8::Handle<v8::Value> Archive::list(const v8::Arguments& args) {
    HandleScope scope;
  
    Archive* obj = ObjectWrap::Unwrap<Archive>(args.This());
    if (obj->list() == 0) {
      return obj->entries;
    }
  
    ThrowException(Exception::TypeError(String::New("list archive filed")));
    return scope.Close(v8::Array::New());
  }

  v8::Handle<v8::Value> Archive::extract(const v8::Arguments& args) {
    HandleScope scope;
  
    Archive* obj = NULL;
    int ret = -1;
    if (args.Length() > 0 && args[0]->IsString()) {
      v8::Local<String> toDir = args[0]->ToString();
      bool isTest = false;
      if (args.Length() > 1) {
        isTest = (int)args[1]->BooleanValue();
      }
    
      Archive* obj = ObjectWrap::Unwrap<Archive>(args.This());
      String::Utf8Value value(toDir);
      ret = obj->extract((const char*)*value, isTest);
    }
    if (obj && ret == 0) {
      return obj->entries;
    }
  
    ThrowException(Exception::TypeError(String::New("extract archive filed")));
    return scope.Close(v8::Array::New());
  }

  v8::Handle<v8::Value> Archive::close(const v8::Arguments& args) {
    HandleScope scope;
  
    Archive* obj = ObjectWrap::Unwrap<Archive>(args.This());
    return scope.Close(Integer::New(obj->close()));
  }

  Archive::Archive(const char* filepath, int mode) {
    reset();
    if (filepath) {
      openError = open(filepath, mode);
    }
  }

  Archive::~Archive() {
    reset();
  }

  void Archive::reset() {
    memset(archiveFilePath, 0, 2048);
    reset_RAROpenArchiveDataEx(&archiveData);
    close();
    openError = -1;
  }

  void Archive::addEntry(v8::Handle<v8::Object> entry) {
    entries->Set(entries->Length(), entry);
  }

  int Archive::process(int op, const char* toDir) {
    int result = 0;
    while (result == 0) {
        struct RARHeaderDataEx entry;
        reset_RARHeaderDataEx(&entry);
        result = RARReadHeaderEx(handler, &entry);
        if (result == 0) {
            result = RARProcessFile(handler, op, (char*)toDir, NULL);
        }
        if (result != 0)
            break;
        v8::Handle<v8::Object> entryObj = v8::Object::New();
        entryObj->Set(String::NewSymbol("FileName"), String::New(entry.FileName));
        addEntry(entryObj);
    }
    if (result == ERAR_END_ARCHIVE) {
        result = 0;
    }
    return result;
  }

  int Archive::open(const char* filepath, int mode) {
    strcpy(archiveFilePath, filepath);
    archiveData.ArcName = archiveFilePath;
    archiveData.OpenMode = mode;
  
    handler = RAROpenArchiveEx(&archiveData);
    if (archiveData.OpenResult != ERAR_SUCCESS) {
        return archiveData.OpenResult;
    }
    // RARSetCallback(handler, cb_list, NULL);
    // RARSetProcessDataProc(handler, processData);
    return 0;
  }

  int Archive::list() {
    return this->process(RAR_SKIP);
  }
  int Archive::extract(const char* toDir, bool test) {
    return this->process(test ? RAR_TEST : RAR_EXTRACT, toDir);
  }
  int Archive::close() {
    return RARCloseArchive(handler);
  }
}

Handle<Value> Method(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New("world"));
}

void init(Handle<v8::Object> exports) {
  exports->Set(v8::String::NewSymbol("hello"), v8::FunctionTemplate::New(Method)->GetFunction());
  NODE_RAR::Archive::Init(exports);
}

NODE_MODULE(unrar, init);

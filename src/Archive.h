#ifndef __ARCHIVE_H_
#define __ARCHIVE_H_

#include <v8.h>
#include <node.h>
#include "dll.hpp"
#include <iostream>

#define _D(msg) do {\
  std::cout << __FILE__ << ":" << __LINE__ << ">> " << msg << std::endl;\
} while(0)


namespace NODE_RAR {

  class Archive : public node::ObjectWrap {
  public:
    static void Init(v8::Handle<v8::Object> exports);
    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    static v8::Persistent<v8::Function> constructor;
  
    static v8::Handle<v8::Value> open(const v8::Arguments& args);
    static v8::Handle<v8::Value> isOpen(const v8::Arguments& args);
    static v8::Handle<v8::Value> list(const v8::Arguments& args);
    static v8::Handle<v8::Value> extract(const v8::Arguments& args);
    static v8::Handle<v8::Value> close(const v8::Arguments& args);
  
  public:
    Archive(const char* filepath = NULL, int mode = RAR_OM_LIST);
    ~Archive();
    void reset();
    inline bool isOpen() const { return openError == 0; }
    int open(const char* filepath, int mode = RAR_OM_LIST);
    int list();
    int extract(const char* toDir, bool test = false);
    int close();
  
  private:
    int process(int op, const char* toDir = NULL);
    void addEntry(v8::Handle<v8::Object> entry);
  
  private:
    char archiveFilePath[2048];
    struct RAROpenArchiveDataEx archiveData;
    HANDLE handler;
    int openError;
    v8::Handle<v8::Array> entries;
  };
  
}

#endif //__ARCHIVE_H_

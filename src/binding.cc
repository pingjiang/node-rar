#include <node.h>
#include <v8.h>
#include "rar.hpp"
#include <iostream>

#define DEBUG(msg) do {\
  std::cout << __FILE__ << ":" << __LINE__ << ">> " << msg << std::endl;\
} while(0)

using namespace v8;

Handle<Value> Method(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New("world"));
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("hello"), FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(unrar, init);

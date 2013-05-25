// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)

#ifndef UTIL_TYPE_NAME_TYPE_NAME_H_
#define UTIL_TYPE_NAME_TYPE_NAME_H_

#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include <stdlib.h>

template<typename T>
std::string GetTypeName(const T& obj) {
  int status;
  char* name  = abi::__cxa_demangle(typeid(obj).name(), 0, 0, &status);
  std::string value(name);
  free(static_cast<void*>(name));
  return value;
}

template<typename T>
std::string GetTypeName() {
  int status;
  char* name  = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
  std::string value(name);
  free(static_cast<void*>(name));
  return value;
}

#endif  // UTIL_TYPE_NAME_TYPE_NAME_H_


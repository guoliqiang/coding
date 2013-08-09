// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yusun@yunrang.com (Sun Yu)

#ifndef STRING_ENUM_H_
#define STRING_ENUM_H_

#include <string>
#include <map>

#include "base/public/basictypes.h"

namespace util {

template <typename T>
class StringEnumParser {
 public:
  StringEnumParser();
  bool String2Enum(const std::string& key, T* value) {
    typename std::map<std::string, T>::const_iterator it;
    if (key.find(':') == std::string::npos) {
      std::string tofind = namespace_;
      tofind.append(key);
      it = str2type_.find(tofind);
    } else {
      it = str2type_.find(key);
    }
    if (it != str2type_.end()) {
      *value = it->second;
      return true;
    } else {
      return false;
    }
  }

  bool Enum2String(const T& key, std::string* value) {
    typename std::map<T, std::string>::const_iterator it =
        type2str_.find(key);
    if (it != type2str_.end()) {
      *value = it->second;
      return true;
    } else {
      return false;
    }
  }

  bool Enum2StringNoNamespace(const T& key, std::string* value) {
    if (Enum2String(key, value)) {
      if (namespace_len_ != 0) {
        value->assign(value->substr(namespace_len_));
      }
      return true;
    } else {
      return false;
    }
  }
 private:
  void GetNameSpace(const std::string& full_name) {
    size_t pos = full_name.find_last_of(':');
    if (pos == std::string::npos) {
      namespace_.clear();
      namespace_len_ = 0;
      return;
    }
    namespace_ = full_name.substr(0, pos + 1).c_str();
    namespace_len_ = pos + 1;
  }

  std::map<std::string, T> str2type_;
  std::map<T, std::string> type2str_;
  std::string namespace_;
  int namespace_len_;
};

#define BEGIN_STRINGMAP_PARSER(type) \
            namespace util { \
            template <> \
            inline StringEnumParser<type>::StringEnumParser() { \
              GetNameSpace(#type);
#define STRINGMAP_PARSER_PAIR(str, type) \
              str2type_[#str] = type; \
              type2str_[type] = #str;
#define END_STRINGMAP_PARSER() }}
#define STRINGMAP_ENUM(str) \
            STRINGMAP_PARSER_PAIR(str, str)
}
#endif  // UTIL_STRING_STRING_MAP_H_

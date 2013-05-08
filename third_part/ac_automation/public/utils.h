// Copyright 2011 Jike Inc. All Rights Reserved.
// Author: Ji Jian (jijian@jike.com)
// Date  : 2011-12-16 13:38:29
// File  : utils.h
#ifndef  MINSHENG_COMMON_UTILS_H_
#define  MINSHENG_COMMON_UTILS_H_

#include <libgen.h>
#include <pthread.h>
#include <dlfcn.h>
#include <vector>
#include <string>
#include "../../../base/public/basictypes.h"
#include "../../../base/public/string16.h"

namespace ac_automaton {

#define ELEMENT_COUNT(x) (sizeof(x)/sizeof(x[0]))
#define SAFE_DELETE(x) { if (x) { delete x; x = NULL; } }
#define SAFE_DELETE_ARRAY(x) { if (x) { delete []x; x = NULL; } }

template<class T>
class AutoDeleteArray {
  public:
    explicit AutoDeleteArray(T * p) : m_pArray(p) {
    }
    ~AutoDeleteArray() {
      SAFE_DELETE_ARRAY(m_pArray);
    }
    inline T * Get() {
      return m_pArray;
    }
    bool Valid() {
      return m_pArray != NULL;
    }
  private:
    T * m_pArray;
};

// Character is the minimum element in a document
// Convert UTF-8 encoded string to Unicode for further processing
// Return true when whole string is correctly converted to string16
// return false and the longest converted string in pStr when fails
bool ToString16(const std::string &utf8Str, std::basic_string<uint16> *pStr);
std::basic_string<uint16> ToString16(uint16 c);
std::string ToUTF8String(const std::basic_string<uint16> &str);
std::string ToUTF8String(uint16 c);
std::string ToUTF8String(const std::vector<uint16> &vec);
std::string ToUTF8String(const std::vector<string16> &vec);

template<class T>
std::vector<T> MergeSortedVector(const std::vector<T> &lhs,
                                 const std::vector<T> &rhs) {
  std::vector<T> re;
  for (size_t i = 0, j = 0; i < lhs.size() || j < rhs.size();) {
    if (i == lhs.size()) {
      re.push_back(rhs[j++]);
    } else if (j == rhs.size()) {
      re.push_back(lhs[i++]);
    } else {
      if (lhs[i] < rhs[j]) {
        re.push_back(lhs[i++]);
      } else if (lhs[i] == rhs[j]) {
        re.push_back(lhs[i++]);
        j++;
      } else {
        re.push_back(rhs[j++]);
      }
    }
  }
  return re;
} 
}   // end of namespace shared

#endif  // MINSHENG_COMMON_UTILS_H_

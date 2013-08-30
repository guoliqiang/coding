// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yubogong@yunrang.com (Yu Bo Gong)

#ifndef  WEB_UTIL_URL_UTIL_URL_UTIL_H_
#define  WEB_UTIL_URL_UTIL_URL_UTIL_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/public/singleton.h"
#include "base/public/logging.h"
#include "third_part/double_array_trie/public/darts.h"

typedef darts::DoubleArrayImpl<char, unsigned char, int, unsigned int>
DomainArray;

namespace url_util {

class DomainUtil {
 public:
  bool GetPrimaryDomain(const std::string &input_domain,
                        std::string *output_domain) const;

  bool GetPrimaryDomain2(const std::string &input_domain,
                         std::string *output_domain) const;

  bool FindDomain(const std::string &domain) const;

 private:
  DomainUtil();
  ~DomainUtil();

  bool LoadDomainData(const char* data_path);
  int DomainSearch(const std::string &domain,
                   DomainArray::result_pair_type *res, int len) const;

  DomainArray domain_suffix_dict;

  friend struct DefaultSingletonTraits<DomainUtil>;

  DISALLOW_COPY_AND_ASSIGN(DomainUtil);
};

void ReverseDelimString(const std::string &site,
                        std::string *result,
                        char delimiter);

inline void ReverseSite(const std::string &site, std::string *result) {
  ReverseDelimString(site, result, '.');
}

int UrlDecode(const char* source, char* dest);
int UrlEncode(const char* source, char* dest, unsigned max,
              const char *unreserved = "-_.!~*'()");
bool UrlDecodeString(const std::string& encoded, std::string* out);
bool UrlEncodeString(const std::string& decoded, std::string* out,
                     const char *unreserved = "-_.!~*'()");


bool IsUrlLikeString(const std::string& input_string);

// void ReverseSite2(const std::string &site, std::string &result);
//  Parse a key_value_delimiter separated string into key-value pairs.
void ParseKvlist(const std::string &line,
                 const std::string &key_value_delimiter,
                 char key_value_pair_delimiter,
                 std::map<std::string, std::string> *
                 kv_pair_map,
                 std::vector<std::pair<std::string, std::string> > *
                 kv_pair_vec);

}  // url_util

#endif  // WEB_UTIL_URL_UTIL_URL_UTIL_H_

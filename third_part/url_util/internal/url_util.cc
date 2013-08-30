// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yubogong@yunrang.com (Yu BO Gong)

#include "../public/url_util.h"

#include <string>
#include <algorithm>
#include <fstream>  // NOLINT

#include "base/public/string_util.h"
#include "base/public/logging.h"
#include "file/public/file.h"
#include "third_part/release/public/release_path.h"

DEFINE_string(domain_suffix_data,
              "third_part/url_util/domain_suffix.txt",
              "Data file define Domain Suffix list.");

using namespace std;  // NOLINT

namespace url_util {

DomainUtil::DomainUtil() {
  std::string path = release::GetReleaseDataPath(FLAGS_domain_suffix_data);
  CHECK(file::File::Exists(path)) << path << " doesn't exists.";
  CHECK(LoadDomainData(path.c_str()));
}

DomainUtil::~DomainUtil() {}

bool DomainUtil::LoadDomainData(const char* data_path) {
  std::vector<std::string> items;
  std::istream *is;
  std::string line;
  std::vector<const char*> key;

  is = new std::ifstream(data_path);
  if (!*is)
    goto fail;

  domain_suffix_dict.clear();

  while (std::getline(*is, line)) {
    std::string line2;
    TrimString(line, "\r\n\t",  &line2);
    StringToLowerASCII(&line2);
    std::string line3;
    ReverseSite(line2, &line3);
    items.push_back(line3);
  }

  std::sort(items.begin(), items.end());

  for (int i = 0; i < items.size(); i++) {
    char *ptr = &items[i][0];
    key.push_back(ptr);
  }

  domain_suffix_dict.build(key.size(), &key[0], 0 , 0);

  delete is;
  return true;

 fail:
  LOG(FATAL) << "load domain data file failed : " << data_path;
  return false;
}

bool DomainUtil::GetPrimaryDomain(const string &input_domain,
                                  string *output) const {
  if (domain_suffix_dict.size() == 0) {
    LOG(FATAL) << "didn't load domain suffix data!";
    return false;
  }
  size_t c = input_domain.size();
  std::string domain_string;
  ReverseSite(input_domain, &domain_string);
  StringToLowerASCII(&domain_string);
  bool tag = false;
  for (int i = 0 ; i < c; i++) {
    if (domain_string[i] == '.') {
      size_t len = i;
      std::string tmp = domain_string.substr(0, len);
      if (FindDomain(tmp)) {
        tag = true;
        continue;
      } else if (tag == false) {
        VLOG(4) << "[" << input_domain << "]" << " haven't domain data ["
                << tmp << "]";
        return false;
      } else {
        ReverseSite(tmp, output);
        return true;
      }
    }
  }
  *output = input_domain;
  return true;
}

bool DomainUtil::GetPrimaryDomain2(const string &input_domain,
                                   string *output) const {
  if (!domain_suffix_dict.array()) {
    LOG(FATAL) << "didn't load domain suffix data!";
    return false;
  }

  std::string domain_string;
  ReverseSite(input_domain, &domain_string);
  StringToLowerASCII(&domain_string);
  int c = domain_string.size();
  DomainArray::result_pair_type res[20];
  int num = DomainSearch(domain_string, res, 20);
  if (num <= 0) {
    VLOG(4) << "[" << input_domain << "]" << " haven't domain data ";
    return false;
  }
  CHECK_NE(res[num-1].value, -1);

  while (num > 0 &&
      c > res[num-1].length &&
      domain_string[res[num-1].length - 1] != '.') {
    num--;
  }

  if (num <= 0) {
    VLOG(4) << "[" << input_domain << "]" << " haven't domain data ";
    return false;
  }

  int p = res[num-1].length;
  if (p == c) {
    return false;
  }

  if (p < c) {
    p++;
  }
  while (p < c && domain_string[p] != '.') {
    p++;
  }
  std::string tmp = domain_string.substr(0, p);
  ReverseSite(tmp, output);
  return true;
}

bool DomainUtil::FindDomain(const string& domain) const {
  DomainArray::result_pair_type res;
  domain_suffix_dict.exactMatchSearch(domain.c_str(), res);
  if (res.value >= 0) {
    VLOG(5) << "Domain: " << domain << "  Found!!!";
    return true;
  } else {
    VLOG(5) << "Domain: " << domain << "  NOT Found!!!";
    return false;
  }
}

int DomainUtil::DomainSearch(const string& domain,
                             DomainArray::result_pair_type *res,
                             int len) const {
  int num = domain_suffix_dict.commonPrefixSearch(domain.c_str(), res, len);

  if (FLAGS_v > 4) {
    VLOG(5) << "num: " << num << domain;
    for (int i =0 ; i < num ; i++) {
      VLOG(5) << "value:" << res[i].value << " length:" << res[i].length;
    }
  }
  return num;
}

void ReverseDelimString(const string &site,
                        string *result,
                        char delimiter) {
  VLOG(5) << "Reverse site: " << site;
  result->clear();
  if (site.empty()) {
    return;
  }

  string::const_iterator it = site.end();
  string::const_iterator pre_it =  it;

  --it;
  while (it >= site.begin()) {
    if (*it == delimiter) {
      result->append(it + 1, pre_it);
      result->append(1, delimiter);
      VLOG(5) << "result: " << *result;
      pre_it = it;
    } else if (it == site.begin()) {
      result->append(it, pre_it);
      VLOG(5) << "result2: " << *result;
      pre_it = it;
    }
    --it;
  }
  VLOG(5) << "Reverse site result: " << *result;
}

static int HexPairValue(const char * code) {
  int value = 0;
  const char * pch = code;
  for (;;) {
    int digit = *pch++;
    if (digit >= '0' && digit <= '9') {
      value += digit - '0';
    } else if (digit >= 'A' && digit <= 'F') {
      value += digit - 'A' + 10;
    } else if (digit >= 'a' && digit <= 'f') {
      value += digit - 'a' + 10;
    } else {
      return -1;
    }
    if (pch == code + 2)
      return value;
    value <<= 4;
  }
}

int UrlDecode(const char *source, char *dest) {
  char * start = dest;

  while (*source) {
    switch (*source) {
    case '+':
      *(dest++) = ' ';
      break;
    case '%':
      if (source[1] && source[2]) {
        int value = HexPairValue(source + 1);
        if (value >= 0) {
          *(dest++) = value;
          source += 2;
        } else {
          *dest++ = '?';
        }
      } else {
        *dest++ = '?';
      }
      break;
    default:
      *dest++ = *source;
    }
    source++;
  }

  *dest = 0;
  return dest - start;
}

int UrlEncode(const char *source, char *dest, unsigned max,
              const char *unreserved) {
  static const char *digits = "0123456789ABCDEF";
  unsigned char ch;
  unsigned len = 0;
  char *start = dest;

  while (len < max - 4 && *source) {
    ch = (unsigned char) *source;
    if (*source == ' ') {
      *dest++ = '+';
    } else if (isalnum(ch) || strchr(unreserved, ch)) {
      *dest++ = *source;
    } else {
      *dest++ = '%';
      *dest++ = digits[(ch >> 4) & 0x0F];
      *dest++ = digits[ch & 0x0F];
    }
    source++;
  }
  *dest = 0;
  return start - dest;
}

bool UrlDecodeString(const string & encoded, string* out) {
  const char * sz_encoded = encoded.c_str();
  size_t needed_length = encoded.length();
  for (const char * pch = sz_encoded; *pch; pch++) {
    if (*pch == '%')
      needed_length += 2;
  }
  needed_length += 10;
  char stackalloc[64];
  char* buf = needed_length > sizeof(stackalloc) / sizeof(*stackalloc)
              ? static_cast<char *>(malloc(needed_length)): stackalloc;
  if (buf == NULL) {
    LOG(ERROR) << "fail to alloc memory";
    return false;
  }
  UrlDecode(encoded.c_str(), buf);
  out->assign(buf);
  if (buf != stackalloc) {
    free(buf);
  }
  return true;
}

bool UrlEncodeString(const string & decoded, string* out,
                     const char *unreserved) {
  size_t needed_length = decoded.length() * 3 + 3;
  char stackalloc[64];
  char* buf = needed_length > sizeof(stackalloc) / sizeof(*stackalloc)
              ? static_cast<char *>(malloc(needed_length)): stackalloc;
  if (buf == NULL) {
    LOG(ERROR) << "fail to alloc memory";
    return false;
  }
  UrlEncode(decoded.c_str(), buf, needed_length, unreserved);
  out->assign(buf);
  if (buf != stackalloc) {
    free(buf);
  }
  return true;
}

bool IsUrlLikeString(const std::string& input_string) {
  if (input_string.find(' ') != string::npos) {
    return false;
  }

  size_t pos = 0;
  if (input_string.find("http://") == 0) {
    pos += 7;
  }
  if (input_string.find("https://") == 0) {
    pos += 8;
  }

  size_t p = input_string.find('/', pos);
  int len;
  if (p == string::npos) {
    len = input_string.size() - pos;
  } else {
    len = p - pos;
  }

  string domain;
  bool is_url_like = Singleton<DomainUtil>()->
    GetPrimaryDomain2(input_string.substr(pos, len) , &domain);
  return is_url_like;
}

void ParseKvlist(
    const std::string& line,
    const std::string& key_value_delimiter,
    char key_value_pair_delimiter,
    std::map<std::string, std::string> *kv_pair_map,
    std::vector<std::pair<std::string, std::string> >* kv_pair_vec) {
  std::vector<std::string> pairs;
  std::vector<std::string> kvpair;
  SplitString(line, key_value_pair_delimiter, &pairs);
  VLOG(10) << "pairs num:" << pairs.size();
  for (int i = 0; i < pairs.size(); ++i) {
    kvpair.clear();
    SplitStringUsingSubstr(pairs[i], key_value_delimiter, &kvpair);
    if (kvpair.size() != 2) {
      continue;
    }
    VLOG(10) << "[" << kvpair[0] << "]:[" << kvpair[1] << "]";
    if (kv_pair_map) {
      kv_pair_map->insert(make_pair(kvpair[0], kvpair[1]));
    }
    if (kv_pair_vec) {
      kv_pair_vec->push_back(make_pair(kvpair[0], kvpair[1]));
    }
  }
}
}  // url_util

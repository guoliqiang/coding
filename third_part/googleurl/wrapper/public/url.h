// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)


#ifndef UTIL_URL_PARSER_YURL_H_
#define UTIL_URL_PARSER_YURL_H_

#include <string>
#include <vector>
#include "third_part/googleurl/public/gurl.h"

namespace util {
namespace url {

// get scheme from url_string
bool GetScheme(const std::string& url_string, std::string* host_string);

// get host from url_string
bool GetHost(const std::string& url_string, std::string* host_string);

// get domain from url_string
bool GetDomain(const std::string& url, std::string* domain);
bool GetDomainFromHost(const std::string& host, std::string* domain);
bool GetDomains(const std::string& url, std::vector<std::string>* domains);

// get port from url_string
bool GetPort(const std::string& url_string, std::string* port_string);

// get user name from url_string
bool GetUserName(const std::string& url_string, std::string* username_string);

// get user name from url_string
bool GetPassword(const std::string& url_string, std::string* password_string);

// get path from url_string
bool GetPath(const std::string& url_string, std::string* path_string);

// get path from url_string
bool GetQuery(const std::string& url_string, std::string* query_string);

// get path from url_string
bool GetRef(const std::string& url_string, std::string* ref_string);

// get path for request from url_string
bool GetPathForRequest(const std::string& url_string,
                       std::string* path_for_request);

// get origin of url_string
bool GetOrigin(const std::string& url_string,
               std::string* origin_string);

// encode the url_string
bool EncodeURL(const std::string& url_string, std::string* encoded_url);

// decode the url_string
bool DecodeURL(const std::string& url_string, std::string* decoded_url);

bool NormalizeURLForCrawler(const std::string& url, std::string* result);

bool NormalizeSiteHomepage(const std::string& url, std::string* result);

bool HostIsIPAddress(const std::string& url);

bool IsHostPage(const std::string& url);

bool IsHostPageEx(const std::string& url);

bool IsDomainPage(const std::string& domain, const std::string& host,
    const std::string& url);

// get url's dir level from url
int GetDirLevel(const std::string& url, bool* is_dir);

void GetDirWithLevel(const std::string& url, std::string* result, int32 depth);

bool GetSingleDirWithLevel(const std::string& url, std::string* result, int32 depth);

// decode an escaped string
// bool DecodeEscapeStr(const char* encoded_str,
//                     int len, std::string* decoded_str);


class UrlParser {
 public:
  UrlParser() {}

  ~UrlParser() {}

  explicit UrlParser(const GURL& gurl) : gurl_(gurl) {}
  explicit UrlParser(const std::string& url_string) : gurl_(url_string) {}
  explicit UrlParser(const string16& url_string) : gurl_(url_string) {}

  void Resolve(const std::string& relative, std::string* result) {
    const GURL& output = gurl_.Resolve(relative);
    result->assign(output.spec());
  }

  const std::string& Spec() const {
    return gurl_.spec();
  }
  const std::string& PossiblyInvalidSpec() const {
    return gurl_.possibly_invalid_spec();
  }

  bool IsValid() const { return gurl_.is_valid(); }

  bool IsEmpty() const { return gurl_.is_empty(); }

  bool IsStandard() const { return gurl_.IsStandard(); }

  bool SchemeIs(const char* lower_ascii_scheme) const {
    return gurl_.SchemeIs(lower_ascii_scheme);
  }

  bool SchemeIsFile() const {
    return gurl_.SchemeIsFile();
  }

  // If the scheme indicates a secure connection
  bool SchemeIsSecure() const {
    return gurl_.SchemeIsSecure();
  }

  bool HostIsIPAddress() const {
    return gurl_.HostIsIPAddress();
  }

  std::string GetOrigin() const { return gurl_.GetOrigin().spec(); }
  std::string Scheme() const { return gurl_.scheme(); }
  std::string Username() const { return gurl_.username(); }
  std::string Password() const { return gurl_.password(); }
  std::string Host() const { return gurl_.host(); }
  std::string Domain() const {
    std::string domain;
    if (GetDomainFromHost(gurl_.host(), &domain)) {
      return domain;
    } else {
      return std::string();
    }
  }
  std::string Port() const { return gurl_.port(); }
  std::string Path() const { return gurl_.path(); }
  std::string Query() const { return gurl_.query(); }
  std::string Ref() const { return gurl_.ref(); }

  bool HasScheme() const { return gurl_.has_scheme(); }
  bool HasUsername() const { return gurl_.has_username(); }
  bool HasPassword() const { return gurl_.has_password(); }
  bool HasHost() const { return gurl_.has_host(); }
  bool HasPort() const { return gurl_.has_port(); }
  bool HasPath() const { return gurl_.has_path(); }
  bool HasQuery() const { return gurl_.has_query(); }
  bool HasRef() const { return gurl_.has_ref(); }
  bool IntPort(int *port) const {
    *port = gurl_.IntPort();
    return url_parse::PORT_UNSPECIFIED != *port &&
           url_parse::PORT_INVALID != *port;
  }

  int EffectiveIntPort() const { return gurl_.EffectiveIntPort(); }

  std::string ExtractFileName() const {
    return gurl_.ExtractFileName();
  }

  std::string PathForRequest() const {
    return gurl_.PathForRequest();
  }

  std::string HostNoBrackets() const {
    return gurl_.HostNoBrackets();
  }

  bool DomainIs(const char* lower_ascii_domain) const {
    return gurl_.DomainIs(lower_ascii_domain);
  }

 private:
  GURL gurl_;

  DISALLOW_COPY_AND_ASSIGN(UrlParser);
};
}  // url
}  // namespace util

#endif  // UTIL_URL_PARSER_YURL_H_

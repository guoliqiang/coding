// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#include "../public/url.h"
#include <stdio.h>
#include <string.h>
#include <vector>

#include "base/public/string_util.h"
#include "third_part/url_util/public/url_util.h"
#include "third_part/googleurl/public/url_util.h"
#include "third_part/googleurl/public/url_canon.h"
#include "third_part/googleurl/public/url_canon_internal.h"

namespace util {
namespace url {

namespace {

const char kFileScheme[] = "file";

const char* kStandardURLSchemes[] = {
  "http",
  "https",
  "ftp",
  "gopher",
  "ws",   // WebSocket.
  "wss",  // WebSocket secure.
};

const int kNumStandardURLSchemes = sizeof(kStandardURLSchemes) /
                                   sizeof(kStandardURLSchemes[0]);

const char* kDirectoryIndex[] = {
  "/index.html",
  "/index.htm",
  "/index.php",
  "/index.jsp",
  "/default.asp",
  "/index.shtml",
  "/default.aspx"
};

const int kNumDirectoryIndex = sizeof(kDirectoryIndex) /
                               sizeof(kDirectoryIndex[0]);

const char* kHomePagePrefix[] = {
  "index",
  "default",
  "home",
  "head",
  "main"
};

const int kNumHomePagePrefix = sizeof(kHomePagePrefix) /
                               sizeof(kHomePagePrefix[0]);

const char* kHomePageSuffix[] = {
  "html",
  "htm",
  "shtml",
  "php",
  "jsp",
  "asp",
  "aspx"
};

const int kNumHomePageSuffix = sizeof(kHomePageSuffix) /
                               sizeof(kHomePageSuffix[0]);

// We treat slashes and backslashes the same for IE compatability.
inline bool IsURLSlash(char16 ch) {
  return ch == '/' || ch == '\\';
}

// This handles everything that may be an authority terminator, including
// backslash. For special backslash handling see DoParseAfterScheme.
bool IsAuthorityTerminator(char16 ch) {
  return IsURLSlash(ch) || ch == '?' || ch == '#' || ch == ';';
}

// Counts the number of consecutive slashes starting at the given offset
// in the given string of the given length.
template<typename CHAR>
inline int CountConsecutiveSlashes(const CHAR *str,
                                   int begin_offset, int str_len) {
  int count = 0;
  while (begin_offset + count < str_len &&
         IsURLSlash(str[begin_offset + count]))
    ++count;
  return count;
}

// Returns the offset of the next authority terminator in the input starting
// from start_offset. If no terminator is found, the return value will be equal
// to spec_len.
template<typename CHAR>
int FindNextAuthorityTerminator(const CHAR* spec,
                                int start_offset,
                                int spec_len) {
  for (int i = start_offset; i < spec_len; i++) {
    if (IsAuthorityTerminator(spec[i]))
      return i;
  }
  return spec_len;  // Not found.
}


// compare two components
template<typename CHAR>
inline bool CompareSchemeComponent(const CHAR* spec,
                                   const url_parse::Component& component,
                                   const char* compare_to) {
  if (!component.is_nonempty()) {
    // When component is empty, match empty scheme.
    return compare_to[0] == 0;
  }
  return url_util::LowerCaseEqualsASCII(&spec[component.begin],
                                        &spec[component.end()],
                                        compare_to);
}

// Returns true if we should trim this character from the URL because it is a
// space or a control character.
inline bool ShouldTrimFromURL(char16 ch) {
  return ch <= ' ';
}

// Given an already-initialized begin index and length, this shrinks the range
// to eliminate "should-be-trimmed" characters. Note that the length does *not*
// indicate the length of untrimmed data from |*begin|, but rather the position
// in the input string (so the string starts at character |*begin| in the spec,
// and goes until |*len|).
template<typename CHAR>
inline void TrimURL(const CHAR* spec, int* begin, int* len) {
  // Strip leading whitespace and control characters.
  while (*begin < *len && ShouldTrimFromURL(spec[*begin]))
    (*begin)++;

  // Strip trailing whitespace and control characters. We need the >i test for
  // when the input string is all blanks; we don't want to back past the input.
  while (*len > *begin && ShouldTrimFromURL(spec[*len - 1]))
    (*len)--;
}

// judge if this is a standard url
bool DoIsStandard(const char* spec, const url_parse::Component& scheme) {
  if (!scheme.is_nonempty()) {
    // Empty or invalid schemes are non-standard.
    return false;  // Empty or invalid schemes are non-standard.
  }

  for (size_t i = 0; i < kNumStandardURLSchemes; i++) {
    if (url_util::LowerCaseEqualsASCII(&spec[scheme.begin], &spec[scheme.end()],
                                       kStandardURLSchemes[i]))
      return true;
  }
  return false;
}

// parse the host:port part
template<typename CHAR>
void ParseServerInfo(const CHAR* spec,
                     const url_parse::Component& serverinfo,
                     url_parse::Component* hostname,
                     url_parse::Component* port_num) {
  if (serverinfo.len == 0) {
    // No server info, host name is empty.
    hostname->reset();
    port_num->reset();
    return;
  }

  // If the host starts with a left-bracket, assume the entire host is an
  // IPv6 literal.  Otherwise, assume none of the host is an IPv6 literal.
  // This assumption will be overridden if we find a right-bracket.
  //
  // Our IPv6 address canonicalization code requires both brackets to exist,
  // but the ability to locate an incomplete address can still be useful.
  int ipv6_terminator = spec[serverinfo.begin] == '[' ? serverinfo.end() : -1;
  int colon = -1;

  // Find the last right-bracket, and the last colon.
  for (int i = serverinfo.begin; i < serverinfo.end(); i++) {
    switch (spec[i]) {
      case ']':
        ipv6_terminator = i;
        break;
      case ':':
        colon = i;
        break;
    }
  }

  if (colon > ipv6_terminator) {
    // Found a port number: <hostname>:<port>
    *hostname = url_parse::MakeRange(serverinfo.begin, colon);
    if (hostname->len == 0)
      hostname->reset();
    *port_num = url_parse::MakeRange(colon + 1, serverinfo.end());
  } else {
    // No port: <hostname>
    *hostname = serverinfo;
    port_num->reset();
  }
}

template<typename CHAR>
void ParseUserInfo(const CHAR* spec,
                   const url_parse::Component& user,
                   url_parse::Component* username,
                   url_parse::Component* password) {
  // Find the first colon in the user section, which separates the username and
  // password.
  int colon_offset = 0;
  while (colon_offset < user.len && spec[user.begin + colon_offset] != ':')
    colon_offset++;

  if (colon_offset < user.len) {
    // Found separator: <username>:<password>
    *username = url_parse::Component(user.begin, colon_offset);
    *password = url_parse::MakeRange(user.begin + colon_offset + 1,
                          user.begin + user.len);
  } else {
    // No separator, treat everything as the username
    *username = user;
    *password = url_parse::Component();
  }
}

template<typename CHAR>
void ParsePath(const CHAR* spec,
               const url_parse::Component& path,
               url_parse::Component* filepath,
               url_parse::Component* query,
               url_parse::Component* ref) {
  // path = [/]<segment1>/<segment2>/<...>/<segmentN>;<param>?<query>#<ref>

  // Special case when there is no path.
  if (path.len == -1) {
    filepath->reset();
    query->reset();
    ref->reset();
    return;
  }
  DCHECK_GT(path.len, 0) << "We should never have 0 length paths";

  // Search for first occurrence of either ? or #.
  int path_end = path.begin + path.len;

  int query_separator = -1;  // Index of the '?'
  int ref_separator = -1;    // Index of the '#'
  for (int i = path.begin; i < path_end; i++) {
    switch (spec[i]) {
      case '?':
        // Only match the query string if it precedes the reference fragment
        // and when we haven't found one already.
        if (ref_separator < 0 && query_separator < 0)
          query_separator = i;
        break;
      case '#':
        // Record the first # sign only.
        if (ref_separator < 0)
          ref_separator = i;
        break;
    }
  }
  // Markers pointing to the character after each of these corresponding
  // components. The code below words from the end back to the beginning,
  // and will update these indices as it finds components that exist.
  int file_end, query_end;

  // Ref fragment: from the # to the end of the path.
  if (ref_separator >= 0) {
    file_end = query_end = ref_separator;
    *ref = url_parse::MakeRange(ref_separator + 1, path_end);
  } else {
    file_end = query_end = path_end;
    ref->reset();
  }

  // Query fragment: everything from the ? to the next boundary (either the end
  // of the path or the ref fragment).
  if (query_separator >= 0) {
    file_end = query_separator;
    *query = url_parse::MakeRange(query_separator + 1, query_end);
  } else {
    query->reset();
  }

  // File path: treat an empty file path as no file path.
  if (file_end != path.begin)
    *filepath = url_parse::MakeRange(path.begin, file_end);
  else
    filepath->reset();
}

}  // namespace {


namespace {
// .s. new made  functions
void GetAuthorityRange(const char* spec,
                       int spec_len,
                       int after_scheme,
                       int* auth_start,
                       int* auth_end) {
  int num_slashes = CountConsecutiveSlashes(spec, after_scheme, spec_len);
  *auth_start = after_scheme + num_slashes;
  *auth_end = FindNextAuthorityTerminator(spec, *auth_start, spec_len);
}

// get user name from standard URL
int GetUserInfoFromStandardURL(const char* spec,
                               int spec_len,
                               int after_scheme,
                               url_parse::Component* username,
                               url_parse::Component* password) {
  // get begin and end position of Authority
  int auth_start = 0;
  int auth_end = 0;
  GetAuthorityRange(spec, spec_len, after_scheme,
                    &auth_start, &auth_end);

  VLOG(5) << "authority start:" << spec+auth_start
          << ",len:" << auth_end-auth_start;
  if (auth_end-auth_start <= 0) {
    return -1;
  }

  // get begin and position of ServerInfo(host:port)
  // Search backwards for @, which is the separator between the user info and
  // the server info.
  int i = auth_end - 1;
  while (i > auth_start && spec[i] != '@') {
    i--;
  }

  // parse ServerInfo and  get host and port
  if (spec[i] == '@') {
    ParseUserInfo(spec, url_parse::Component(auth_start, i - auth_start),
                  username, password);
  }

  return 0;
}

// get host from standard URL
bool GetHostFromStandardURL(const char* spec,
                           int spec_len,
                           int after_scheme,
                           url_parse::Component* hostname) {
  // get begin and end position of Authority
  int auth_start = 0;
  int auth_end = 0;
  GetAuthorityRange(spec, spec_len, after_scheme,
                    &auth_start, &auth_end);

  VLOG(5) << "authority start:" << spec+auth_start
          << ",len:" << auth_end-auth_start;
  if (auth_end-auth_start <= 0) {
    return false;
  }

  // get begin and position of ServerInfo(host:port)
  // Search backwards for @, which is the separator between the user info and
  // the server info.
  int i = auth_end - 1;
  while (i > auth_start && spec[i] != '@') {
    i--;
  }

  url_parse::Component port_num;
  // parse ServerInfo and  get host and port
  if (spec[i] == '@') {
    ParseServerInfo(spec, url_parse::MakeRange(i+1, auth_end),
                    hostname, &port_num);
  } else {
    // No user info, everything is server info.
    ParseServerInfo(spec,
      url_parse::MakeRange(auth_start, auth_end),
      hostname, &port_num);
  }

  return true;
}

// get port from standard URL
bool GetPortFromStandardURL(const char* spec,
                           int spec_len,
                           int after_scheme,
                           url_parse::Component* port_num) {
  // get begin and end position of Authority
  int auth_start = 0;
  int auth_end = 0;
  GetAuthorityRange(spec, spec_len, after_scheme,
                    &auth_start, &auth_end);

  VLOG(5) << "authority start:" << spec+auth_start
          << ",len:" << auth_end-auth_start;
  if (auth_end-auth_start <= 0) {
    return false;
  }

  // get begin and position of ServerInfo(host:port)
  // Search backwards for @, which is the separator between the user info and
  // the server info.
  int i = auth_end - 1;
  while (i > auth_start && spec[i] != '@') {
    i--;
  }

  url_parse::Component hostname;
  // parse ServerInfo and  get host and port
  if (spec[i] == '@') {
    ParseServerInfo(spec, url_parse::MakeRange(i+1, auth_end),
                    &hostname, port_num);
  } else {
    // No user info, everything is server info.
    ParseServerInfo(spec,
      url_parse::MakeRange(auth_start, auth_end),
      &hostname, port_num);
  }

  return true;
}

// get path information from standard URL
bool GetPathFromStandardURL(const char* spec,
                           int spec_len,
                           int after_scheme,
                           url_parse::Component* filepath,
                           url_parse::Component* query,
                           url_parse::Component* ref) {
  // path = [/]<segment1>/<segment2>/<...>/<segmentN>;<param>?<query>#<ref>
  // get begin and end position of Authority
  int auth_start = 0;
  int auth_end = 0;
  GetAuthorityRange(spec, spec_len, after_scheme,
                    &auth_start, &auth_end);

  url_parse::Component full_path;
  if (auth_end == spec_len) {
    // No beginning of path found.
    full_path = url_parse::Component();
  } else {
    // Everything starting from the slash to the end is the path.
    full_path = url_parse::Component(auth_end, spec_len - auth_end);
  }

  ParsePath(spec, full_path, filepath, query, ref);

  return true;
}

}  // namespace {

// .e. new made small functions
// get path from url_string
bool GetPath(const std::string& url_string, std::string* path_string) {
  VLOG(5) << url_string;
  if (path_string == NULL) {
    return false;
  }
  *path_string = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(url_string.data(),
                            in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;
  // before canonicialize
  url_parse::Component path_component;
  url_parse::Component query_component;
  url_parse::Component ref_component;
  // after canonicialize
  url_parse::Component new_path;
  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  std::string output_string;
  url_canon::StdStringCanonOutput output(&output_string);

  if (DoIsStandard(spec, scheme)) {
    // get host from standard URL
    GetPathFromStandardURL(spec,
                           spec_len,
                           after_scheme,
                           &path_component,
                           &query_component,
                           &ref_component);
    if (path_component.len > 0) {
      url_canon::CanonicalizePath(spec, path_component,
                                  &output, &new_path);
      output.Complete();
      path_string->assign(output_string.data(), new_path.begin, new_path.len);
    } else {
      // no host found
      *path_string = "/";
    }
  } else {
    // not do any here
    GURL gurl(url_string);
    *path_string =  gurl.path();
  }
  return true;
}

// get path from url_string
bool GetQuery(const std::string& url_string, std::string* query_string) {
  VLOG(5) << url_string;
  if (query_string == NULL) {
    return false;
  }
  *query_string = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(url_string.data(),
                            in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;
  // before canonicialize
  url_parse::Component path_component;
  url_parse::Component query_component;
  url_parse::Component ref_component;
  // after canonicialize
  url_parse::Component new_query;
  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  std::string output_string;
  url_canon::StdStringCanonOutput output(&output_string);

  if (DoIsStandard(spec, scheme)) {
    // get host from standard URL
    GetPathFromStandardURL(spec,
                           spec_len,
                           after_scheme,
                           &path_component,
                           &query_component,
                           &ref_component);
    if (query_component.len > 0) {
      url_canon::CanonicalizeQuery(spec, query_component, NULL,
                                   &output, &new_query);
      output.Complete();
      query_string->assign(
          output_string.data(), new_query.begin, new_query.len);
    } else {
      // no host found
      *query_string = "";
    }
  } else {
    // not do any here
    GURL gurl(url_string);
    *query_string =  gurl.query();
  }
  return true;
}


// get path for request
bool GetPathForRequest(const std::string& url_string,
                      std::string* path_for_request) {
  VLOG(5) << url_string;
  if (path_for_request == NULL) {
    return false;
  }
  *path_for_request = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(url_string.data(),
                            in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;
  // before canonicialize
  url_parse::Component path_component;
  url_parse::Component query_component;
  url_parse::Component ref_component;
  // after canonicialize
  url_parse::Component new_path;
  url_parse::Component new_query;
  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  std::string output_string;
  url_canon::StdStringCanonOutput output(&output_string);

  if (DoIsStandard(spec, scheme)) {
    // get host from standard URL
    GetPathFromStandardURL(spec,
                           spec_len,
                           after_scheme,
                           &path_component,
                           &query_component,
                           &ref_component);
    url_canon::CanonicalizePath(spec, path_component, &output, &new_path);
    if (query_component.len > 0) {
      url_canon::CanonicalizeQuery(spec, query_component, NULL,
                                   &output, &new_query);
    }
    output.Complete();
    *path_for_request = output_string;
  } else {
    // not do any here
    GURL gurl(url_string);
    *path_for_request =  gurl.PathForRequest();
  }
  return true;
}

// get path from url_string
bool GetRef(const std::string& url_string, std::string* ref_string) {
  VLOG(5) << url_string;
  if (ref_string == NULL) {
    return false;
  }
  *ref_string = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(url_string.data(),
                            in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;
  // before canonicialize
  url_parse::Component path_component;
  url_parse::Component query_component;
  url_parse::Component ref_component;
  // after canonicialize
  url_parse::Component new_ref;
  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  std::string output_string;
  url_canon::StdStringCanonOutput output(&output_string);

  if (DoIsStandard(spec, scheme)) {
    // get host from standard URL
    GetPathFromStandardURL(spec,
                           spec_len,
                           after_scheme,
                           &path_component,
                           &query_component,
                           &ref_component);
    if (ref_component.len > 0) {
      url_canon::CanonicalizeRef(spec, ref_component,
                                  &output, &new_ref);
      output.Complete();
      ref_string->assign(output_string.data(), new_ref.begin, new_ref.len);
    } else {
      // no host found
      *ref_string = "";
    }
  } else {
    // not do any here
    GURL gurl(url_string);
    *ref_string =  gurl.ref();
  }
  return true;
}

/// get Port from url_string
bool GetScheme(const std::string& url_string, std::string* scheme_string) {
  VLOG(5) << url_string;
  if (scheme_string == NULL) {
    return false;
  }
  *scheme_string = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(
      url_string.data(),
      in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // after canonicialize
  url_parse::Component new_scheme;


  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  scheme_string->reserve(url_string.size() + 32);
  std::string output_string;
  url_canon::StdStringCanonOutput output(&output_string);

  if (DoIsStandard(spec, scheme)) {
    // get the default port
    url_canon::CanonicalizeScheme(spec, scheme,
                                  &output, &new_scheme);

    output.Complete();
    scheme_string->assign(output_string.data(),
                          new_scheme.begin, new_scheme.len);
  } else {
    // not do any here
    GURL gurl(url_string);
    *scheme_string =  gurl.scheme();
  }

  return true;
}

// get origin of url_string
bool GetOrigin(const std::string& url_string,
              std::string* origin_string) {
  VLOG(5) << url_string;
  if (origin_string == NULL) {
    return false;
  }
  *origin_string = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(url_string.data(),
                            in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;

  url_parse::Component host_name;
  url_parse::Component port_name;
  // after canonicialize
  url_parse::Component new_scheme;
  url_parse::Component new_port;
  url_parse::Component new_host;

  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  std::string output_string;
  url_canon::StdStringCanonOutput output(&output_string);

  if (DoIsStandard(spec, scheme)) {
    // get scheme
    url_canon::CanonicalizeScheme(spec, scheme,
                                  &output, &new_scheme);
    // get host
    GetHostFromStandardURL(spec, spec_len,
                           after_scheme, &host_name);
    if (host_name.len > 0) {
      url_canon::CanonicalizeHost(spec, host_name,
                                  &output, &new_host);
    }
    // get port
    GetPortFromStandardURL(spec, spec_len,
                           after_scheme, &port_name);
    // get the default port
    int default_port = url_canon::DefaultPortForScheme(spec, scheme.len);
    url_canon::CanonicalizePort(spec, port_name, default_port,
                                &output, &new_port);
    // make output string
    output.Complete();

    origin_string->append(
        output_string.substr(new_scheme.begin, new_scheme.len));
    origin_string->append("://");
    origin_string->append(output_string.substr(new_host.begin, new_host.len));
    if (new_port.len > 0) {
      origin_string->append(":");
      origin_string->append(output_string.substr(new_port.begin, new_port.len));
    }
    origin_string->append("/");
  } else {
    // not do any here
    GURL gurl(url_string);
    *origin_string = gurl.GetOrigin().spec();
  }
  return true;
}

// get user name from url_string
bool GetUserName(const std::string& url_string, std::string* user_name) {
  VLOG(5) << url_string;
  if (user_name == NULL) {
    return false;
  }
  *user_name = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(url_string.data(),
                            in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;
  // before canonicialize
  url_parse::Component user_name_component;
  url_parse::Component password_component;
  // after canonicialize
  url_parse::Component new_user_name;
  url_parse::Component new_password;
  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  user_name->reserve(url_string.size() + 32);
  ::std::string output_string;
  url_canon::StdStringCanonOutput output(&output_string);

  if (DoIsStandard(spec, scheme)) {
    // get host from standard URL
    GetUserInfoFromStandardURL(spec, spec_len,
                               after_scheme,
                               &user_name_component,
                               &password_component);
    if (user_name_component.len > 0) {
      url_canon::CanonicalizeUserInfo(spec, user_name_component,
                                      spec, password_component,
                                      &output, &new_user_name, &new_password);
      output.Complete();
      user_name->assign(output_string.data(),
                        new_user_name.begin,
                        new_user_name.len);
    } else {
      // no host found
      *user_name = "";
    }
  } else {
    // not do any here
    GURL gurl(url_string);
    *user_name =  gurl.username();
  }

  return true;
}


// get user name from url_string
bool GetPassword(const std::string& url_string, std::string* password_string) {
  VLOG(5) << url_string;
  if (password_string == NULL) {
    return false;
  }
  *password_string = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(url_string.data(),
                            in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;
  // before canonicialize
  url_parse::Component user_name_component;
  url_parse::Component password_component;
  // after canonicialize
  url_parse::Component new_user_name;
  url_parse::Component new_password;
  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  password_string->reserve(url_string.size() + 32);
  ::std::string output_string;
  url_canon::StdStringCanonOutput output(&output_string);

  if (DoIsStandard(spec, scheme)) {
    // get host from standard URL
    GetUserInfoFromStandardURL(spec, spec_len,
                               after_scheme,
                               &user_name_component,
                               &password_component);
    if (user_name_component.len > 0) {
      url_canon::CanonicalizeUserInfo(spec, user_name_component,
                                      spec, password_component,
                                      &output, &new_user_name, &new_password);
      output.Complete();
      password_string->assign(output_string.data(),
                              new_password.begin, new_password.len);
    } else {
      // no host found
      *password_string = "";
    }
  } else {
    // not do any here
    GURL gurl(url_string);
    *password_string =  gurl.password();
  }

  return true;
}

// get host from url_string
bool GetHost(const std::string& url_string, std::string* host_string) {
  VLOG(5) << url_string;
  if (host_string == NULL) {
    return false;
  }
  *host_string = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(
      url_string.data(),
      in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;
  // before canonicialize
  url_parse::Component host_name;
  // after canonicialize
  url_parse::Component new_host;
  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  host_string->reserve(url_string.size() + 32);
  url_canon::StdStringCanonOutput output(host_string);

  if (DoIsStandard(spec, scheme)) {
    // get host from standard URL
    GetHostFromStandardURL(spec, spec_len,
                           after_scheme, &host_name);
    if (host_name.len > 0) {
      url_canon::CanonicalizeHost(spec, host_name,
                                  &output, &new_host);
      output.Complete();
    } else {
      // no host found
      host_string ->clear();
    }
  } else {
    // not do any here
    GURL gurl(url_string);
    host_string->assign(gurl.host());
  }

  return true;
}

// get Port from url_string
bool GetPort(const std::string& url_string, std::string* port_string) {
  VLOG(5) << url_string;
  if (port_string == NULL) {
    return false;
  }
  *port_string = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(url_string.data(),
                            in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;
  // before canonicialize
  url_parse::Component port_name;
  // after canonicialize
  url_parse::Component new_port;
  // Reserve enough room in the output for the input, plus some extra so that
  // we have room if we have to escape a few things without reallocating.
  port_string->reserve(url_string.size() + 32);
  std::string output_string;
  url_canon::StdStringCanonOutput output(&output_string);

  if (DoIsStandard(spec, scheme)) {
    // get host from standard URL
    GetPortFromStandardURL(spec, spec_len,
                           after_scheme, &port_name);
    // get the default port
    int default_port = url_canon::DefaultPortForScheme(spec, scheme.len);
    url_canon::CanonicalizePort(spec, port_name, default_port,
                                &output, &new_port);
    output.Complete();
    port_string->assign(output_string.data(), new_port.begin, new_port.len);
  } else {
    // not do any here
    GURL gurl(url_string);
    *port_string =  gurl.port();
  }

  return true;
}

bool GetDomainFromHost(const std::string& host, std::string* domain) {
  return Singleton<url_util::DomainUtil>()->GetPrimaryDomain2(host, domain);
}

bool GetDomain(const std::string& url, std::string* domain) {
  static const std::string ip_chars("0123456789.");
  std::string host;
  GetHost(url, &host);
  if (host.find_first_not_of(ip_chars) == std::string::npos) {
    domain->swap(host);
    return true;
  }
  return GetDomainFromHost(host, domain);
}

bool GetDomains(const std::string& url, std::vector<std::string>* domains) {
  std::string domain;
  bool ok = GetDomain(url, &domain);
  if (ok && NULL != domains) {
    domains->clear();
    domains->push_back(domain);
    size_t pos = domain.rfind('.');
    while (pos != std::string::npos) {
      domains->push_back(domain.substr(0, pos));
      pos = domain.rfind('.', pos - 1);
    }
  }
  return ok;
}

// encode the url_string
bool EncodeURL(const std::string& url_string, std::string* encoded_url) {
  VLOG(5) << url_string;
  if (encoded_url == NULL) {
    return false;
  }
  *encoded_url = "";

  // remove all white spaces(/n /r /t)
  url_canon::RawCanonOutputT<char> whitespace_buffer;

  int in_spec_len = static_cast<int>(url_string.length());
  int spec_len;
  const char* spec = url_canon::RemoveURLWhitespace(url_string.data(),
                            in_spec_len, &whitespace_buffer, &spec_len);

  // trim the spaces before and end of the url
  int begin = 0;
  TrimURL(spec, &begin, &spec_len);
  spec = spec + begin;
  spec_len -= begin;

  // parse the scheme
  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(spec, spec_len, &scheme)) {
    return false;
  }

  // go over the ':'
  int after_scheme = scheme.len+1;
  // before canonicialize
  url_parse::Component path_component;
  url_parse::Component query_component;
  url_parse::Component ref_component;
  // after canonicialize
  url_parse::Component new_query;
  std::string encode_buff = "";
  url_canon::StdStringCanonOutput output(&encode_buff);

  if (DoIsStandard(spec, scheme)) {
    // get host from standard URL
    GetPathFromStandardURL(spec,
                           spec_len,
                           after_scheme,
                           &path_component,
                           &query_component,
                           &ref_component);
    if (query_component.len > 0) {
      encoded_url->assign(spec, scheme.begin, query_component.begin);
      // just encode the "query"
      url_canon::CanonicalizeQuery(spec, query_component, NULL,
                                   &output, &new_query);
      output.Complete();
      // append the other parts
      encoded_url->append(encode_buff.substr(new_query.begin, new_query.len));
      encoded_url->append(spec+query_component.begin+query_component.len);
    } else {
      // no query, no need to encode
      *encoded_url = url_string;
      return true;
    }
  } else {
    // not do any here
    GURL gurl(url_string);
    *encoded_url =  gurl.spec();
  }
  return true;
}

bool DecodeURL(const std::string& url_string, std::string* decoded_url) {
  return url_util::UrlDecodeString(url_string, decoded_url);
}

bool NormalizeURLForCrawler(const std::string& url, std::string* result) {
  result->assign(url);
  int strip_size = 0;
  for (std::string::reverse_iterator it = result->rbegin();
       it != result->rend(); ++it) {
    if (*it == '&' || *it == '?') {
      strip_size++;
    } else {
      break;
    }
  }

  if (strip_size > 0) {
    result->resize(result->size() - strip_size);
  }

  GURL gurl(*result);
  if (!gurl.is_valid() || !gurl.username().empty() ||
      !gurl.password().empty() ||
      (!gurl.SchemeIs("http") && !gurl.SchemeIs("https"))) {
    result->clear();
    return false;
  }

  // Clear Ref
  GURL::Replacements repl;
  std::string replaced_ref = "";
  if (gurl.has_ref()) {
    if (result->find("#!") == std::string::npos) {
      url_parse::Component comp;
      repl.SetRef(replaced_ref.c_str(), comp);
    } else {
      // if #! exist, assume ref is part of path
      url_parse::Component in_comp(0, gurl.ref().size());
      url_parse::Component out_comp;
      url_canon::StdStringCanonOutput canon_output(&replaced_ref);
      bool success = url_canon::CanonicalizePath(
          gurl.ref().c_str(), in_comp, &canon_output, &out_comp);
      canon_output.Complete();
      if (success) {
        if (gurl.ref() != replaced_ref) {
          // CanonicalizePath will add / at the head when path without / start
          url_parse::Component comp;
          comp.len = replaced_ref.size() - 1;
          repl.SetRef(replaced_ref.c_str() + 1, comp);
        }
      } else {
        LOG(WARNING) << "canonicalize path failed:" << gurl.ref();
      }
    }
  }

  std::string replace_path;
  const std::string& url_path = gurl.path();
  if (url_path.find("//") != std::string::npos) {
    size_t path_size = url_path.size();

    char last_char_in_replace = 0;
    for (size_t i = 0; i < path_size; ++i) {
      char char_in_url_path = url_path[i];
      if (char_in_url_path != '/' || last_char_in_replace != '/') {
        replace_path += char_in_url_path;
        last_char_in_replace = char_in_url_path;
      }
    }
    url_parse::Component comp;
    comp.len = static_cast<int>(replace_path.size());
    repl.SetPath(replace_path.c_str(), comp);
  }
  GURL output = gurl.ReplaceComponents(repl);
  result->assign(output.spec());

  while (true) {
    int orig_size = result->length();
    ReplaceSubstringsAfterOffset(result, 0, "&amp;", "&");
    if (result->length() == orig_size) {
      break;
    }
  }
  return true;
}

bool NormalizeSiteHomepage(const std::string& url, std::string* result) {
  std::string path;
  GetPath(url, &path);
  for (int i = 0; i < kNumDirectoryIndex; i++) {
    if (!base::StrCaseCmp(path.c_str(), kDirectoryIndex[i])) {
      result->assign(url, 0, url.find(path.c_str()) + 1);
      return true;
    }
  }
  return false;
}

bool HostIsIPAddress(const std::string& url) {
  static const std::string ip_chars("0123456789.");
  std::string host;
  if (!GetHost(url, &host)) {
    return false;
  }
  return host.find_first_not_of(ip_chars) == std::string::npos;
}

bool IsHostPage(const std::string& url) {
  util::url::UrlParser url_parser(url);
  if (url_parser.HasUsername() ||
      url_parser.HasQuery() ||
      !url_parser.HasHost()) {
    return false;
  }
  const std::string& path = url_parser.Path();
  if (0 == path.compare("/")) {
    return true;
  }
  for (int i = 0; i < kNumDirectoryIndex; i++) {
    if (0 == path.compare(kDirectoryIndex[i])) {
      return true;
    }
  }
  return false;
}

bool IsHostPageEx(const std::string& url) {
  util::url::UrlParser url_parser(url);
  if (url_parser.HasUsername() ||
      url_parser.HasQuery() ||
      !url_parser.HasHost()) {
    return false;
  }
  const std::string& path = url_parser.Path();
  if (0 == path.compare("/")) {
    return true;
  }
  for (int i = 0; i < kNumHomePagePrefix; i++) {
    for (int j = 0; j < kNumHomePageSuffix; j++) {
      const std::string& end = StringPrintf("/%s.%s",
          kHomePagePrefix[i], kHomePageSuffix[j]);
      if (0 == path.compare(end)) {
        return true;
      }
    }
  }
  return false;
}

int GetDirLevel(const std::string& url, bool* is_dir) {
  std::string path;
  std::vector<std::string> path_split_str;
  int dir_level = -1;
  if (GetPath(url, &path)) {
    SplitString(path, '/', &path_split_str);
    dir_level = 0;
    int len = path_split_str.size();
    for (int i = 0; i < len; ++i) {
      if (!path_split_str[i].empty()) {
        ++dir_level;
      }
    }
    bool dir = true;
    //  only find '.' at last 6 char
    const int max_find_len = 6;
    if (len > 0) {
      int last_seg_len = path_split_str[len - 1].size();
      size_t pos = last_seg_len > max_find_len ?
        last_seg_len - max_find_len : 0;
      if (path_split_str[len - 1].find('.', pos) != std::string::npos) {
        dir = false;
        --dir_level;
      }
    }
    if (is_dir != NULL) { *is_dir = dir; }
  }
  return dir_level;
}

void GetDirWithLevel(const std::string& url, std::string* result, int32 depth) {
  util::url::UrlParser url_parser(url);
  std::string host = url_parser.Host();
  std::string path = url_parser.Path();

  result->assign(host);

  int32 cur_dep = 0;
  size_t level_start = 0;
  for (size_t i = 0; i < path.size(); ++i) {
    if (cur_dep > depth) {
      break;
    }
    if (path[i] == '/') {
      cur_dep++;
      result->append(path.substr(level_start, i - level_start + 1));
      level_start = i + 1;
    }
  }
}

bool GetSingleDirWithLevel(const std::string& url,
                           std::string* result, int32 depth) {
  util::url::UrlParser url_parser(url);
  std::string host = url_parser.Host();
  std::string path = url_parser.Path();
  VLOG(3) << "Path:" << path;
  result->clear();

  int32 cur_dep = 0;
  size_t level_start = 0;
  for (size_t i = 0; i < path.size(); ++i) {
    if (cur_dep > depth) {
      break;
    }
    if (path[i] == '/') {
      cur_dep++;
      if (cur_dep == depth + 1) {
        result->append(path.substr(level_start, i - level_start));
        return true;
      }
      level_start = i + 1;
    }
  }
  if (cur_dep == depth) {
    result->append(path.substr(level_start));
    return true;
  }
  return false;
}

bool IsDomainPage(const std::string& domain, const std::string& host,
    const std::string& url) {
  bool is_domain_page = false;
  std::string url_tmp = "http://" + host + "/";
  if (url == url_tmp) {
    is_domain_page = true;
  }
  for (int i = 0; (i < kNumDirectoryIndex) && !is_domain_page; i++) {
    url_tmp = "http://" + host + "/" + kDirectoryIndex[i];
    if (url == url_tmp) {
      is_domain_page = true;
    }
  }
  if (!is_domain_page) {
    return false;
  }
  std::string tmp = "www." + domain;
  return ((domain == host)||( tmp == host));
}


}  // namespace url
}  // namespace util

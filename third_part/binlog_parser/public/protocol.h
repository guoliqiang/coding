// Copyright (c) 2003, 2011, Oracle and/or its affiliates. All rights
// reserved.
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of
// the License.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
// 02110-1301  USA

#ifndef THIRD_PART_BINLOG_PARSER_PUBLIC_PROTOCOL_H
#define	THIRD_PART_BINLOG_PARSER_PUBLIC_PROTOCOL_H

#include <list>
#include "third_part/binlog_parser/public/binlog_event.h"

namespace binlog_parser {

enum enum_field_types {
  MYSQL_TYPE_DECIMAL,
  MYSQL_TYPE_TINY,
  MYSQL_TYPE_SHORT,
  MYSQL_TYPE_LONG,
  MYSQL_TYPE_FLOAT,
  MYSQL_TYPE_DOUBLE,
  MYSQL_TYPE_NULL,
  MYSQL_TYPE_TIMESTAMP,
  MYSQL_TYPE_LONGLONG,
  MYSQL_TYPE_INT24,
  MYSQL_TYPE_DATE,
  MYSQL_TYPE_TIME,
  MYSQL_TYPE_DATETIME,
  MYSQL_TYPE_YEAR,
  MYSQL_TYPE_NEWDATE,
  MYSQL_TYPE_VARCHAR,
  MYSQL_TYPE_BIT,
  MYSQL_TYPE_TIMESTAMP2,
  MYSQL_TYPE_DATETIME2,
  MYSQL_TYPE_TIME2,
  MYSQL_TYPE_NEWDECIMAL = 246,
  MYSQL_TYPE_ENUM = 247,
  MYSQL_TYPE_SET = 248,
  MYSQL_TYPE_TINY_BLOB = 249,
  MYSQL_TYPE_MEDIUM_BLOB = 250,
  MYSQL_TYPE_LONG_BLOB = 251,
  MYSQL_TYPE_BLOB = 252,
  MYSQL_TYPE_VAR_STRING = 253,
  MYSQL_TYPE_STRING = 254,
  MYSQL_TYPE_GEOMETRY = 255
};


#define int3store(T,A) do { *(T)=  (unsigned char) ((A));\
                            *(T+1)=(unsigned char) (((unsigned int) (A) >> 8));\
                            *(T+2)=(unsigned char) (((A) >> 16)); } while (0)

class buffer_source;

// The Protocol interface is used to describe a grammar consisting of
// chunks of bytes that are read or written in consequtive order.
// Example:
// class Protocol_impl : public Protocol;
// Protocol_impl chunk1(chunk_datastore1);
// Protocol_impl chunk2(chunk_datastore2);
// input_stream >> chunk1
//              >> chunk2;
class Protocol {
 public:
  Protocol() { m_length_encoded_binary = false; }
  // Return the number of bytes which is read or written by this protocol chunk.
  // The default size is equal to the underlying storage data type.
  virtual unsigned int size() = 0;
  // Return a pointer to the first byte in a linear storage buffer
  virtual const char *data() = 0;
  // Change the number of bytes which will be read or written to the storage.
  // The default size is euqal to the storage type size. This can change if the
  // protocol is reading a length encoded binary.
  // The new size must always be less than the size of the underlying storage
  // type.
  virtual void collapse_size(unsigned int new_size) = 0;
  // The first byte will have a special interpretation which will indicate
  // how many bytes should be read next.
  void set_length_encoded_binary(bool bswitch) {
    m_length_encoded_binary= bswitch;
  }
  bool is_length_encoded_binary(void) { return m_length_encoded_binary; }

 private:
  bool m_length_encoded_binary;
  friend std::istream &operator<<(std::istream &is, Protocol &chunk);
  friend std::istream &operator>>(std::istream &is, Protocol &chunk);
  friend buffer_source &operator>>(buffer_source &src, Protocol &chunk);
  friend std::istream &operator>>(std::istream &is, std::string &str);
};

template<typename T>
class Protocol_chunk : public Protocol {
 public:
  Protocol_chunk() : Protocol() {
    m_size = 0;
    m_data = 0;
  }

  Protocol_chunk(T &chunk) : Protocol() {
    m_data = (const char *)&chunk;
    m_size = sizeof(T);
  }

  Protocol_chunk(const T &chunk) : Protocol () {
    m_data = (const char *) &chunk;
    m_size = sizeof(T);
  }

  Protocol_chunk(T *buffer, unsigned long size) : Protocol () {
    m_data = (const char *)buffer;
    m_size = size;
  }

  virtual unsigned int size() { return m_size; }
  virtual const char *data() { return m_data; }
  virtual void collapse_size(unsigned int new_size) {
    memset((char *)m_data + new_size,'\0', m_size - new_size);
    m_size = new_size;
  }

 private:
  const char *m_data;
  unsigned long m_size;
};

std::ostream & operator<<(std::ostream &os, Protocol &chunk);
typedef Protocol_chunk<boost::uint8_t> Protocol_chunk_uint8;

class Protocol_chunk_string {
 public:
  Protocol_chunk_string(std::string &chunk, unsigned long size) {
    m_str= &chunk;
    m_str->assign(size,'*');
  }
  virtual unsigned int size() const { return m_str->size(); }
  virtual const char *data() const { return m_str->data(); }
  virtual void collapse_size(unsigned int new_size) {
    m_str->resize(new_size);
  }

 private:
  friend std::istream &operator>>(std::istream &is, Protocol_chunk_string &str);
  std::string *m_str;
};

class Protocol_chunk_vector : public Protocol_chunk_uint8 {
 public:
  Protocol_chunk_vector(std::vector<boost::uint8_t> &chunk,
        unsigned long size) : Protocol_chunk_uint8() {
    m_vec = &chunk;
    m_vec->reserve(size);
    m_size= size;
  }
  virtual unsigned int size() { return m_vec->size(); }
  virtual const char *data() {
    return reinterpret_cast<const char *>(&*m_vec->begin());
  }
  virtual void collapse_size(unsigned int new_size) {
    m_vec->resize(new_size);
  }
 private:
  friend std::istream &operator>>(std::istream &is,
                                  Protocol_chunk_vector &chunk);
  std::vector<boost::uint8_t> *m_vec;
  unsigned long m_size;
};


std::istream &operator>>(std::istream &is, Protocol_chunk_vector &chunk);

class buffer_source {
 public:
  buffer_source(const char *src, int sz) {
    m_src = src;
    m_size = sz;
    m_ptr = 0;
  }
  friend buffer_source & operator>>(buffer_source &src, Protocol &chunk);
 private:
  const char *m_src;
  int m_size;
  int m_ptr;
};

class Protocol_chunk_string_len {
 public:
  Protocol_chunk_string_len(std::string &str) {
    m_storage = &str;
  }

 private:
  friend std::istream &operator>>(std::istream &is,
                                  Protocol_chunk_string_len &lenstr);
  std::string *m_storage;
};

buffer_source &operator>>(buffer_source &src, Protocol &chunk);
std::istream &operator>>(std::istream &is, Protocol &chunk);
std::istream &operator>>(std::istream &is, std::string &str);
std::istream &operator>>(std::istream &is, Protocol_chunk_string_len &lenstr);
std::istream &operator>>(std::istream &is, Protocol_chunk_string &str);

// Allocates a new event and copy the header. The caller must be responsible for
// releasing the allocated memory.
Query_event *proto_query_event(std::istream &is, Log_event_header *header);
Rotate_event *proto_rotate_event(std::istream &is, Log_event_header *header);
Incident_event *proto_incident_event(std::istream &is,
                                     Log_event_header *header);
Row_event *proto_rows_event(std::istream &is, Log_event_header *header);
Table_map_event *proto_table_map_event(std::istream &is,
                                       Log_event_header *header);
Int_var_event *proto_intvar_event(std::istream &is, Log_event_header *header);
User_var_event *proto_uservar_event(std::istream &is, Log_event_header *header);

}  // namespace binlog_parser

#endif	// THIRD_PART_BINLOG_PARSER_PUBLIC_PROTOCOL_H

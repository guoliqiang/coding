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

#ifndef THIRD_PART_BINLOG_PARSER_PUBLIC_FILE_DRIVER_H
#define	THIRD_PART_BINLOG_PARSER_PUBLIC_FILE_DRIVER_H

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "third_part/binlog_parser/public/binlog_api.h"
#include "third_part/binlog_parser/public/binlog_driver.h"
#include "third_part/binlog_parser/public/protocol.h"

#define MAGIC_NUMBER_SIZE 4

namespace binlog_parser {

class Binlog_file_driver : public Binary_log_driver {
 public:
  template <class TFilename>
  Binlog_file_driver(const TFilename& filename = TFilename(),
                     unsigned int offset = 0)
    : Binary_log_driver(filename, offset) {}
  int connect();
  int disconnect();
  int wait_for_next_event(Binary_log_event **event);
  int set_position(const std::string &str, unsigned long position);
  int get_position(std::string *str, unsigned long *position);

 private:
  unsigned long m_binlog_file_size;
  unsigned long m_bytes_read;
  std::ifstream m_binlog_file;
  Log_event_header m_event_log_header;
};

} // namespace binlog_parser

#endif	// THIRD_PART_BINLOG_PARSER_PUBLIC_FILE_DRIVER_H

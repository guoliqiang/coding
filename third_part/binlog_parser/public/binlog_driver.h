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

#ifndef THIRD_PART_BINLOG_PAESER_PUBLIC_BINLOG_DRIVER_H
#define	THIRD_PART_BINLOG_PAESER_PUBLIC_BINLOG_DRIVER_H

#include "third_part/binlog_parser/public/binlog_event.h"
#include "third_part/binlog_parser/public/protocol.h"

namespace binlog_parser {

class Binary_log_driver {
 public:
  template <class FilenameT>
  Binary_log_driver(const FilenameT& filename = FilenameT(),
                    unsigned int offset = 0) : m_binlog_file_name(filename),
                                               m_binlog_offset(offset) {}
  ~Binary_log_driver() {}
  // Connect to the binary log using previously declared connection parameters
  // 0 Success, >0 Error code (to be specified)
  virtual int connect() = 0;

  // Blocking attempt to get the next binlog event from the stream
  // @param event [out] Pointer to a binary log event to be fetched.
  virtual int wait_for_next_event(Binary_log_event ** event) = 0;

  // Set the reader position. str the file name. position is the file position
  // @return False on success and True if an error occurred.
  virtual int set_position(const std::string &str, unsigned long position) = 0;

  // Get the read position.
  // string_ptr Pointer to location where the filename will be stored.
  // position_ptr Pointer to location where the position will be stored.
  // 0 Success, >0 Error code
  virtual int get_position(std::string *filename_ptr,
                           unsigned long *position_ptr) = 0;
  Binary_log_event* parse_event(std::istream &sbuff, Log_event_header *header);

 protected:
  // Used each time the client reconnects to the server to specify an
  // offset position.
  std::string m_binlog_file_name;
  unsigned long m_binlog_offset;
};

} // namespace binlog_parser

#endif	// THIRD_PART_BINLOG_PAESER_PUBLIC_BINLOG_DRIVER_H

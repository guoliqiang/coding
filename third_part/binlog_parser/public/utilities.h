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

#ifndef THIRD_PART_BINLOG_PARSER_PUBLIC_UTILITIES_H
#define THIRD_PART_BINLOG_PARSER_PUBLIC_UTILITIES_H

#include <map>
#include <vector>
#include "third_part/binlog_parser/public/value.h"
#include "third_part/binlog_parser/public/protocol.h"

namespace binlog_parser {

typedef enum {
  Q_FLAGS2_CODE= 0,
  Q_SQL_MODE_CODE,
  Q_CATALOG_CODE,
  Q_AUTO_INCREMENT,
  Q_CHARSET_CODE,
  Q_TIME_ZONE_CODE,
  Q_CATALOG_NZ_CODE,
  Q_LC_TIME_NAMES_CODE,
  Q_CHARSET_DATABASE_CODE,
  Q_TABLE_MAP_FOR_UPDATE_CODE,
  Q_MASTER_DATA_WRITTEN_CODE,
  Q_INVOKER
} enum_var_types;

int server_var_decoder(std::map<std::string, Value> *my_var_map,
                       std::vector<boost::uint8_t > variables);

}  // namespace binlog_parser

#endif  // THIRD_PART_BINLOG_PARSER_PUBLIC_UTILITIES_H

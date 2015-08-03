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

#include <cstdlib>
#include "base/public/logging.h"
#include "third_part/boost/include/boost/foreach.hpp"
#include "third_part/binlog_parser/public/binlog_api.h"
#include "third_part/binlog_parser/public/utilities.h"

using namespace binlog_parser;

int main (int argc, char* argv[]) {
  Binary_log
  binlog(create_transport("/var/lib/mysql/binlog.000002"));

  if (binlog.connect()) {
    LOG(INFO) << "Can't connect!";
    return 0;
  }
  LOG(INFO) << "Connected to server!!";

  if (binlog.set_position(4) != ERR_OK) {
    LOG(INFO) << "Can't reposition the binary log reader.";
    exit(1);
  }

  Binary_log_event *event;
  bool quit=false;
  while (!quit) {
    if (binlog.wait_for_next_event (&event)) {
      quit= true;
      continue;
    }
    LOG(INFO) << "Pos = " << event->header()->next_position
              <<" Event_type = " << event->get_event_type();
    switch (event->header()->type_code) {
      case QUERY_EVENT: {
          const Query_event * qev =
              static_cast<const Query_event *>(event);
          LOG(INFO) << "Query = " << qev->query << " DB = " << qev->db_name;
          std::map<std::string, Value> my_var_map;
          if (server_var_decoder(&my_var_map, qev->variables)) {
            return 0;
          }
          Converter converter;
          typedef std::map<std::string, Value>::value_type my_pair;
          BOOST_FOREACH (my_pair &ref, my_var_map) {
            std::string value;
            converter.to(value, ref.second);
            LOG(INFO) << ref.first << " = " << value;
          }
          LOG(INFO) << "----------";

          if (qev->query.find("DROP DATABASE `sys_var`") != std::string::npos ||
              qev->query.find("DROP DATABASE sys_var") != std::string::npos) {
            quit = true;
          }
          break;
        }
      default:
        break;
    }
    delete event;
  }
  return 0;
}

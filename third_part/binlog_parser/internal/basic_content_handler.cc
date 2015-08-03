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

#include "base/public/logging.h"
#include "third_part/binlog_parser/public/basic_content_handler.h"
#include "third_part/boost/include/boost/bind.hpp"

namespace binlog_parser {

Content_handler::Content_handler () {}
Content_handler::~Content_handler () {}

Binary_log_event * Content_handler::process_event(Query_event *ev) {
  return ev;
}
Binary_log_event *Content_handler::process_event(Table_map_event *ev) {
  return ev;
}
Binary_log_event *Content_handler::process_event(User_var_event *ev) {
  return ev;
}
Binary_log_event *Content_handler::process_event(Incident_event *ev) {
  return ev;
}
Binary_log_event *Content_handler::process_event(Rotate_event *ev) {
  return ev;
}
Binary_log_event *Content_handler::process_event(Int_var_event *ev) {
  return ev;
}
Binary_log_event *Content_handler::process_event(Binary_log_event *ev) {
  return ev;
}
Binary_log_event * Content_handler::process_event(Row_event *ev) { return ev; }
Binary_log_event *Content_handler::process_event(Xid *ev) { return ev; }

Injection_queue *Content_handler::get_injection_queue(void) {
  return m_reinject_queue;
}

void Content_handler::set_injection_queue(Injection_queue *queue) {
  m_reinject_queue = queue;
}

Binary_log_event * Content_handler::internal_process_event(
    Binary_log_event *ev) {
 Binary_log_event * processed_event = NULL;
 LOG(INFO) << "type:" << (int)ev->header ()->type_code;
 switch(ev->header ()->type_code) {
   case QUERY_EVENT: {
     processed_event = process_event(static_cast<Query_event*>(ev));
     break;
   }
   case WRITE_ROWS_EVENT:
   case UPDATE_ROWS_EVENT:
   case DELETE_ROWS_EVENT: {
     processed_event = process_event(static_cast<Row_event*>(ev));
     break;
   }
   case USER_VAR_EVENT: {
     processed_event = process_event(static_cast<User_var_event *>(ev));
     break;
   }
   case ROTATE_EVENT: {
     processed_event = process_event(static_cast<Rotate_event *>(ev));
     break;
   }
   case INCIDENT_EVENT: {
     processed_event = process_event(static_cast<Incident_event *>(ev));
     break;
   }
   case XID_EVENT: {
     processed_event = process_event(static_cast<Xid *>(ev));
     break;
   }
   case TABLE_MAP_EVENT: {
     processed_event = process_event(static_cast<Table_map_event *>(ev));
     break;
   }
   case FORMAT_DESCRIPTION_EVENT: {
     processed_event = process_event(ev);
     break;
   }
   case BEGIN_LOAD_QUERY_EVENT: {
     processed_event = process_event(ev);
     break;
   }
   case EXECUTE_LOAD_QUERY_EVENT: {
     processed_event = process_event(ev);
     break;
   }
   case INTVAR_EVENT: {
     processed_event = process_event(ev);
     break;
   }
   case STOP_EVENT: {
     processed_event = process_event(ev);
     break;
   }
   case RAND_EVENT: {
     processed_event = process_event(ev);
     break;
   }
   default: {
     processed_event = process_event(ev);
     break;
   }
 }
 return processed_event;
}

}  // namespace binlog_parser

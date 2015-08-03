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

// mysql settings
// /etc/mysql/my.cnf add:
// [mysqld]
// server-id = 1
// log-bin = binlog
// log-bin-index = binlog.index
// binlog_format = ROW

#include <stdlib.h>
#include "base/public/logging.h"
#include "third_part/boost/include/boost/foreach.hpp"
#include "third_part/binlog_parser/public/binlog_api.h"

using namespace binlog_parser;

bool is_text_field(Value &val) {
  if (val.type() == MYSQL_TYPE_VARCHAR ||
      val.type() == MYSQL_TYPE_BLOB ||
      val.type() == MYSQL_TYPE_MEDIUM_BLOB ||
      val.type() == MYSQL_TYPE_LONG_BLOB) return true;
  return false;
}

void table_insert(const std::string& table_name, Row_of_fields &fields) {
  std::stringstream out;
  out << "INSERT INTO " << table_name << " VALUES (";
  Row_of_fields::iterator field_it= fields.begin();
  Converter converter;
  do {
    // Each row contains a vector of Value objects. The converter
    // allows us to transform the value into another representation.
    std::string str;
    converter.to(str, *field_it);
    if (is_text_field(*field_it)) out << '\'';
    out << str;
    if (is_text_field(*field_it)) out << '\'';
    ++field_it;
    if (field_it != fields.end()) out << ", ";
  } while(field_it != fields.end());
  out << ")";
  LOG(INFO) << out.str();
}

void table_update(const std::string& table_name,
                  Row_of_fields &old_fields,
                  Row_of_fields &new_fields) {
  std::stringstream out;
  out << "UPDATE " << table_name << " SET ";
  int field_id = 0;
  Row_of_fields::iterator field_it = new_fields.begin();
  Converter converter;
  do {
    std::string str;
    converter.to(str, *field_it);
    out << field_id << "= ";
    if (is_text_field(*field_it)) out << '\'';
    out << str;
    if (is_text_field(*field_it)) out << '\'';
    ++field_it;
    ++field_id;
    if (field_it != new_fields.end()) out << ", ";
  } while(field_it != new_fields.end());
  out << " WHERE ";
  field_it = old_fields.begin();
  field_id = 0;
  do {
    std::string str;
    converter.to(str, *field_it);
    out << field_id << "= ";
    if (is_text_field(*field_it)) out << '\'';
    out << str;
    if (is_text_field(*field_it)) out << '\'';
    ++field_it;
    ++field_id;
    if (field_it != old_fields.end()) out << " AND ";
  } while(field_it != old_fields.end());
  out << " LIMIT 1";
  LOG(INFO) << out.str();
}

void table_delete(const std::string& table_name, Row_of_fields &fields) {
  std::stringstream out;
  out << "DELETE FROM " << table_name << " WHERE ";
  Row_of_fields::iterator field_it = fields.begin();
  int field_id= 0;
  Converter converter;
  do {
    std::string str;
    converter.to(str, *field_it);
    out << field_id << "= ";
    if (is_text_field(*field_it)) out << '\'';
    out << str;
    if (is_text_field(*field_it)) out << '\'';
    ++field_it;
    ++field_id;
    if (field_it != fields.end()) out << " AND ";
  } while(field_it != fields.end());
  out << " LIMIT 1";
  LOG(INFO) << out.str();
}

class Incident_handler : public Content_handler {
 public:
  Incident_handler() : Content_handler() {}
  Binary_log_event *process_event(Incident_event *incident) {
   LOG(INFO) << "Event type: "
             << get_event_type_str(incident->get_event_type())
             << " length: " << incident->header()->event_length
             << " next pos: " << incident->header()->next_position;
   LOG(INFO) << "type= "
             << (unsigned)incident->type
             << " message= "
             << incident->message;
   // Consume the event
   delete incident;
   return 0;
 }
};

class Replay_binlog : public Content_handler {
 public:
  Replay_binlog() : Content_handler() {}
  Binary_log_event *process_event(Binary_log_event *event) {
    if (event->get_event_type() != USER_DEFINED) return event;
    LOG(INFO) << "Event type: "
              << get_event_type_str(event->get_event_type())
              << " length: " << event->header()->event_length
              << " next pos: " << event->header()->next_position;
    Transaction_log_event * trans =
        static_cast<Transaction_log_event *>(event);
    // The transaction event we created has aggregated all row events in an
    // ordered list.
    BOOST_FOREACH(Binary_log_event * event, trans->m_events) {
      switch (event->get_event_type()) {
        case WRITE_ROWS_EVENT:
        case DELETE_ROWS_EVENT:
        case UPDATE_ROWS_EVENT: {
          Row_event * rev= static_cast<Row_event *>(event);
          boost::uint64_t table_id = rev->table_id;
          // BUG: will create a new event header if the table id doesn't exist.
          Binary_log_event * tmevent = trans->table_map()[table_id];
          Table_map_event * tm;
          if (tmevent != NULL) {
            tm = static_cast<Table_map_event *>(tmevent);
          } else {
            LOG(INFO) << "Table id " << table_id
                      << "was not registered by any preceding table map event.";
            continue;
          }
          // Each row event contains multiple rows and fields. The Row_iterator
          // allows us to iterate one row at a time.
          Row_event_set rows(rev, tm);
          // Create a fuly qualified table name
          std::ostringstream os;
          os << tm->db_name << '.' << tm->table_name;
          Row_event_set::iterator it = rows.begin();
          do {
            Row_of_fields fields = *it;
            if (event->get_event_type() == WRITE_ROWS_EVENT) {
              table_insert(os.str(), fields);
            }
            if (event->get_event_type() == UPDATE_ROWS_EVENT) {
              ++it;
              Row_of_fields fields2 = *it;
              table_update(os.str(), fields, fields2);
            }
            if (event->get_event_type() == DELETE_ROWS_EVENT) {
              table_delete(os.str(), fields);
            }
          } while (++it != rows.end());
          break;
        }
        default: {
          break;
        }
      }
    }
    // Consume the event
    delete trans;
    return 0;
  }
};
int main(int argc, char** argv) {
  if (argc != 2) {
    LOG(INFO) << "Usage:\nreplaybinlog file_path\nExample:\nreplaybinlog "
                 "/var/lib/mysql/binlog.000005";
    return 0;
  }
  Binary_log binlog(create_transport(argv[1]));
  // Attach a custom event parser which produces user defined events
  Basic_transaction_parser transaction_parser;
  Incident_handler incident_hndlr;
  Replay_binlog replay_hndlr;

  binlog.content_handler_pipeline()->push_back(&transaction_parser);
  binlog.content_handler_pipeline()->push_back(&incident_hndlr);
  binlog.content_handler_pipeline()->push_back(&replay_hndlr);

  if (binlog.connect()) {
    LOG(INFO) << "Can't connect to the master.";
    return 0;
  }
  
  if (binlog.set_position(4) != ERR_OK) {
    LOG(INFO) << "Can't reposition the binary log reader.";
    return 0;
  }
  Binary_log_event  * event;
  bool quit= false;
  while(!quit) {
    // Pull events from the master. This is the heart beat of the event listener.
    if (binlog.wait_for_next_event(&event)) {
      quit= true;
      continue;
    }
    // Print the event
    LOG(INFO) << "Event type: "
              << get_event_type_str(event->get_event_type())
              << " length: " << event->header()->event_length
              << " next pos: " << event->header()->next_position;

    // Perform a special action based on event type
    switch(event->header()->type_code) {
      case QUERY_EVENT: {
        const Query_event *qev =
            static_cast<const Query_event *>(event);
        LOG(INFO) << "query= " << qev->query << " db= " << qev->db_name;
        if (qev->query.find("DROP TABLE REPLICATION_LISTENER")
            != std::string::npos ||
            qev->query.find("DROP TABLE `REPLICATION_LISTENER`")
            != std::string::npos) {
          quit= true;
        }
        break;
      }
    case ROTATE_EVENT: {
        Rotate_event *rot= static_cast<Rotate_event *>(event);
        LOG(INFO) << "filename= " << rot->binlog_file.c_str() << " pos= "
                  << rot->binlog_pos;
        break;
      }
    }
    delete event;
  }
  return 0;
}

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

// TODO The Transaction_log_event and Basic_transaction_parser will be removed
// from this library and replaced  with a table map indexer instead which can be
// used to retrive table names.

#ifndef THIRD_PART_BINLOG_PAESER_PUBLIC_BASIC_TRANSACTION_PARSER_H
#define	THIRD_PART_BINLOG_PAESER_PUBLIC_BASIC_TRANSACTION_PARSER_H

#include <list>
#include <iostream>
#include <map>
#include "third_part/boost/include/boost/cstdint.hpp"
#include "third_part/binlog_parser/public/binlog_event.h"
#include "third_part/binlog_parser/public/basic_content_handler.h"

namespace binlog_parser {

typedef std::pair<boost::uint64_t, Binary_log_event *> Event_index_element;
typedef std::map<boost::uint64_t, Binary_log_event *> Int_to_Event_map;

class Transaction_log_event : public Binary_log_event {
 public:
  Transaction_log_event() : Binary_log_event() {}
  Transaction_log_event(Log_event_header *header) : Binary_log_event(header) {}
  virtual ~Transaction_log_event();
  Int_to_Event_map &table_map() { return m_table_map; }

  Int_to_Event_map m_table_map;
  std::list<Binary_log_event *> m_events;
};

Transaction_log_event *create_transaction_log_event(void);

class Basic_transaction_parser : public Content_handler {
 public:
  Basic_transaction_parser() : Content_handler() {
    m_transaction_state = NOT_IN_PROGRESS;
  }
  Binary_log_event * process_event(Query_event *ev);
  Binary_log_event * process_event(Row_event *ev);
  Binary_log_event * process_event(Table_map_event *ev);
  Binary_log_event * process_event(Xid *ev);
  Binary_log_event * process_event(Binary_log_event *ev) { return ev; }

 private:
  boost::uint32_t m_start_time;
  enum Transaction_states {
    STARTING,
    IN_PROGRESS,
    COMMITTING,
    NOT_IN_PROGRESS
  } m_transaction_state;
  std::list<Binary_log_event *> m_event_stack;
 
 private:
  Binary_log_event * process_transaction_state(Binary_log_event *ev);
};

}  // namespace binlog_parser

#endif	// THIRD_PART_BINLOG_PAESER_PUBLIC_BASIC_TRANSACTION_PARSER_H

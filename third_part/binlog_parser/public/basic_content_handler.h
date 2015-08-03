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

#ifndef THIRD_PART_BINLOG_PAESER_PUBLIC_BASIC_CONTENT_HANDLER_H
#define	THIRD_PART_BINLOG_PAESER_PUBLIC_BASIC_CONTENT_HANDLER_H

#include "third_part/binlog_parser/public/binlog_event.h"

namespace binlog_parser {

class Injection_queue : public std::list<Binary_log_event *> {
 public:
  Injection_queue() : std::list<Binary_log_event * >() {}
  ~Injection_queue() {}
};

// A content handler accepts an event and returns the same event,
// a new one or 0 (the event was consumed by the content handler).
// The default behaviour is to return the event unaffected.
// The generic event handler is used for events which aren't routed to
// a dedicated member function, user defined events being the most
// common case.
class Content_handler {
 public:
  Content_handler();
  Content_handler(const Content_handler& orig);
  virtual ~Content_handler();
  virtual Binary_log_event *process_event(Query_event *ev);
  virtual Binary_log_event *process_event(Row_event *ev);
  virtual Binary_log_event *process_event(Table_map_event *ev);
  virtual Binary_log_event *process_event(Xid *ev);
  virtual Binary_log_event *process_event(User_var_event *ev);
  virtual Binary_log_event *process_event(Incident_event *ev);
  virtual Binary_log_event *process_event(Rotate_event *ev);
  virtual Binary_log_event *process_event(Int_var_event *ev);
  // Process any event which hasn't been registered yet.
  virtual Binary_log_event *process_event(Binary_log_event *ev);

 protected:
  // The Injection queue is emptied before any new event is pulled from
  // the Binary_log_driver. Injected events will pass through all content
  // handlers. The Injection_queue is a derived std::list.
  Injection_queue *get_injection_queue();

private:
  Injection_queue *m_reinject_queue;
  void set_injection_queue(Injection_queue *injection_queue);
  Binary_log_event *internal_process_event(Binary_log_event *ev);

  friend class Binary_log;
};

}  // namespace binlog_parser

#endif	// THIRD_PART_BINLOG_PAESER_PUBLIC_BASIC_CONTENT_HANDLER_H

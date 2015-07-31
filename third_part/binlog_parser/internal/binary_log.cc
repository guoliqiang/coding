/*
Copyright (c) 2003, 2011, Oracle and/or its affiliates. All rights
reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2 of
the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
02110-1301  USA
*/

#include <list>
#include "third_part/binlog_parser/public/binlog_api.h"
#include "third_part/boost/include/boost/foreach.hpp"

using namespace mysql::system;

namespace mysql {

Binary_log::Binary_log(Binary_log_driver *drv)
    : m_binlog_position(4), m_binlog_file("") {
  if (drv == NULL) m_driver= &m_dummy_driver;
  else m_driver= drv;
}

Content_handler_pipeline * Binary_log::content_handler_pipeline() {
  return &m_content_handlers;
}

int Binary_log::wait_for_next_event(mysql::Binary_log_event ** event_ptr) {
  int rc = 0;
  bool handler_code = false;
  mysql::Binary_log_event * event;
  mysql::Injection_queue reinjection_queue;
  do {
    handler_code= false;
    if (!reinjection_queue.empty()) {
      event = reinjection_queue.front();
      reinjection_queue.pop_front();
    } else {
      if((rc = m_driver->wait_for_next_event(&event)) != 0) {
        // Return in case of non-ERR_OK.
        return rc;
      }
    }
    m_binlog_position = event->header()->next_position;
    mysql::Content_handler * handler;
    BOOST_FOREACH(handler, m_content_handlers) {
      if (event) {
        handler->set_injection_queue(&reinjection_queue);
        event = handler->internal_process_event(event);
      }
    }
  } while(event == 0 || !reinjection_queue.empty());
  if (event_ptr) *event_ptr = event;
  return 0;
}

int Binary_log::set_position(const std::string &filename,
                             unsigned long position) {
  int status = m_driver->set_position(filename, position);
  if (status == ERR_OK) {
    m_binlog_file = filename;
    m_binlog_position = position;
  }
  return status;
}

int Binary_log::set_position(unsigned long position) {
  std::string filename;
  m_driver->get_position(&filename, NULL);
  return this->set_position(filename, position);
}

unsigned long Binary_log::get_position(std::string &filename) {
  m_driver->get_position(&m_binlog_file, &m_binlog_position);
  filename = m_binlog_file;
  return m_binlog_position;
}

unsigned long Binary_log::get_position(void) { return m_binlog_position; }
int Binary_log::connect() { return m_driver->connect(); }

}  // namespace mysql

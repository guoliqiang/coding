// @author Mats Kindahl <mats.kindahl@oracle.com>
// This is a basic example that just opens a binary log either from a
// file or a server and print out what events are found.  It uses a
// simple event loop and checks information in the events using a
// switch.

#include "base/public/logging.h"
#include "third_part/binlog_parser/public/binlog_api.h"
#include <iostream>
#include <map>
#include <string>

// Here is a basic system using the event loop to fetch context events
// and store them in an associative array.
using namespace binlog_parser;

// Class to maintain variable values.
template <class AssociativeContainer>
class Save_variables : public Content_handler {
 public:
  Save_variables(AssociativeContainer& container) : m_var(container) { }
  Binary_log_event *process_event(User_var_event *event) {
    m_var[event->name] = event->value;
    return NULL;
  }

 private:
  AssociativeContainer &m_var;
};


template <class AssociativeContainer>
class Replace_variables : public Content_handler {
public:
  Replace_variables(AssociativeContainer& variables) : m_var(variables) { }

  Binary_log_event *process_event(Query_event *event) {
    std::string *query = &event->query;
    size_t start, end = 0;
    while (true) {
      start = query->find_first_of("@", end);
      if (start == std::string::npos) break;
      end = query->find_first_not_of("abcdefghijklmnopqrstuvwxyz", start+1);
      std::string key = query->substr(start + 1, end - start - 1);
      query->replace(start, end - start, "'" + m_var[key] + "'");
    }
    return event;
  }
 private:
  AssociativeContainer &m_var;
};


int main(int argc, char** argv) {
  typedef std::map<std::string, std::string> Map;
  if (argc != 2) {
    LOG(INFO) << "Usage: basic-2 <uri>";
    return 0;
  }

  Binary_log binlog(create_transport(argv[1]));
  binlog.connect();
  binlog.set_position(4);

  Map variables;
  Save_variables<Map> save_variables(variables);
  binlog.content_handler_pipeline()->push_back(&save_variables);
  Replace_variables<Map> replace_variables(variables);
  binlog.content_handler_pipeline()->push_back(&replace_variables);

  while (true) {
    Binary_log_event *event;
    int result = binlog.wait_for_next_event(&event);
    if (result == ERR_EOF) break;
    switch (event->get_event_type()) {
      case QUERY_EVENT:
        LOG(INFO) << static_cast<Query_event*>(event)->query;
        break;
      default:
        break;
    }
  }
}

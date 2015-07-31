// @file basic-1
// @author Mats Kindahl <mats.kindahl@oracle.com>
// This is a basic example that just opens a binary log either from a
// file or a server and print out what events are found.  It uses a
// simple event loop and checks information in the events using a
// switch.

#include "base/public/logging.h"
#include "third_part/binlog_parser/public/binlog_api.h"

using mysql::Binary_log;
using mysql::system::create_transport;

int main(int argc, char** argv) {
  if (argc != 2) {
    LOG(INFO) << "Usage: basic-2 <uri>";
    return 0;
  }

  Binary_log binlog(create_transport(argv[1]));
  binlog.connect();
  Binary_log_event *event;

  while (true) {
    int result = binlog.wait_for_next_event(&event);
    if (result == ERR_EOF) break;
    LOG(INFO) << "Found event of type " << event->get_event_type();
  }
}

// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "util/mysql_client2/connection.h"

#include "base/flags.h"
#include "base/logging.h"

DEFINE_string(mysql_host, "localhost", "mysql server host name");
DEFINE_string(mysql_db, "social", "test database on mysql server");
DEFINE_string(mysql_usr, "root", "mysql server login user name");
DEFINE_string(mysql_password, "root", "password to login mysql server");
DEFINE_string(mysql_socket,
              "/var/run/mysqld/mysqld.sock",
              "socket file path be used to connect mysql server");
DEFINE_int32(mysql_port, 1234, "port be used to connect mysql server");

util_mysql::ConnectionOption option;
using base::ParseCommandLineFlags;

int main(int32 argc, char **argv) {
  base::ParseCommandLineFlags(&argc, &argv, true);
  option.host_ = FLAGS_mysql_host;
  option.database_ = FLAGS_mysql_db;
  option.password_ =  FLAGS_mysql_password;
  option.user_ = FLAGS_mysql_usr;
  option.port_ = FLAGS_mysql_port;
  option.socket_ = FLAGS_mysql_socket;

  util_mysql::MysqlConnection  conn(option);
  CHECK(conn.Connect());
  LOG(INFO) << "Connect work";

  CHECK(conn.IsAlive());
  LOG(INFO) << "IsAlive work";

  conn.Reconnect();

  CHECK(conn.IsAlive());
  LOG(INFO) << "reconnection work";

  conn.Disconnect();

  CHECK_EQ(conn.IsAlive(), false);
  LOG(INFO) << "disconnect working";

  conn.Disconnect();
  conn.KeepAlive();

  CHECK(conn.IsAlive());
  LOG(INFO) << "KeepAlive working";

  return 0;
}

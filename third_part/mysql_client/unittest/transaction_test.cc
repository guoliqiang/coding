// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "base/logging.h"
#include "base/flags.h"
#include "base/string_util.h"

#include "util/mysql_client2/transaction.h"
#include "util/mysql_client2/connection.h"
#include "util/mysql_client2/query.h"

DEFINE_string(mysql_host, "localhost", "mysql server host name");
DEFINE_string(mysql_db, "social", "test database on mysql server");
DEFINE_string(mysql_usr, "root", "mysql server login user name");
DEFINE_string(mysql_password, "root", "password to login mysql server");
DEFINE_string(mysql_socket,
              "/var/run/mysqld/mysqld.sock",
              "socket file path be used to connect mysql server");
DEFINE_int32(mysql_port, 1234, "port be used to connect mysql server");

DEFINE_int32(tran_commit, 0, "");

using base::ParseCommandLineFlags;
util_mysql::ConnectionOption option;

int main(int argc, char **argv) {
  ParseCommandLineFlags(&argc, &argv, true);
  option.host_ = FLAGS_mysql_host;
  option.database_ = FLAGS_mysql_db;
  option.password_ =  FLAGS_mysql_password;
  option.user_ = FLAGS_mysql_usr;
  option.port_ = FLAGS_mysql_port;
  option.socket_ = FLAGS_mysql_socket;

  util_mysql::MysqlConnection  conn(option);
  util_mysql::MysqlQuery query(&conn);

  query.Execute("DROP TABLE IF EXISTS test_table");
  query.Execute("CREATE TABLE test_table( " \
                "id   BIGINT(20) unsigned NOT NULL AUTO_INCREMENT,"
                "col1 INT,"
                "col2 INT UNSIGNED,"
                "col3 BIGINT,"
                "col4 BIGINT UNSIGNED,"
                "col5 double,"
                "col6 bool,"
                "col7 varchar(5),"
                "col8 blob,"
                "col9 TIMESTAMP,"
                "col10 int,"
                "PRIMARY KEY (id))"
                "ENGINE=InnoDB");

  util_mysql::MysqlTransaction tran(&conn,
                                 util_mysql::MysqlTransaction::REPEATABLEREAD);
  query.Execute("insert into test_table(col1) values(100)");
  query.Execute("insert into test_table(col1) values(200)");
  query.Execute("insert into test_table(col1) values(300)");
  query.Execute("insert into test_table(col1) values(400)");
  query.Execute("insert into test_table(col1) values(500)");

  if (FLAGS_tran_commit == 0)
    CHECK(tran.Rollback());
  else
    CHECK(tran.Commit());
  return 0;
}

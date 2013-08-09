// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "../public/query.h"

#include "../../../base/public/logging.h"
#include "../../../base/public/string_util.h"
#include "../../../base/public/flags.h"
#include "../../../base/public/callback_spec.h"
#include "../../../base/public/thread.h"
#include "../public/connection.h"
#include "../public/result.h"

DEFINE_string(mysql_host, "localhost", "mysql server host name");
DEFINE_string(mysql_db, "social", "test database on mysql server");
DEFINE_string(mysql_usr, "root", "mysql server login user name");
DEFINE_string(mysql_password, "root", "password to login mysql server");
DEFINE_string(mysql_socket,
              "/var/run/mysqld/mysqld.sock",
              "socket file path be used to connect mysql server");
DEFINE_int32(mysql_port, 1234, "port be used to connect mysql server");

DEFINE_int32(test_switch, 1, "");

using base::Thread;
using base::NewPermanentCallback;
using base::ParseCommandLineFlags;
using util_mysql::kMysqlIllegalValue;
using util_mysql::MysqlResult;

util_mysql::ConnectionOption option;

namespace util_mysql {

class TestConnection {
 public:
  explicit TestConnection(ConnectionOption option) {
    conn_ = new MysqlConnection(option);
    query_ = new MysqlQuery(conn_);
  }

  MysqlConnection *conn_;
  MysqlQuery *query_;
};


class TestThreadSafe : public Thread {
 public:
  TestThreadSafe(int32 test_switch, util_mysql::TestConnection* conn) {
    test_switch_ = test_switch;
    test_conn_ = conn;
  }
  void Run() {
    while (1) {
      util_mysql::MysqlResult result;
      switch (test_switch_) {
        case 0:
          test_conn_->query_->Insert("test_table", "col1", "555", false);
          LOG(INFO) << "insert";
          break;
        case 1:
          test_conn_->query_->Delete("test_table", "col4>500000");
          LOG(INFO) << "delete";
          break;
        case 2:
          test_conn_->query_->Update("test_table", "col1=20, col4=0",
                                     "col3>20");
          LOG(INFO) << "update";
          break;
        case 3:
          test_conn_->query_->Select("test_table", "*", &result);
          LOG(INFO) << "select";
          break;
        case 4:
          test_conn_->query_->Execute("delete from test_talbe");
          LOG(INFO) << "E delete";
          break;
      }
    }
  }

  util_mysql::TestConnection *test_conn_;
  int32 test_switch_;
};
}

int main(int argc, char *argv[]) {
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
                "col7 varchar(100),"
                "col8 blob,"
                "col9 TIMESTAMP,"
                "col10 int,"
                "PRIMARY KEY (id))"
                "ENGINE=InnoDB");

  for (int i = 0; i < 30; i++) {
    uint64 insert_id;
    int64 l = i;
    insert_id = query.Insert("test_table",
                 "col1, col2, col3, col4, col5, col6, col7, col8",
                 StringPrintf("-100, 100, %ld, %ld, %f, %d, \"%s\", \"%s\"",
                               static_cast<int64>(i),
                               static_cast<uint64>(l*100000),
                               static_cast<double>(2233.5048),
                               true,
                               "fewafewa",
                               "blob"),
                               false);
    VLOG(1) << insert_id;
  }

  switch (FLAGS_test_switch) {
    case 1:
      CHECK_EQ(query.Count("test_table", "col1", "col1=-100"), 30);
      break;
    case 2:
      CHECK_EQ(query.Update("test_table", "col1=20, col4=0", "col3>20"), 9);
      break;
    case 3:
      CHECK_EQ(query.Delete("test_table", "col4>500000"), 24);
      break;
    case 4: {
      std::vector<std::string> values;
      for (int i = 0; i < 30; i++) {
        std::string setence = StringPrintf("%d, %d,\"%s\"", i, i+1, "test");
        values.push_back(setence);
      }
      query.Insert("test_table", "col1, col2, col7", values, false);
      MysqlResult result;
      query.Execute("select col1, col2, col7 "
                    "from test_table "
                    "where col7=\"test\"",
                    &result);
      int cur = 0;
      CHECK_EQ(result.GetRowsNum(), 30);
      while (result.FetchRow()) {
        int32 col1;
        int32 col2;
        result.GetValue("col1", &col1);
        CHECK_EQ(col1, cur);
        result.GetValue("col2", &col2);
        CHECK_EQ(col2, cur+1);
        cur++;
      }
      break;
    }
    case 5:

      util_mysql::TestConnection conn2(option);
      util_mysql::TestThreadSafe thread1(0, &conn2);
      util_mysql::TestThreadSafe thread2(1, &conn2);
      util_mysql::TestThreadSafe thread3(2, &conn2);
      util_mysql::TestThreadSafe thread4(3, &conn2);

      thread1.Start();
      thread2.Start();
      thread3.Start();
      thread4.Start();
      while (1);
      break;
  }
  return 0;
}

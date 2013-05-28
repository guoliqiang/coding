// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "base/flags.h"
#include "util/mysql_client2/testdata/blob_test.pb.h"
#include "util/mysql_client2/bind.h"
#include "util/mysql_client2/query.h"
#include "util/mysql_client2/result.h"
#include "util/mysql_client2/connection.h"

DEFINE_string(mysql_host, "localhost", "mysql server host name");
DEFINE_string(mysql_db, "social", "test database on mysql server");
DEFINE_string(mysql_usr, "root", "mysql server login user name");
DEFINE_string(mysql_password, "root", "password to login mysql server");
DEFINE_string(mysql_socket,
              "/var/run/mysqld/mysqld.sock",
              "socket file path be used to connect mysql server");
DEFINE_int32(mysql_port, 3306, "port be used to connect mysql server");

util_mysql::ConnectionOption option;
using util_mysql::TestMysqlInfo;

int main(int argc, char **argv) {
  base::ParseCommandLineFlags(&argc, &argv, true);
  option.host_ = FLAGS_mysql_host;
  option.database_ = FLAGS_mysql_db;
  option.password_ =  FLAGS_mysql_password;
  option.user_ = FLAGS_mysql_usr;
  option.port_ = FLAGS_mysql_port;
  option.socket_ = FLAGS_mysql_socket;

  util_mysql::MysqlConnection conn(option);
  util_mysql::MysqlQuery query(&conn);
  util_mysql::MysqlResult result;

  query.Execute("DROP TABLE IF EXISTS test_table");
  query.Execute("CREATE TABLE test_table("
                "id BIGINT(20) unsigned NOT NULL AUTO_INCREMENT,"
                "col1 INT NOT NULL,"
                "col2 INT UNSIGNED NOT NULL,"
                "col3 BIGINT,"
                "col4 BIGINT UNSIGNED,"
                "col5 double,"
                "col6 bool,"
                "col7 varchar(255),"
                "col8 blob,"
                "col9 TIMESTAMP,"
                "col10 int,"
                "PRIMARY KEY (id))"
                "ENGINE=InnoDB");

  util_mysql::MysqlBind bind(&conn,
        "INSERT INTO test_table(col1, col2, col3, col4, col5, col6, col7, col8)"
        " VALUES(?,?,?,?,?,?,?,?)");

  TestMysqlInfo input_blob;
  input_blob.set_data_int64(-123456789654);
  input_blob.set_data_int32(-1234567);
  input_blob.set_data_uint32(1234567);
  input_blob.set_data_uint64(123456789654654);
  input_blob.set_data_double(6.32564);

  std::string str_input;
  input_blob.SerializeToString(&str_input);
  for (int i = 1; i < 3; i++) {
    std::string tmp = "abcdefghizkjie";
    bind.SetParam(0, i * 2);
    bind.SetParam(1, static_cast<uint32>(i * 3));
    bind.SetParam(2, static_cast<int64>(i * 10));
    bind.SetParam(3, static_cast<uint64>(i * 10));
    bind.SetParam(4, 0.3+i);
    bind.SetParam(5, true);
    bind.SetParam(6, tmp);
    bind.SetParam(7, str_input);
    uint64 num = bind.Execute();
    VLOG(1) << "affected num: " << num;
  }

  query.Execute("select col8 from test_table", &result);
  while (result.FetchRow()) {
    std::string str_output;
    result.GetValue("col8", &str_output);
    TestMysqlInfo output_blob;
    output_blob.ParseFromString(str_output);
    CHECK_EQ(output_blob.data_int64(), -123456789654);
    CHECK_EQ(output_blob.data_int32(), -1234567);
    CHECK_EQ(output_blob.data_uint32(), 1234567);
    CHECK_EQ(output_blob.data_uint64(), 123456789654654);
    CHECK_EQ(output_blob.data_double(), 6.32564);
  }
}

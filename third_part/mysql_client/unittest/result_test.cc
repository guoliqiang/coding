// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "base/logging.h"
#include "base/string_util.h"
#include "base/thrift.h"
#include "util/mysql_client2/testdata/blob_test.pb.h"
#include "util/mysql_client2/testdata/gen-cpp/blob_test_types.h"
#include "util/mysql_client2/connection.h"
#include "util/mysql_client2/query.h"
#include "util/mysql_client2/result.h"

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
using util_mysql::TestStruct;
using base::FromThriftToString;
using base::FromStringToThrift;

int main(int32 argc, char **argv) {
  base::ParseCommandLineFlags(&argc, &argv, true);
  option.host_ = FLAGS_mysql_host;
  option.database_ = FLAGS_mysql_db;
  option.password_ =  FLAGS_mysql_password;
  option.user_ = FLAGS_mysql_usr;
  option.port_ = FLAGS_mysql_port;
  option.socket_ = FLAGS_mysql_socket;

  util_mysql::MysqlConnection  conn(option);
  util_mysql::MysqlQuery query(&conn);
  util_mysql::MysqlResult result;

  query.Execute("DROP TABLE IF EXISTS test_table");
  query.Execute("CREATE TABLE test_table( "
                "id   BIGINT(20) unsigned NOT NULL AUTO_INCREMENT,"
                "col1 INT,"
                "col2 INT UNSIGNED,"
                "col3 BIGINT,"
                "col4 BIGINT UNSIGNED,"
                "col5 double,"
                "col6 bool,"
                "col7 varchar(255),"
                "col8 blob,"
                "col9 TIMESTAMP,"
                "col10 blob,"
                "PRIMARY KEY (id))"
                "ENGINE=InnoDB");

  std::string serialize_string;
  TestMysqlInfo input_data;
  std::string proto_value;
  input_data.set_data_int64(-123456789654);
  input_data.set_data_int32(-1234567);
  input_data.set_data_uint32(1234567);
  input_data.set_data_uint64(123456789654654);
  input_data.set_data_double(6.32564);
  input_data.SerializeToString(&serialize_string);
  query.FromBlobToString(serialize_string, &proto_value);

  serialize_string.clear();
  std::string thrift_value;
  TestStruct input_thrift;
  for (int i = 0; i < 10; i++) {
    input_thrift.data_int32.push_back(static_cast<int32>(i));
    input_thrift.__isset.data_int32 = true;
  }
  input_thrift.data_int64 = static_cast<int64>(100);
  input_thrift.data_bool = true;
  input_thrift.data_double = static_cast<double>(3.14);
  input_thrift.data_str = "this is a thrift";
  serialize_string = FromThriftToString(&input_thrift);
  query.FromBlobToString(serialize_string, &thrift_value);

  for (int i = 0; i < 30; i++) {
    uint64 insert_id;
    int64 l = i;
    VLOG(1) << "i : " << i;
    insert_id = query.Insert("test_table",
                 "col1, col2, col3, col4, col5, col6, col7, col8, col10",
            StringPrintf("-100, 100, %ld, %ld, %f, %d, \"%s\", \"%s\", \"%s\"",
                               9998388607,
                               l*100000,
                               2233.5048,
                               true,
                               "fewafewa",
                               proto_value.c_str(),
                               thrift_value.c_str()),
                               false);
    VLOG(1) << insert_id;
  }

  query.Select("test_table", "*", &result);
  LOG(INFO) << result.GetRowsNum();

  for (int i = 0; result.FetchRow(); i++) {
    std::string ch;
    int id;
    int32 col1;
    int64 col3;
    uint64 col4;
    double col5;
    bool col6;
    std::string col7;
    std::string col8;
    std::string col10;

    result.GetValue("col1", &col1);
    CHECK_EQ(col1, -100);
    result.GetValue("col3", &col3);
    CHECK_EQ(col3, 9998388607);
    result.GetValue("col4", &col4);
    CHECK_EQ(col4, i * 100000);
    result.GetValue("col5", &col5);
    CHECK_EQ(col5, 2233.5048);
    result.GetValue("col6", &col6);
    CHECK_EQ(col6, true);
    result.GetValue("col7", &col7);
    CHECK_EQ(col7, "fewafewa");
    result.GetValue(0, &id);
  }

  query.Select("test_table", "*", &result);
  while (result.FetchRow()) {
    std::string pro_str;
    std::string thr_str;
    TestMysqlInfo output_data;
    TestStruct out_thrift;
    result.GetValue("col8", &pro_str);
    result.GetValue("col10", &thr_str);

    output_data.ParseFromString(pro_str);
    CHECK_EQ(output_data.data_int64(), -123456789654);
    CHECK_EQ(output_data.data_int32(), -1234567);
    CHECK_EQ(output_data.data_uint32(), 1234567);
    CHECK_EQ(output_data.data_uint64(), 123456789654654);
    CHECK_EQ(output_data.data_double(), 6.32564);

    FromStringToThrift(thr_str, &out_thrift);
    for (int i = 0; i < out_thrift.data_int32.size(); i++) {
      VLOG(1) << "int32: " << out_thrift.data_int32[i];
      CHECK_EQ(out_thrift.data_int32[i], i);
    }
    CHECK_EQ(out_thrift.data_int64, 100);
    CHECK_EQ(out_thrift.data_bool, true);
    CHECK_EQ(out_thrift.data_double, 3.14);
    CHECK_EQ(out_thrift.data_str, "this is a thrift");
  }
  LOG(INFO) << "test select and get values pass";
  return 0;
}

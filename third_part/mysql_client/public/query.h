// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#ifndef UTIL_MYSQL_CLIENT2_QUERY_H_
#define UTIL_MYSQL_CLIENT2_QUERY_H_

#include <string>
#include <vector>

#include "../../../base/public/basictypes.h"

namespace util_mysql {
class MysqlResult;
class MysqlConnection;

class MysqlQuery {
 public:
  explicit MysqlQuery(MysqlConnection *conn);
  virtual ~MysqlQuery();

  // when you want to execute query with blob data,
  // you have two choices:
  // 1. after you generate string from proto or thrift, you should
  //    use this method to convert to an ordinary string, then you
  //    can use it.
  // 2. use parameter bind (see bind_test.cc for more examples)
  // 3. FromBlobToString == EscapeString
  bool FromBlobToString(const std::string &blob, std::string *ouput);
  bool EscapeString(const std::string &input, std::string *ouput);

  // 向数据库插入一条数据
  // 参数 ‘replace’ 为 true 时，如果要插入的记录在数据库中存在，则替换
  // 函数返回此次插入新生成的id值
  // 出错则返回 kMysqlIllegalValue
  uint64 Insert(const std::string &table_name,
                const std::string &columns,
                const std::string &values,
                bool replace = false);  // whether replace if exist

  uint64 Insert(const std::string &table_name,
                const std::string &columns,
                const std::vector<std::string> &values,
                bool replace = false);  // whether replace if exist
  // 更新记录
  // 函数返回更新操作所影响的行数
  // 出错则返回 kMysqlIllegalValue
  uint64 Update(const std::string &table_name,
                const std::string &expression,
                const std::string &conditions);

  uint64 Update(const std::string &table_name, const std::string &expression);

  // 删除表中记录
  // 函数返回删除的行数, 出错返回 kMysqlIllegalValue
  uint64 Delete(const std::string &table_name, const std::string &conditions);

  uint64 Delete(const std::string &table_name);

  // 统计表中满足一定条件的记录
  // 函数统计结果
  // 出错则返回 kMysqlIllegalValue
  uint64 Count(const std::string &table_name,
               const std::string &columns,
               const std::string &conditions);
  uint64 Count(const std::string &table_name,
               const std::string &columns);

  bool Select(const std::string &table_name,
              const std::string &columns,
              const std::string &conditions,
              MysqlResult *result);

  bool Select(const std::string &table_name,
              const std::string &columns,
              MysqlResult *result);

  bool Execute(const std::string &query, MysqlResult *result);

  // 不需要返回结果的操作
  // 返回此次操作影响的行数
  uint64 Execute(const std::string &query);

 private:
  void AddConditions(const std::string &conditions, std::string *query_str);
  bool ExecuteInternal(const std::string &query, MysqlResult *result);

  // 不需要返回结果的操作
  // 返回此次操作影响的行数
  uint64 ExecuteInternal(const std::string &query);

  MysqlConnection *conn_;
  DISALLOW_COPY_AND_ASSIGN(MysqlQuery);
};
}  // namespace util_mysql

#endif  // UTIL_MYSQL_CLIENT2_QUERY_H_

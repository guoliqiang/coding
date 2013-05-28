// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#ifndef UTIL_MYSQL_CLIENT2_RESULT_H_
#define UTIL_MYSQL_CLIENT2_RESULT_H_

#include <map>
#include <string>

#include "../../../base/public/basictypes.h"
#include "../../mysql/include/mysql.h"

namespace util_mysql {
class MysqlQuery;

class MysqlResult {
 public:
  MysqlResult();
  ~MysqlResult();
  void Reset(MYSQL_RES *res);

  // 获取结果集中的下一行, 如果没有则返回false
  bool FetchRow();

  // 返回结果集中的行数目
  // 对于一次只取一行的查询方式，需要等到检索完所有的值之后才可调用
  uint64 GetRowsNum() const;

  // 返回结果集中的列数目
  uint64 GetFieldsNum() const;

  // 返回结果集中指定列的信息
  void GetFieldName(int32 column_index, std::string *name);
  uint64 GetFieldWidth(int32 column_index);

  // 取得当前行某一列的值
  bool GetValue(int32 column_index, std::string *result);
  bool GetValue(int32 column_index, int32 *result);
  bool GetValue(int32 column_index, int64 *result);
  bool GetValue(int32 column_index, uint64 *result);
  bool GetValue(int32 column_index, double  *result);
  bool GetValue(int32 column_index, bool *result);

  bool GetValue(const std::string &column_name, std::string *result);
  bool GetValue(const std::string &column_name, int32 *result);
  bool GetValue(const std::string &column_name, int64 *result);
  bool GetValue(const std::string &column_name, uint64 *result);
  bool GetValue(const std::string &column_name, double *result);
  bool GetValue(const std::string &column_name, bool *result);
  bool GetColumnIndex(const std::string &column_name, int32 *index);

 private:

  void Clean();
  // 用于域名到索引之间的转换
  std::map<std::string, int32> name_index_map_;
  MYSQL_RES *res_;
  MYSQL_FIELD *fields_;
  // 调用FetchRow 所指向的行
  MYSQL_ROW current_row_;
  DISALLOW_COPY_AND_ASSIGN(MysqlResult);
};
}  // namespace util_mysql

#endif  // UTIL_MYSQL_CLIENT2_RESULT_H_

// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "../public/result.h"

#include "../../../base/public/string_util.h"
#include "../../../base/public/logging.h"

namespace util_mysql {

//
MysqlResult::MysqlResult() : res_(NULL), fields_(NULL) {
}

//
void MysqlResult::Reset(MYSQL_RES *res) {
  Clean();

  CHECK(res);
  res_ = res;
  name_index_map_.clear();

  uint32 num_fields;

  num_fields = mysql_num_fields(res);
  fields_ = mysql_fetch_fields(res);
  for (uint32 i = 0; i < num_fields; i++) {
    name_index_map_[fields_[i].name] = i;
  }
}

//
MysqlResult::~MysqlResult() {
  Clean();
}

// 获取结果集中的下一行, 如果没有则返回false
bool MysqlResult::FetchRow() {
  current_row_ = mysql_fetch_row(res_);
  if (current_row_ == NULL)
    return false;
  return true;
}

//
uint64 MysqlResult::GetRowsNum() const {
  return mysql_num_rows(res_);
}

// 返回结果集中的列数目
uint64 MysqlResult::GetFieldsNum() const {
  return mysql_num_fields(res_);
}

// 返回结果集中指定列的信息
void MysqlResult::GetFieldName(int32 column_index, std::string *name) {
  if (column_index >= GetFieldsNum()) {
    LOG(ERROR) << "column index cross border !";
    return;
  }
  name->assign(fields_[column_index].name);
}

// TODO (guoliqiang) add explanation for this function
uint64 MysqlResult::GetFieldWidth(int32 column_index) {
  if (column_index >= GetFieldsNum()) {
    LOG(ERROR) << "column index cross border !";
    return 0;
  }
  uint64 *lengths = mysql_fetch_lengths(res_);
  return lengths[column_index];
}

// 取得当前行某一列的值
bool MysqlResult::GetValue(int32 column_index, std::string *result) {
  if (column_index >= GetFieldsNum()) {
    LOG(ERROR) << "column index cross border !";
    return false;
  }
  result->assign(current_row_[column_index], GetFieldWidth(column_index));
  return true;
}

//
bool MysqlResult::GetValue(int32 column_index, int32 *result) {
  std::string tmp_value;
  if (!GetValue(column_index, &tmp_value))
    return false;
  StringToInt(tmp_value, result);
  return true;
}

//
bool MysqlResult::GetValue(int32 column_index, int64 *result) {
  std::string tmp_value;
  if (!GetValue(column_index, &tmp_value))
    return false;
  StringToInt64(tmp_value, result);
  return true;
}

//
bool MysqlResult::GetValue(int32 column_index, uint64 *result) {
  std::string tmp_value;
  if (!GetValue(column_index, &tmp_value))
    return false;
  StringToUint64(tmp_value, result);
  return true;
}

//
bool MysqlResult::GetValue(int32 column_index, double  *result) {
  std::string tmp_value;
  if (!GetValue(column_index, &tmp_value))
    return false;
  StringToDouble(tmp_value, result);
  return true;
}

//
bool MysqlResult::GetValue(int32 column_index, bool *result) {
  std::string tmp_value;
  if (!GetValue(column_index, &tmp_value))
    return false;
  int tmp_result = 0;
  StringToInt(tmp_value, &tmp_result);
  if (tmp_result != 0)
    *result = true;
  else
    *result = false;
  return true;
}

//
bool MysqlResult::GetColumnIndex(const std::string &column_name, int32 *index) {
  if (name_index_map_.find(column_name) == name_index_map_.end()) {
    LOG(ERROR) << "column : " << column_name << " does not exist !";
    return false;
  }
  *index = name_index_map_[column_name];
  return true;
}

//
bool MysqlResult::GetValue(const std::string &column_name,
                           std::string *result) {
  int32 index;
  if (GetColumnIndex(column_name, &index))
    return GetValue(index, result);
  else
    return false;
}

//
bool MysqlResult::GetValue(const std::string &column_name, int32 *result) {
  int32 index;
  if (GetColumnIndex(column_name, &index))
    return GetValue(index, result);
  else
    return false;
}

//
bool MysqlResult::GetValue(const std::string &column_name, int64 *result) {
  int32 index;
  if (GetColumnIndex(column_name, &index))
    return GetValue(index, result);
  else
    return false;
}

//
bool MysqlResult::GetValue(const std::string &column_name, uint64 *result) {
  int32 index;
  if (GetColumnIndex(column_name, &index))
    return GetValue(index, result);
  else
    return false;
}

//
bool MysqlResult::GetValue(const std::string &column_name, double *result) {
  int32 index;
  if (GetColumnIndex(column_name, &index))
    return GetValue(index, result);
  else
    return false;
}

//
bool MysqlResult::GetValue(const std::string &column_name, bool *result) {
  int32 index;
  if (GetColumnIndex(column_name, &index))
    return GetValue(index, result);
  else
    return false;
}

// 释放此次结果集,在接受新的结果之前一定要释放结果集
void MysqlResult::Clean() {
  if (res_) {
    mysql_free_result(res_);
    res_ = NULL;
    fields_ = NULL;
  }
}
}  // namespace util_mysql

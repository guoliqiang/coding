// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "../public/query.h"

#include "../../../base/public/string_util.h"
#include "../../../base/public/logging.h"

#include "../public/result.h"
#include "../public/connection.h"

// NOTE: In order to guarantee thread safe, this class may
//       be to strong to effect the efficiency.
//
//       MysqlConnection class is not thread safe, but MysqlQuery class is
//       thread safe.
// TODO(guoliqiang): reference the mysql API doc for thread safe.

using base::MutexLock;

namespace util_mysql {
//
MysqlQuery::MysqlQuery(MysqlConnection *conn) : conn_(conn) {
  CHECK(conn_);
}
//
MysqlQuery::~MysqlQuery() {}
//
bool MysqlQuery::FromBlobToString(const std::string &blob,
                                  std::string *output) {
  MutexLock lock(conn_->GetMutex());
  return conn_->GetEscapeString(blob, output);
}
//
bool MysqlQuery::EscapeString(const std::string &input,
                              std::string *output) {
  MutexLock lock(conn_->GetMutex());
  return conn_->GetEscapeString(input, output);
}
//
void MysqlQuery::AddConditions(const std::string &conditions,
                               std::string *query_str) {
  if (!conditions.empty())
    query_str->append(StringPrintf(" WHERE %s", conditions.c_str()));
}
//
uint64 MysqlQuery::Insert(const std::string &table_name,
                          const std::string &columns,
                          const std::string &values,
                          bool replace) {
  std::string action;
  if (replace)
    action.assign("REPLACE");
  else
    action.assign("INSERT");
  std::string query = StringPrintf("%s INTO %s(%s) VALUES(%s)",
                                    action.c_str(),
                                    table_name.c_str(),
                                    columns.c_str(),
                                    values.c_str());
  MutexLock lock(conn_->GetMutex());
  if (ExecuteInternal(query) != kMysqlIllegalValue) {
    return conn_->GetInsertId();
  }
  return kMysqlIllegalValue;
}
//
uint64 MysqlQuery::Insert(const std::string &table_name,
                          const std::string &columns,
                          const std::vector<std::string> &values,
                          bool replace) {
  std::string action;
  if (replace)
    action.assign("REPLACE");
  else
    action.assign("INSERT");

  std::string values_buf = "";
  for (int i = 0; i < values.size(); i++) {
    values_buf += "(" + values[i] + ")";
    if (i != (values.size() - 1)) {
      values_buf += ",";
    }
  }
  const std::string sentence = StringPrintf("%s %s(%s) values %s",
                                        action.c_str(),
                                        table_name.c_str(),
                                        columns.c_str(),
                                        values_buf.c_str());
  MutexLock lock(conn_->GetMutex());
  if (ExecuteInternal(sentence) != kMysqlIllegalValue) {
    return conn_->GetInsertId();
  }
  return kMysqlIllegalValue;
}
//
uint64 MysqlQuery::Update(const std::string &table_name,
                          const std::string &expression,
                          const std::string &conditions) {
  std::string query = StringPrintf("UPDATE %s SET %s",
                                   table_name.c_str(),
                                   expression.c_str());
  AddConditions(conditions, &query);
  MutexLock lock(conn_->GetMutex());
  return ExecuteInternal(query);
}
//
uint64 MysqlQuery::Update(const std::string &table_name,
                          const std::string &expression) {
  return Update(table_name, expression, "");
}
//
uint64 MysqlQuery::Delete(const std::string &table_name,
                          const std::string &conditions) {
  std::string query = StringPrintf("DELETE FROM %s",
                                  table_name.c_str());
  AddConditions(conditions, &query);
  MutexLock lock(conn_->GetMutex());
  return ExecuteInternal(query);
}
//
uint64 MysqlQuery::Delete(const std::string &table_name) {
  return Delete(table_name, "");
}
//
uint64 MysqlQuery::Count(const std::string &table_name,
                         const std::string &columns,
                         const std::string &conditions) {
  MysqlResult result;
  std::string query = StringPrintf("SELECT COUNT(%s) FROM %s",
                                    columns.c_str(),
                                    table_name.c_str());
  AddConditions(conditions, &query);
  MutexLock lock(conn_->GetMutex());
  if (!ExecuteInternal(query, &result)) {
    return kMysqlIllegalValue;
  }
  int64 count = 0;
  result.FetchRow();
  result.GetValue(0, &count);
  return count;
}
//
uint64 MysqlQuery::Count(const std::string &table_name,
                         const std::string &columns) {
  return Count(table_name, columns, "");
}
//
bool MysqlQuery::Select(const std::string &table_name,
                        const std::string &columns,
                        const std::string &conditions,
                        MysqlResult *result) {
  if (result == NULL) {
    LOG(ERROR) << "Please make the result available !";
    return false;
  }
  std::string query = StringPrintf("SELECT %s FROM %s",
                                    columns.c_str(),
                                    table_name.c_str());
  AddConditions(conditions, &query);
  MutexLock lock(conn_->GetMutex());
  return ExecuteInternal(query, result);
}
//
bool MysqlQuery::Select(const std::string &table_name,
                        const std::string &columns,
                        MysqlResult *result) {
  return Select(table_name, columns, "", result);
}
//
bool MysqlQuery::ExecuteInternal(const std::string &query,
                                 MysqlResult *result) {
  VLOG(3) << query;
  if (conn_->RealQuery(query, query.size()) != 0) {
    LOG(ERROR) << conn_->GetErrorInfo();
    return false;
  }

  MYSQL_RES *ret_result = conn_->GetStoreResult();
  if (ret_result)
    result->Reset(ret_result);
  return true;
}
//
uint64 MysqlQuery::ExecuteInternal(const std::string &query) {
  VLOG(3) << query;
  if (conn_->RealQuery(query, query.size()) != 0) {
    LOG(ERROR) << conn_->GetErrorInfo();
    return kMysqlIllegalValue;
  }
  return conn_->GetAffectedRowNum();
}
//
bool MysqlQuery::Execute(const std::string &query,
                         MysqlResult *result) {
  MutexLock lock(conn_->GetMutex());
  return ExecuteInternal(query, result);
}
//
uint64 MysqlQuery::Execute(const std::string &query) {
  MutexLock lock(conn_->GetMutex());
  return ExecuteInternal(query);
}
}  // namespace util_mysql


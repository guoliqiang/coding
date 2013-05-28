// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "../public/bind.h"
#include "../../../base/public/logging.h"

namespace util_mysql {

class MysqlConnection;

MysqlBind::MysqlBind(MysqlConnection *conn, const std::string &query)
                    : conn_(conn) {
  query_statment_ = query;
  ParamInit();
}

void MysqlBind::ParamInit() {
  stmt_ = conn_->GetStmtHandle();
  if (NULL == stmt_)
    LOG(ERROR) << mysql_stmt_error(stmt_);
  if (0 != mysql_stmt_prepare(stmt_,
                              query_statment_.c_str(),
                              query_statment_.length())) {
    LOG(ERROR) << mysql_stmt_error(stmt_);
  }

  if (!bind_.get()) {
    param_num_ = mysql_stmt_param_count(stmt_);
    bind_.reset(new MYSQL_BIND[param_num_]);
    memset(bind_.get(), 0, sizeof(MYSQL_BIND) * param_num_);
    for (int i = 0; i < param_num_; i++) {
      bind_[i].buffer_type = MYSQL_TYPE_NULL;
    }
  }
}

MysqlBind::~MysqlBind() {
  for (int i = 0; i < param_num_; i++) {
    if (bind_[i].buffer) {
      delete[] static_cast<char *>(bind_[i].buffer);
    }
    if (bind_[i].length) {
      delete bind_[i].length;
    }
  }
  Close();
}

bool MysqlBind::FillParamInfo(int32 column_index,
                              const void *data,
                              size_t size,
                              enum enum_field_types field_type,
                              bool is_unsigned) {
  if (column_index >= param_num_ || column_index < 0) {
    LOG(ERROR) << "column index error.";
    return false;
  }
  if (bind_[column_index].buffer) {
    delete[] static_cast<char *>(bind_[column_index].buffer);
    bind_[column_index].buffer = NULL;
  }

  if (bind_[column_index].length) {
    delete bind_[column_index].length;
  }

  memset(&bind_[column_index], 0, sizeof(MYSQL_BIND));

  bind_[column_index].buffer_type = field_type;

  if (field_type != MYSQL_TYPE_STRING) {
    bind_[column_index].buffer_length = 0;
    bind_[column_index].length = NULL;
  } else {
    bind_[column_index].buffer_length = size;
    bind_[column_index].length = new uint64(size);
  }

  bind_[column_index].buffer = new char[size];
  bind_[column_index].is_unsigned = is_unsigned;
  bind_[column_index].is_null_value = 0;
  memcpy(bind_[column_index].buffer, data, size);
  return true;
}

// 设定参数
bool MysqlBind::SetParam(int32 column_index, const std::string &data) {
  return FillParamInfo(column_index, data.c_str(), data.size(),
                       MYSQL_TYPE_STRING, false);
}

bool MysqlBind::SetParam(int32 column_index, uint32 data) {
  return FillParamInfo(column_index, &data, 4, MYSQL_TYPE_LONG, true);
}

bool MysqlBind::SetParam(int32 column_index, int32 data) {
  return FillParamInfo(column_index, &data, 4, MYSQL_TYPE_LONG, false);
}

bool MysqlBind::SetParam(int32 column_index, uint64 data) {
  return FillParamInfo(column_index, &data, 8, MYSQL_TYPE_LONGLONG, true);
}

bool MysqlBind::SetParam(int32 column_index, int64 data) {
  return FillParamInfo(column_index, &data, 8, MYSQL_TYPE_LONGLONG, false);
}

bool MysqlBind::SetParam(int32 column_index, double data) {
  return FillParamInfo(column_index, &data, 8, MYSQL_TYPE_DOUBLE, false);
}

bool MysqlBind::SetParam(int32 column_index, bool data) {
  return FillParamInfo(column_index, &data, 1, MYSQL_TYPE_TINY, false);
}

// 返回操作影响的行数
// 出错返回 kMysqlIllegalValue
uint64 MysqlBind::Execute() {
  if (!conn_->IsAlive()) {
    conn_->Reconnect();
    ParamInit();
  }
  if (mysql_stmt_bind_param(stmt_, bind_.get())) {
    LOG(ERROR) << mysql_stmt_error(stmt_);
    return kMysqlIllegalValue;
  }
  if (mysql_stmt_execute(stmt_)) {
    LOG(ERROR) << mysql_stmt_error(stmt_);
    return kMysqlIllegalValue;
  }
  return mysql_stmt_affected_rows(stmt_);
}

// 释放stmt_ , 并且置空, 在析构函数中要调用
void MysqlBind::Close() {
  if (stmt_) {
    mysql_stmt_close(stmt_);
    stmt_ = NULL;
  }
}
}  // namespace util_mysql

// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "../public/connection.h"

#include "../../../base/public/logging.h"
#include "../../../base/public/flags.h"
#include "../../../base/public/time.h"
#include "../../../base/public/string_util.h"
#include "../../../base/public/scoped_ptr.h"

DEFINE_int32(mysql_timeout, 24, "mysql 关闭交互的等待间隔时间(小时)");

using base::Time;

namespace util_mysql {
//
MysqlConnection::MysqlConnection(const ConnectionOption &option)
                                 : option_(option),
                                   mysql_(NULL),
                                   ping_interval_(10),
                                   ping_timestamp_(0) {
  // connect here is to make this Constructor compatible with the old version,
  // and here ignores the connect result if fail, you need to call Connect()
  // Explicitly and check the connect result when establish a real connection.
  // it will be removed later.
  if (!Connect()) {
    LOG(ERROR) << "Failed to connect mysql server!";
    LOG(ERROR) << "> Host: "      << option_.host_;
    LOG(ERROR) << "> Port: "      << option_.port_;
    LOG(ERROR) << "> User Name: " << option_.user_;
    LOG(ERROR) << "> Database: "  << option_.database_;
    LOG(ERROR) << "> Socket: "    << option_.socket_;
  }
}
//
bool MysqlConnection::Connect() {
  if (!IsAlive())
    return ConnectInternal();
  return true;
}
//
bool MysqlConnection::ConnectInternal() {
  mysql_ = mysql_init(NULL);
  if (!mysql_)
    return false;
  if (!mysql_real_connect(mysql_,
                          option_.host_.c_str(),
                          option_.user_.c_str(),
                          option_.password_.c_str(),
                          option_.database_.c_str(),
                          option_.port_,
                          option_.socket_.empty() ?
                              NULL : option_.socket_.c_str(),
                          0)) {
    LOG(ERROR) << "Failed to connect mysql server!";
    LOG(ERROR) << "> Host: "      << option_.host_;
    LOG(ERROR) << "> Port: "      << option_.port_;
    LOG(ERROR) << "> User Name: " << option_.user_;
    LOG(ERROR) << "> Database: "  << option_.database_;
    LOG(ERROR) << "> Socket: "    << option_.socket_;
    return false;
  }

  if (mysql_query(mysql_, "SET NAMES utf8") != 0) return false;

  std::string time_out = StringPrintf("set interactive_timeout=%d * 3600",
                                      FLAGS_mysql_timeout);
  if (mysql_query(mysql_, time_out.c_str()) != 0) return false;

  return true;
}
//
void MysqlConnection::Disconnect() {
  if (IsAlive()) {
    mysql_close(mysql_);
  }
  mysql_ = NULL;
}
//
bool MysqlConnection::Reconnect() {
  Disconnect();
  return Connect();
}
//
bool MysqlConnection::IsAlive() {
  if (mysql_ == NULL)
    return false;
  if (0 != mysql_ping(mysql_)) {
    LOG(ERROR) << mysql_error(mysql_);
    return false;
  }
  return true;
}
//
void MysqlConnection::SetPingInterval(int32 interval) {
  ping_interval_ = interval;
}
//
void MysqlConnection::KeepAlive() {
  Time time = Time::Now();
  if (time.ToTimeVal().tv_sec - ping_timestamp_ >= ping_interval_) {
    ping_timestamp_ = time.ToTimeVal().tv_sec;
    if (!IsAlive()) Connect();
  }
}
//
MysqlConnection::~MysqlConnection() {
  Disconnect();
}
//
uint64 MysqlConnection::GetAffectedRowNum() {
  return mysql_affected_rows(mysql_);
}
//
uint64 MysqlConnection::GetInsertId() {
  return mysql_insert_id(mysql_);
}
//
MYSQL_STMT * MysqlConnection::GetStmtHandle() {
  KeepAlive();
  return mysql_stmt_init(mysql_);
}
//
uint64 MysqlConnection::RealQuery(const std::string &query, int32 length) {
  KeepAlive();
  return mysql_real_query(mysql_, query.c_str(), length);
}
//
const char * MysqlConnection::GetErrorInfo() {
  return mysql_error(mysql_);
}
//
MYSQL_RES * MysqlConnection::GetStoreResult() {
  return mysql_store_result(mysql_);
}
//
MYSQL_RES * MysqlConnection::GetUseResult() {
  return mysql_use_result(mysql_);
}
//
base::Mutex* MysqlConnection::GetMutex() {
  return &mutex_;
}
//
uint64 MysqlConnection::GetEscapeString(const std::string &in,
                                        std::string *output) {
  scoped_ptr<char> to;
  to.reset(new char[in.size() * 2 + 1]);
  uint64 ret = mysql_real_escape_string(mysql_,
                                        to.get(),
                                        in.c_str(),
                                        in.size());
  output->assign(to.get(), ret);
  return ret;
}

}  // namespace util_mysql

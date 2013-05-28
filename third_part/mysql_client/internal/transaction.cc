// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#include "../public/transaction.h"

#include <string>

#include "../../../base/public/string_util.h"
#include "../public/query.h"


namespace util_mysql {

class MysqlQuery;

MysqlTransaction::MysqlTransaction(MysqlConnection *conn,
                                   IsolationLevel level)
                                   : conn_(conn), finish_(false) {
  MysqlQuery query(conn);
  std::string isolation_level;
  switch (level) {
    case READUNCOMMITTED :
      isolation_level = "READ UNCOMMITTED";
      break;
    case READCOMMITTED :
      isolation_level = "READ COMMITTED";
      break;
    case REPEATABLEREAD :
      isolation_level = "REPEATABLE READ";
      break;
    case SERIALIZABLE :
      isolation_level = "SERIALIZABLE";
      break;
    default:
      isolation_level = "SERIALIZABLE";
  }

  query.Execute(StringPrintf("SET SESSION TRANSACTION ISOLATION LEVEL %s",
                             isolation_level.c_str()));
  query.Execute("START TRANSACTION");
}

MysqlTransaction::~MysqlTransaction() {
  if (!finish_)
    Rollback();
}

bool MysqlTransaction::Commit() {
  MysqlQuery query(conn_);
  if (query.Execute("COMMIT") == kMysqlIllegalValue)
    return false;
  if (query.Execute("SET AUTOCOMMIT = 1") == kMysqlIllegalValue)
    return false;
  finish_ = true;
  return true;
}

bool MysqlTransaction::Rollback() {
  MysqlQuery query(conn_);
  if (query.Execute("ROLLBACK") == kMysqlIllegalValue)
    return false;
  if (query.Execute("SET AUTOCOMMIT = 1") == kMysqlIllegalValue)
    return false;
  finish_ = true;
  return true;
}
}  // namespace util_mysql

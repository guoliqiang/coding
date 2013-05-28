// Copyrights 2011 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Xu Jian)

#ifndef UTIL_MYSQL_CLIENT2_TRANSACTION_H_
#define UTIL_MYSQL_CLIENT2_TRANSACTION_H_

#include "connection.h"

namespace util_mysql {

// Note: 使用前请确认 mysql 的搜索引擎支持事务管理
class MysqlTransaction {
 public:
  enum IsolationLevel {
    READUNCOMMITTED = 0,  // allow "dirty reads" from other transactions.
    READCOMMITTED = 1,    // only read rows committed by other transactions.
    REPEATABLEREAD = 2,   // other transactions do not affect repeated reads
                          // in this transaction.
    SERIALIZABLE = 3,     // this transaction prevents writes to any rows it
                          // accesses while it runs.
  };

  MysqlTransaction(MysqlConnection *conn,
                   IsolationLevel level = SERIALIZABLE);

  ~MysqlTransaction();

  bool Commit();
  bool Rollback();

 private:
  MysqlConnection *conn_;
  bool finish_;
  DISALLOW_COPY_AND_ASSIGN(MysqlTransaction);
};
}  // namespace util_mysql

#endif  // UTIL_MYSQL_CLIENT2_TRANSACTION_H_

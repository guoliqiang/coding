#! /usr/bin/env python

import MySQLdb
import os
import sys
import mysql_option

MysqlHost=sys.argv[1]
MysqlUser=sys.argv[2]
MysqlDatabase=sys.argv[3]
MysqlPassword=sys.argv[4]
MysqlPort=int(sys.argv[5])
MysqlSocket=sys.argv[6]

Param_format="--mysql_host=%s \
              --mysql_usr=%s  \
              --mysql_password=%s \
              --mysql_socket=%s  \
              --mysql_port=%d"
Param = Param_format % (MysqlHost, MysqlUser, MysqlPassword, MysqlSocket, MysqlPort)

command = "HEAPCHECK=normal " + mysql_option.binary_dir + "transaction_test " + Param + " --mysql_db="+MysqlDatabase
print "Testing : ", command

conn = MySQLdb.connect(MysqlHost,
                       MysqlUser,
                       MysqlPassword,
                       db=MysqlDatabase)
cursor = conn.cursor()

# test commit
os.system(command + " --tran_commit=1")
cursor.execute("select * from test_table");
cursor.scroll(0, mode='absolute')
results = cursor.fetchall()
conn.commit()

i = 1
for r in results:
  if i == 6:
    break
  assert (r[1] == i*100)
  i = i + 1
mysql_option.DBG_INFO("OK", "'commit' OK.")

# test rollback
os.system(command + " --tran_commit=0")
cursor.execute("select * from test_table");
results = cursor.fetchall()
conn.commit()

assert (len(results) == 0)

mysql_option.DBG_INFO("OK", "'rollback' OK.")
mysql_option.DBG_INFO("OK", "Test transaction Pass !!!")
conn.close()


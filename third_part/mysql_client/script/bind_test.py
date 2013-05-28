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

command = "HEAPCHECK=normal " + mysql_option.binary_dir + "bind_test " + Param + " --mysql_db="+MysqlDatabase
print "Testing : ", command
os.system(command)

conn = MySQLdb.connect(MysqlHost,
                       MysqlUser,
                       MysqlPassword,
                       db=MysqlDatabase)
cursor = conn.cursor()
cursor.execute("select * from test_table");
cursor.scroll(0,mode='absolute')
results = cursor.fetchall()
conn.commit()

i = 1
for r in results:
  if i == 3:
    break
  assert (r[1] == i*2)
  assert (r[2] == i*3)
  assert (r[3] == i*10)
  assert (r[4] == i*10)
  assert (r[5] == 0.3+i)
  assert (r[6] == 1)
  assert (r[7] == 'abcdefghizkjie')
  i = i + 1
conn.close()

mysql_option.DBG_INFO("OK", "Test 'BIND' Pass !!!")

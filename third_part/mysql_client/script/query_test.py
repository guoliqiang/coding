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

command = "HEAPCHECK=normal " + mysql_option.binary_dir + "query_test " + Param+" --mysql_db="+MysqlDatabase
print "Testing : ", command
os.system(command)

conn = MySQLdb.connect(MysqlHost,
                       MysqlUser,
                       MysqlPassword,
                       db=MysqlDatabase)
cursor = conn.cursor()

# test insert and count
os.system(command + " --test_switch=1")
cursor.execute("select * from test_table");
cursor.scroll(0, mode='absolute')
results = cursor.fetchall()
conn.commit()

i = 0
for r in results:
  if i == 30:
    break
  assert (r[1] == -100)
  assert (r[2] == 100)
  assert (r[3] == i)
  assert (r[4] == i*100000)
  assert (r[5] == 2233.5048)
  assert (r[6] == 1)
  assert (r[7] == "fewafewa")
  assert (r[8] == "blob")
  i = i + 1
mysql_option.DBG_INFO("OK", "'INSERT' OK")
mysql_option.DBG_INFO("OK", "'COUNT' OK")

# test update
os.system(command + " --test_switch=2")
cursor = conn.cursor()
cursor.execute("select * from test_table");
cursor.scroll(0, mode='absolute')
results = cursor.fetchall()
conn.commit()

for r in results:
  if r[3]>20:
    assert (r[1] == 20)
    assert (r[4] == 0)
mysql_option.DBG_INFO("OK","'UPDATE' OK")

# test delete
os.system(command + " --test_switch=3")
cursor.execute("select * from test_table");
cursor.scroll(0, mode='absolute')
results = cursor.fetchall()
conn.commit()
assert (len(results) == 6)
mysql_option.DBG_INFO("OK","'DELETE' OK")
mysql_option.DBG_INFO("OK", "Test query Pass !!!")
conn.close()


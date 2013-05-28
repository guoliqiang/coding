#! /bin/bash

HOST=localhost
DATABASE=mysql_test
USER=root
PASSWORD=root
PORT=3306
SOCKET="/var/run/mysqld/mysqld.sock"

MYSQL_BASE="mysql -u$USER -p$PASSWORD -h$HOST"
MYSQL_CLIENT="$MYSQL_BASE $DATABASE"

MODE=dbg
DIR=".ymake-out/$MODE/util/mysql_client2"

$MYSQL_BASE -e"DROP DATABASE IF EXISTS $DATABASE;" || exit -1
$MYSQL_BASE -e"create database $DATABASE;" || exit -1

echo 'drop database: '$DATABASE' if EXISTS'
echo 'create a new database named : '$DATABASE

PREFIX=$(cd "$(dirname "$0")"; pwd)

python $PREFIX/query_test.py $HOST $USER $DATABASE $PASSWORD $PORT $SOCKET    || exit -1
python $PREFIX/transaction_test.py  $HOST $USER $DATABASE $PASSWORD $PORT $SOCKET || exit -1
python $PREFIX/bind_test.py $HOST $USER $DATABASE $PASSWORD $PORT $SOCKET   || exit -1

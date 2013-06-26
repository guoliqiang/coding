#!/bin/bash
# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# I just want to GH to hss~
# Date  : 2013-05-26 01:21:12
# File  : thrift.sh
# Brief :
dir=`pwd`

prefix=`pwd | awk -F "/coding/" '{print $1}'`
thrift=$prefix/coding/third_part/thrift/bin/thrift

if [ $#==1 ]; then
  `$thrift --gen cpp -o $dir $1`
elif [ $#==2 ]; then
  `$thrift --gen cpp -o $2 $1`
else
  `$thrift --gen $1 -o $3 $2`
fi

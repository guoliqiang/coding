#!/bin/bash
# Copyright 2014 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang2006@gmail.com)
# Date  : 2014-07-21 04:30:42
# File  : test.sh
# Brief :
cat file_name | grep "XXX" | grep -v "YYY" | awk -F "ZZZ" '{print $0}'| awk -F "KKK" 'BEGIN{t=0}{t+=$1}END{print t" "NR" "t/NR}'

#!/bin/bash
# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# I just want to GH to hss~
# Date  : 2013-07-17 21:01:01
# File  : run.sh
# Brief : 
hadoop fs -rmr /usr/local/hadoop/hdfs/output
# hadoop pipes -conf ./word.xml -input /usr/local/hadoop/hdfs/input/hello.txt -output /usr/local/hadoop/hdfs/output/
hadoop pipes -D hadoop.pipes.java.recordreader=true -D hadoop.pipes.java.recordwriter=true -program /usr/local/hadoop/hdfs/bin/word_count -input /usr/local/hadoop/hdfs/input/hello.txt -output /usr/local/hadoop/hdfs/output/

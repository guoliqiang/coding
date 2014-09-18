#!/usr/bin/env python
#encoding=utf-8
#
# Copyright 2014 Liqiang Guo. All Rights Reserved.
# Author: Liqiang Guo (guoliqiang2006@gmail.com)
# I just want to GH to hss~
# Date  : 2014-09-17 23:53:08
# File  : base.py
# Brief : 

import sys
import os
import time
import logging
import logging.handlers

def InitSysPath():
  sys.path.append("../third_part/gevent-1.0.1-py2.6-linux-x86_64.egg")
  sys.path.append("../third_part/greenlet-0.4.4-py2.6-linux-x86_64.egg")
  sys.path.append("../third_part/protobuf-2.6.0-py2.6.egg")

def LocalTime():
  """return local time"""
  return "%s" % time.strftime('%Y%m%d%H',time.localtime(time.time()))

def InitLog():
  logname = 'log/log.' + LocalTime()
  handler = logging.FileHandler(logname)
  formatter = logging.Formatter('[%(process)d/%(thread)d %(filename)s:%(lineno)d]%(message)s')
  handler.setFormatter(formatter)
  logging.getLogger().addHandler(handler)
  logging.getLogger().setLevel(logging.DEBUG)

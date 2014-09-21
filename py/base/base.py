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
import inspect
import getopt

def InitSysPath():
  sys.path.append("../third_part/gevent-1.0.1-py2.6-linux-x86_64.egg")
  sys.path.append("../third_part/greenlet-0.4.4-py2.6-linux-x86_64.egg")
  sys.path.append("../third_part/protobuf-2.6.0-py2.6.egg")

def LocalTime():
  """return local time"""
  return "%s" % time.strftime('%Y%m%d%H',time.localtime(time.time()))

def MkDir(dir_path):
  """helper method to create the dir if it does not exists."""
  if os.path.exists(dir_path):
    assert os.path.isdir(dir_path), '%s should be a directory!' % dir_path
  else:
    os.spawnlp(os.P_WAIT, 'mkdir', 'mkdir', '-p', dir_path)

def ModeMatch(path, mode):
  """the mod of path is equal with mode, note mode is decimal, eg 511 == 777"""
  filemode = stat.S_IMODE(os.stat(path).st_mode)
  print filemode
  return filemode == mode

def MatchOnePattern(file_path, pattern, group_num = 1):
  result = []
  pattern_re = re.compile(pattern)
  for line in open(str(file_path), 'r'):
    m = pattern_re.match(line)
    if m:
      result.append(m.group(group_num))
  return result

def Highlight(string, status, bold):
  """highligting"""
  if not sys.stdout.isatty() or os.environ.get('TERM') == 'dumb':
    return string
  attr = []
  if status == 1:
    # green
    attr.append('32')
  else:
    # red
    attr.append('31')
  if bold:
    attr.append('1')
  return '\x1b[%sm%s\x1b[0m' % (';'.join(attr), string)

def InitLog():
  log_level = ''
  options,args = getopt.getopt(sys.argv[1:],"",["level="])
  for name,value in options:
    if name == '--level':
      log_level = value

  if log_level == '':
    log_level = 'debug'

  MkDir("./log")
  logname = './log/log.' + LocalTime()
  handler = logging.FileHandler(logname)
  formatter = logging.Formatter('[%(levelname)s][%(asctime)s]\
  [%(process)d/%(thread)d][%(filename)s:%(lineno)d] %(message)s')
  handler.setFormatter(formatter)
  logging.getLogger().addHandler(handler)

  msg = Highlight("[log level] ", 1, True)
  print msg + log_level
  if log_level == '' or log_level == 'debug':
    logging.getLogger().setLevel(logging.DEBUG)
  elif log_level == 'critical':
    logging.getLogger().setLevel(logging.CRITICAL)
  elif log_level == 'error':
    logging.getLogger().setLevel(logging.ERROR)
  elif log_level == 'warn':
    logging.getLogger().setLevel(logging.WARN)
  elif log_level == 'info':
    logging.getLogger().setLevel(logging.INFO)
  else:
    logging.getLogger().setLevel(logging.NOSET)

def Abort(msg):
  """abort"""
  logging.critical(msg)
  s = inspect.stack()
  prefix = ''
  if len(s) >= 2:
    prefix = '[%s:%d] ' % (os.path.basename(s[1][1]), s[1][2])
  print Highlight('[FAIL]' + prefix, 2, True)+ str(msg)

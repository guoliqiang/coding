#!/usr/bin/env python
#encoding=utf-8
#
# Copyright 2015 Liqiang Guo. All Rights Reserved.
# Author: Liqiang Guo (guoliqiang2006@gmail.com)
# Date  : 2015-02-12 22:45:17
# File  : hello_world.py
# Brief :

def Foo(s1, s2, s3):
  return 'Hello World %s %s %s' % (s1, s2, s3)

class Bar:
  def __init__(self, name, value):
    self.name_ = name
    self.value_ = value
  def Dump(self, s1, s2):
    return 'name = %s value = %s input1=%s input2=%s' % (self.name_, self.value_, s1, s2)

if __name__ == '__main__':
  print Foo('guo', 'li', 'qiang')
  bar = Bar("NAME", "VALUE")
  print bar.Dump("INPUT1", "INPUT2")

#!/usr/bin/python2.6

class BuildingObject(object):
  def __init__(self):
    self.name_ = ''          # logical name.    //base:base
    self.path_ = ''          # virtual path     /home/$ROOT/coding/base/base
    self.sources_ = []       # logical name     //base:string_util.cc
    self.depends_ = []       # logical name     //base:base
    self.raw_depends_ = []   # same as depends_ but raw
    self.option_ = {}        # copt / cflags / link_flags
    self.build_type_ = ''    # cc_binary cc_libary
    pass

  def __str__(self):
    result = '<build obj(%s)' % self.name_
    if self.sources_:
      result += ', src: %s' % str(self.sources_)
    if self.depends_:
      result += ', dep: %s' % str(self.depends_)
    if self.option_:
      result += ', option: %s' % str(self.option_)
    else:
      result += ', public'
      result += '>'
    return result

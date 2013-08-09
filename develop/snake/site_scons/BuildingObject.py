class BuildingObject(object):
  def __init__(self):
    self.name_ = ''              # logical name.    //base:base
    self.path_ = ''              # virtual path     /home/$ROOT/coding/base/base
    self.sources_ = []           # logical name     //base:string_util.cc
    self.depends_ = []           # logical name     //base:base
    self.raw_depends_ = []       # same as depends_ but raw
    self.option_ = {}            # copt / cflags / link_flags
    self.build_type_ = ''        # cc_binary cc_libary
    self.has_thrift_dep = False  # dependent thrift ?
    self.has_proto_dep = False   # dependent proto ?
    pass

  def __str__(self):
    result = '<build obj(%s)' % self.name_
    if self.sources_:
      result += ', src: %s' % str(self.sources_)
    if self.depends_:
      result += ', dep: %s' % str(self.depends_)
    if self.option_:
      result += ', option: %s' % str(self.option_)
    if self.build_type_:
      result += ', build_type: %s' % str(self.build_type_)
    else:
      result += ', thrift_dep: %s ' % str(self.has_thrift_dep)
      result += ', proto_dep: %s ' % str(self.has_proto_dep)
      result += '>'
    return result

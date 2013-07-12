#!/usr/bin/python2.6


from SCons.Script import ARGUMENTS
import Path
import SCons.Script
import Util
import glob
import os
import traceback

"Add new language build here"
_Languages = ('Cpp Thrift')

class BuildManager(object):
  """Object that manages all build for all languages."""
  def __init__(self, env):
    self.env_ = env

    # objects used to analysis dependencies and hold object info
    self.obj_maps_ = {}            # logical path -> object
    self.dependent_snakes_ = []    # before dumplicate
    self.processed_snakes_ = set() # after  dumplicate

    self.building_objs_ = []       # need to build used by SCONS with full depences.
    self.build_targets_ = []       # need to build

    # objects for builder info
    self.builders_ = {}       # cc_binary -> CppBuilder / cc_library -> CppBuilder
    self.lang_builders_ = []  # CppBuilder
    pass


  def _LoadSnakeFile(self, snake_path):
    command = self._GenerateRegistererCommand()
    # from Cpp import cc_binary
    # from Cpp import cc_libary
    try:
      exec(command)
    except:
      Util.Abort('exec failed: %s.' % command)
    # chdir to the lowest dir containing snake file.
    os.chdir(os.path.dirname(snake_path))
    try:
      f = file(snake_path, 'r')
    except:
      Util.Abort('No SNAKE file: %s' % snake_path)
    try:
      exec f
    except:
      traceback.print_exc()
    pass


  def _LoadTargets(self, target):
    "target is logical file path, e.g. //base:base"
    assert not Path.IsStaticLib(target)
    self.dependent_snakes_.append(Path.GetSnakeFilePath(target))
    while len(self.dependent_snakes_) > 0:
      build_file = self.dependent_snakes_.pop(0)
      if not build_file in self.processed_snakes_:
        self.processed_snakes_.add(build_file)
        self._LoadSnakeFile(build_file)

    # ... means build all objects under the directory
    if target.endswith(':*'):
      path = Path.GetAbsPath(target[:-2])
      for obj in self.obj_maps_.itervalues():
        if os.path.dirname(obj.path_) == path:
          self.build_targets_.append(obj.name_)  # logical path
    else:
      self.build_targets_.append(target)         # logical path


  def _GetAllObjs(self, targets):
    obj_list = [[x,x] for x in targets]
    obj_set = []
    for t in targets:
      obj_set.append(t)
    idx = 0
    # get all objs
    while idx < len(obj_list):
      obj_name = obj_list[idx][0]
      obj = self.GetObjByName(obj_name)
      for dep in obj.depends_:
        if Path.IsStaticLib(dep):
          continue
        if dep not in obj_set:
          obj_list.append([dep, obj_name])
          obj_set.append(dep)
      idx += 1
    return [x[0] for x in obj_list]


  def _TopologySort(self, obj_list):
    out_map = {}
    objs = [] + obj_list
    result = []
    for item in obj_list:
      obj = self.GetObjByName(item)
      out_map[item] = len(obj.depends_)
      for dep in obj.depends_:
        if Path.IsStaticLib(dep):
          out_map[item] -= 1
        pass
      pass
    pass
    for i in range(len(obj_list)):
      found = False
      for idx in range(len(objs) - 1, -1, -1):
        if out_map[objs[idx]] == 0:
          found = True
          break

      if not found:
        # found loop, now remove all leaf nodes
        sz = 0
        while (sz != len(out_map)):
          sz = len(out_map)
          keys = out_map.keys()
          for dep in keys:
            dep_num = 0
            for obj_name in keys:
              if (obj_name == dep):
                continue
              if dep in self.GetObjByName(obj_name).depends_:
                dep_num += 1
            if dep_num == 0:
              del out_map[dep]
        Util.Abort('Found dependency loop: %s' % ' '.join(out_map.keys()))

      key = objs[idx]
      result.append(key)
      for k in out_map.iterkeys():
        if key in self.GetObjByName(k).depends_:
          out_map[k] -= 1

      del out_map[key]
      objs.pop(idx)
    result.reverse()
    return result


  def _SpecialCheck(self, obj_list):
    from Cpp import CheckThriftDependency
    for k in range(len(obj_list)):
      name = obj_list[k]
      obj = self.GetObjByName(name)
      CheckThriftDependency(obj)
    pass

  def _GetAllDependents(self, targets):
    obj_list = self._GetAllObjs(targets)
    result = self._TopologySort(obj_list)
    # updates the dependencies
    # add all dependencies
    dep_idx = len(result) - 1
    while dep_idx > 0:
      dep_obj_name = result[dep_idx]
      dep_obj = self.GetObjByName(dep_obj_name)
      obj_idx = dep_idx - 1
      while obj_idx >= 0:
        obj_name = result[obj_idx]
        obj = self.GetObjByName(obj_name)
        if (dep_obj_name in obj.depends_):
          for d in dep_obj.depends_:
            if d in obj.depends_:
              obj.depends_.remove(d)
            obj.depends_.append(d)
        obj_idx -= 1
      dep_idx -= 1
    result.reverse()
    self._SpecialCheck(result)
    return result


  def _PreProcessObjs(self):
    for obj_name in self.building_objs_:
      obj = self.GetObjByName(obj_name)
      try:
        builder = self.builders_[obj.build_type_]
      except:
        Util.Abort('invalid build type: %s' % obj.build_type_)
      builder.PreProcessObject(self.env_, obj)

  def _BuildObjs(self):
    os.chdir(Path.GetBaseDir())
    for obj_name in self.building_objs_:
      obj = self.GetObjByName(obj_name)
      try:
        builder = self.builders_[obj.build_type_]
      except:
        Util.Abort('invalid build type: %s' % obj.build_type_)
      builder.BuildObject(self.env_, obj)
    pass


  def _GenerateRegistererCommand(self):
    result = ''
    for lang, lang_builder in self.lang_builders_:
      registers = lang_builder.GetSnakeRegisterers()
      # frim Cpp import cc_libiary / cc_binary
      for r in registers:
        result += 'from %s import %s\n' % (lang, r)
    return result



  def _AddBuilders(self):
    for lang in _Languages.split():
      command = 'from %s import %sBuilder; lang_builder = %sBuilder()' % (
                 lang, lang, lang)
      try:
        exec(command)
      except:
        traceback.print_exc()
        Util.Abort('exec failed: %s.' % command)
      self.lang_builders_.append((lang, lang_builder))
      registers = lang_builder.GetSnakeRegisterers()
      #cc_binary cc_library
      for r in registers:
        self.builders_[r] = lang_builder
      lang_builder.GenerateEnv(self.env_)
      builders = lang_builder.RegisterSnakeBuilders()
      if len(builders) > 0:
        self.env_.Append(BUILDERS = builders)

  def AddDependentSnakeFile(self, snake_file):
    assert os.path.exists(snake_file), 'INPUT: %s END' % snake_file
    if not snake_file in self.processed_snakes_:
      self.dependent_snakes_.append(snake_file)
    pass

  def HasObj(self, name):
    return self.obj_maps_.has_key(name)

  def GetObjByName(self, name):
    return self.obj_maps_[name]

  def AddObj(self, name, obj):
    self.obj_maps_[name] = obj

  def Build(self, target):
    self._AddBuilders()
    self._LoadTargets(target)
    self.building_objs_ = self._GetAllDependents(self.build_targets_)
    self._PreProcessObjs()
    self._BuildObjs()
    for lang, builder in self.lang_builders_:
      builder.Finish(self.env_)
    pass


class BuildManagerFactory(object):
  _build_manager = None
  def GetBuildManager(self, env = None):
    if BuildManagerFactory._build_manager == None:
      assert isinstance(env, SCons.Script.Environment)
      BuildManagerFactory._build_manager = BuildManager(env)
    return BuildManagerFactory._build_manager


def GetBuildManager(env = None):
  build_manager_factory = BuildManagerFactory()
  return build_manager_factory.GetBuildManager(env)

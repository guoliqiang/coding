#!/usr/bin/env python
#encoding=utf-8
#
# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# I just want to GH to hss~
# Date  : 2013-07-27 21:56:48
# File  : Proto.py
# Brief : 

from LanguageBuilder import LanguageBuilder
from LanguageBuilder import RegisterObj
from SCons.Script import Action
from SCons.Script import Builder
from SCons.Script import ARGUMENTS
import os
import Cpp
import Flags
import Path
import SCons.Util
import Util

_proto_bin = ARGUMENTS.get('proto_bin', '')

def proto_library(name = None, srcs= [], deps = [],
                  gen_cpp = '', gen_py = ''):
  if not name.endswith('_proto'):
    Util.Abort('proto lib\'s name should ends with "_proto"')
  if len(srcs) != 1 or not srcs[0].endswith('.proto'):
    Util.Abort('proto src should be only one .proto file')
  opt = {'gen_cpp':False, 'gen_py':False}
  if str(gen_cpp) == '1':
    opt['gen_cpp'] = True
  if str(gen_py) == '1':
    opt['gen_py'] = True
  RegisterObj(name, srcs, deps, opt, 'proto_library')

def _proto_cpp_emitter(target, source, env):
  path = str(source[0])
  path = os.path.join(os.path.dirname(path),
                      os.path.basename(path))

  cpp_target = os.path.join(Path.GetProtoOutPath(), path)
  prefix = cpp_target.replace('.proto', '.pb.')
  target.append(prefix + 'cc')
  target.append(prefix + 'h')
  return target, source

def _proto_py_emitter(target, source, env):
  path = str(source[0])
  path = os.path.join(os.path.dirname(path),
                      os.path.basename(path))
  cpp_target = os.path.join(Path.GetProtoOutPath(), path)
  prefix = cpp_target.replace('.proto', '.pb2.')
  target.append(prefix + 'py')
  return target, source

class ProtoBuilder(LanguageBuilder):
  """Proto code builders"""
  def __init__(self):
    LanguageBuilder.__init__(self)

  def GetSnakeRegisterers(self):
    return ['proto_library']

  def RegisterSnakeBuilders(self):
    proto_cpp_action = Action('$PROTOCCOM', '$PROTOCCOMSTR')
    proto_cpp_builder = Builder(action = proto_cpp_action,
                                emitter = _proto_cpp_emitter,
                                src_suffix = '.proto')
    proto_py_action = Action('$PROTOPYCCOM', '$PROTOPYCCOMSTR')
    proto_py_builder = Builder(action = proto_py_action,
                               emitter = _proto_py_emitter,
                               src_suffix = '.proto')
    return {'ProtoCppLibrary' : proto_cpp_builder,
            'ProtoPyLibrary' : proto_py_builder}

  def GenerateEnv(self, env):
    assert len(_proto_bin) > 0
    env['PROTO'] = _proto_bin
    env['PROTOINCLUDE'] = ('-I%s -I%s' %
                           (Path.GetBaseDir(),
                            Path.AddBaseDir('third_part/protobuf/include')))
    env['PROTOCOUTDIR'] = Path.GetProtoOutPath()
    env['PROTOCCOM'] = ('$PROTO $PROTOCFLAGS ${SOURCE.abspath} '
                        '--cpp_out=$PROTOCCPPOUTFLAGS$PROTOCOUTDIR $PROTOINCLUDE')
    # python
    env['PROTOPYCCOM'] = ('$PROTO $PROTOCFLAGS ${SOURCE.abspath} '
                          '--python_out=$PROTOCCPPOUTFLAGS$PROTOCOUTDIR $PROTOINCLUDE')
    pass

  def BuildObject(self, env, obj):
    if (not obj.option_['gen_cpp'] and not obj.option_['gen_py']):
      print Util.BuildMessage('%s: at least specify one generated '
                              'language(gen_cpp, gen_py)' % obj.name_,
                              'WARNING')
    source = [Path.GetAbsPath(x) for x in obj.sources_]
    if obj.option_['gen_cpp']:
      cpp_source = env.ProtoCppLibrary([], source)
      cpp_path = Cpp.GetCppInclude(obj)
      target = Path.GetAbsRelativePath(obj.name_)
      env.StaticLibrary(target = target,
                        source = cpp_source,
                        CPPPATH = cpp_path)
    if obj.option_['gen_py']:
      py_source = env.ProtoPyLibrary([], source)


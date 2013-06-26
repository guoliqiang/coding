#!/usr/bin/env python2.6

from LanguageBuilder import LanguageBuilder
from LanguageBuilder import RegisterObj
from SCons.Script import ARGUMENTS
from SCons.Script import Action
from SCons.Script import Builder
from datetime import datetime
import commands
import os
import socket
import Flags
import Path
import Util
import sys
sys.path.append('develop/snake/third_part/python')
import cpplint

"""Cpp build registerers"""
def _cc_internal(name, srcs, deps, copt, libs, path,
                 cflags, link_flags, build_type):
  opt = {}
  if copt != None and len(copt) > 0:
    opt['copt'] = copt

  if cflags != None and len(cflags) > 0:
    opt['cflags'] = cflags

  if libs != None and len(libs) > 0:
    opt['libs'] = libs

  if link_flags != None and len(link_flags) > 0:
    opt['link_flags'] = link_flags

  if path != None and len(path) > 0:
    opt['path'] = path

  RegisterObj(name, srcs, deps, opt, build_type)


def cc_library(name = None, srcs = [], deps = [], copt = [], libs = [], path = [],
               cflags = [], link_flags = []):
  _cc_internal(name, srcs, deps, copt, libs, path,
               cflags, link_flags, 'cc_library')


def cc_binary(name = None, srcs = [], deps = [], libs = [], path = [],
              copt = [], cflags = [], link_flags = []):
  _cc_internal(name, srcs, deps, copt, libs, path,
               cflags, link_flags, 'cc_binary')

def GetCppInclude(obj):
  result = ['.', Path.GetOutputDir(), Path.GetBaseDir()]
  return result

class CppBuilder(LanguageBuilder):
  """C++ code builders"""
  def __init__(self):
    LanguageBuilder.__init__(self)
    cpplint._cpplint_state.ResetErrorCounts()
    self._build_mode = ARGUMENTS.get('c', 'dbg')
    self._distcc_log = ARGUMENTS.get('distcc_log',
                                     '%s/distcc_log' % Flags.SNAKE_OUT)
    self._use_distcc = ARGUMENTS.get('use_distcc', 'on')
    self._lib_name_map = {}
    self._opend_files = set()
    self._checked_dir = set()


  def _HasCopt(self, obj, opt):
    """Checks if an obj is specified one c/c++ option."""
    return obj.option_.has_key('copt') and opt in obj.option_['copt']


  def _GetGccVersion(self):
    return commands.getoutput('gcc -dumpversion')


  def GetSnakeRegisterers(self):
    return ['cc_binary', 'cc_library']


  def _GetOpenedFiles(self, path, obj = None):
    d = os.path.dirname(path)
    if d in self._checked_dir:
      return
    self._checked_dir.add(d)
    for f in obj.sources_:
      if f.endswith('.h') or f.endswith('.cc') or f.endswith('.cpp'):
        self._opend_files.add(os.path.join(d, Path.GetBaseName(f)))
        pass
    pass


  def _StyleCheck(self):
    for f in self._opend_files:
      cpplint.ProcessFile(f, cpplint._cpplint_state.verbose_level, False)
    if cpplint._cpplint_state.error_count > 0:
      print Util.BuildMessage('There\'re %d style warnings in the opend files, '
                              'please try fixing them before submit the code!' %
                              cpplint._cpplint_state.error_count, 'WARNING')
    pass


  def _ConfigForDistcc(self, env):
    if self._use_distcc == 'on':
      distcc_hosts = [x.strip() for x in open(Flags.DISTCC_HOSTS).read().split('\n')]
      distcc_hosts.sort()
      while len(distcc_hosts[0]) == 0:
        del distcc_hosts[0]
      # check if the hostname is in the list
      hostname = '%s.yunrang.com' % socket.gethostname()
      for i in range(len(distcc_hosts)):
        if distcc_hosts[i] == hostname:
          distcc_hosts = distcc_hosts[i+1:] + distcc_hosts[0:i]
          break
        if distcc_hosts[i] > hostname:
          distcc_hosts = distcc_hosts[i:] + distcc_hosts[0:i]
          break
      env['ENV']['DISTCC_HOSTS'] = ' '.join(distcc_hosts)
      if len(self._distcc_log) > 0:
        if os.path.exists(self._distcc_log):
          os.remove(self._distcc_log)
        env['ENV']['DISTCC_LOG'] = self._distcc_log
      distcc_dir = os.path.join(Flags.SNAKE_OUT, '.distcc')
      if not os.path.exists(distcc_dir):
        os.mkdir(distcc_dir)
      env['ENV']['DISTCC_DIR'] = distcc_dir
      env['CXX'] = Flags.DISTCC_BIN


  def GenerateEnv(self, env):
    self._ConfigForDistcc(env)
    env['LINK'] = 'g++'
    cc_flags = ('-m64 -fPIC -Wall -Werror -Wwrite-strings -Wno-sign-compare -g '
                '-Wuninitialized -Wno-error=deprecated-declarations ')
    link_flags = ['-pthread ']
    if self._GetGccVersion() >= '4.5':  # only available after gcc4.5
      link_flags.append('-static-libstdc++')
    else:
      cc_flags += '-fno-strict-aliasing '

    env.Replace(LINKFLAGS = ' '.join(link_flags))

    if self._build_mode == 'dbg':
      env.Replace(CCFLAGS = ' '.join([cc_flags]))
    elif self._build_mode == 'opt':
      env.Replace(CCFLAGS = ' '.join([cc_flags, '-O2 -DNDEBUG']))
    else:
      assert False, 'WRONG build strategy: %s' % self._build_mode
    pass

  def _GetSourcePath(self, sources):
    result = [];
    for x in sources:
      result.append(Path.GetAbsRelativePath(x))
    return result


  def _GetLibName(self, path):
    lib_name = 'lib' + os.path.basename(path) + '.a'
    return os.path.join(os.path.dirname(path), lib_name)


  def _GetLibPath(self, name):
    try:
      lib_name = self._lib_name_map[name]
    except:
      build_name = Path.GetAbsRelativePath(name)
      lib_name = self._GetLibName(build_name)
      self._lib_name_map[name] = lib_name
    return lib_name


  def _GetStaticLib(self, path, abort = True):
    return Path.GetAbsPath(self._GetLibName(p), abort)


  def _GetLibPathForObj(self, obj):
    if Path.IsStaticLib(obj.name_):
      return self._GetStaticLib(obj.name_)
    return self._GetLibPath(obj.name_)


  def _GetFlags(self, obj, env):
    source = []
    path = []
    always_link_libs = ''
    libs = ['dl', 'rt', 'crypt']

    if 'libs' in obj.option_:
      libs += obj.option_['libs']

    if 'path' in obj.option_:
      path += obj.option_['path']

    # check dependent obj's special attributes
    for d in obj.depends_:
      dep_obj = self.build_manager_.GetObjByName(d)
      d_lib = self._GetLibPathForObj(dep_obj)
      if self._HasCopt(dep_obj, 'always_link'):
        always_link_libs += ' %s' % d_lib
        # add explicit dependency since Scons could not figure out such dep
        # relationship for always link lib
        obj_name = Path.GetAbsRelativePath(obj.name_)
        env.Depends(obj_name, d_lib)
      else:
        source.append(d_lib)

    link_flags = env['LINKFLAGS']
    if len(always_link_libs) > 0:
      link_flags += (' -Wl,--whole-archive%s -Wl,--no-whole-archive' %
                     always_link_libs)
    if 'link_flags' in obj.option_:
      link_flags += ' '
      link_flags += ' '.join(obj.option_['link_flags'])
    return source, libs, path, link_flags


  def BuildObject(self, env, obj):
    self._GetOpenedFiles(obj.path_, obj)
    # convert to SCons dendency graph
    target = Path.GetAbsRelativePath(obj.name_)
    source = self._GetSourcePath(obj.sources_)
    cpp_path = GetCppInclude(obj)
    lib_sources, libs, libpath, link_flags = self._GetFlags(obj, env)
    cc_flags = env['CCFLAGS']
    if 'cflags' in obj.option_:
      cc_flags += ' '
      cc_flags += ' '.join(obj.option_['cflags'])

    CXX_value = env['CXX']
    if (not self._use_distcc == 'on'):
      CXX_value = 'g++'

    if obj.build_type_ in ['cc_binary']:
      print Util.BuildMessage(target)
      binary = env.Program(target = target,
                           source = source + lib_sources,
                           LIBS = libs,
                           LIBPATH = libpath,
                           CPPPATH = cpp_path,
                           LINKFLAGS = link_flags,
                           CCFLAGS = cc_flags,
                           CXX = CXX_value)
    if obj.build_type_ in ['cc_library']:
      print Util.BuildMessage(os.path.dirname(target) +
                              "/lib" +
                              os.path.basename(target) + ".a")
      result = env.StaticLibrary(target = target,
                                 source = source,
                                 CPPPATH = cpp_path,
                                 CCFLAGS = cc_flags,
                                 CXX = CXX_value)
  def Finish(self, env):
    self._StyleCheck()

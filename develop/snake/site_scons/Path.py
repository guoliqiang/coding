#!/usr/bin/env python2.6

from SCons.Script import ARGUMENTS
import os
import re
import Flags
import Util


_base_dir = os.path.normpath(ARGUMENTS.get('base_dir'))

def GetBaseDir():
  """root dir of the code base."""
  return _base_dir


def GetOutputDir():
  """Get '.snake-out/opt' or '.snake-out/dbg'"""
  strategy = ARGUMENTS.get('c', 'dbg')
  return os.path.join(GetBaseDir(), Flags.SNAKE_OUT, strategy)


def GetSnakeFilePath(logical_path):
  """'//base:base' --> '.../$ROOT/base/SNAKE'"""
  assert logical_path.startswith('//') and logical_path.find(':') != -1
  snake_path = os.path.join(logical_path.split(':')[0], Flags.SNAKE_NAME)
  return GetAbsPath(snake_path, abort = False)


def IsInDir(file_path, dir_path):
  dir_path = os.path.normpath(dir_path) + '/'
  return file_path.startswith(dir_path)


def GetLogicalPath(cur_dir, path):
  """('~/$ROOT/base', 'foo') --> '//base:foo'"""
  if path.startswith('//'):
    return path

  if path.startswith(':'):
    assert path.find('/') == -1
    abs_path = os.path.join(cur_dir, path[1:])
  else:
    abs_path = os.path.join(cur_dir, path)

  result = os.path.dirname(abs_path) + ':' + os.path.basename(abs_path)
  if IsInDir(abs_path, GetBaseDir()):
    result = '//%s' % os.path.relpath(result, GetBaseDir())
  else:
    Util.Abort('invalid path: (%s, %s)' % (cur_dir, path))
  return result


def GetRelativePath(path):
  assert path.startswith('//')
  return path[2:].replace(':', '/')


def GetAbsPath(path, abort = True):
  assert path.startswith('//')
  path = GetRelativePath(path)
  result = os.path.join(GetBaseDir(), path)
  if os.path.exists(result):
    return result
  if abort:
    Util.Abort('can not find absolute path for %s' % path)
  else:
    return ''
  pass


def GetAbsRelativePath(path):
  """Given a logical path, give out its building path"""
  assert path.startswith('//')
  return os.path.join(GetOutputDir(), path[2:]).replace(':', '/')


def GetBaseName(name):
  idx = name.index(':')
  return name[idx+1:]


def IsStaticLib(lib_name):
  if lib_name.startswith(Flags.STATIC_LIB_PREFIX):
    return True
  return False


def IsSVNClient():
  return os.path.exists(os.path.join(GetBaseDir(), '.svn'))


def IsGITClient():
  return os.path.exists(os.path.join(GetBaseDir(), '.git'))

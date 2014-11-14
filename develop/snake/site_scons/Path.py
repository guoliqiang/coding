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
  """'.snake-out/opt' or '.snake-out/dbg'"""
  strategy = ARGUMENTS.get('c', 'dbg')
  return os.path.join(GetBaseDir(), Flags.SNAKE_OUT, strategy)

def GetSnakeFilePathWithoutAbort(logical_path):
  """'//base/internal:base' --> '~/$ROOT/base/internal/SNAKE'"""
  assert logical_path.startswith('//') and logical_path.find(':') != -1 , "INPUT : %s" % logical_path
  snake_path = os.path.join(logical_path.split(':')[0], Flags.SNAKE_NAME)
  return GetAbsPath(snake_path, abort = False)

def Dep2Path(path):
  """dep path to paths for Python"""
  """//base/internal:base --> //base/internal"""
  assert path.startswith('//'), "INPUT %s" % path
  return path[0 : path.index(':')]

def GetSnakeFilePath(logical_path):
  """'//base/internal:base' --> '~/$ROOT/base/internal/SNAKE'"""
  assert logical_path.startswith('//') and logical_path.find(':') != -1 , "INPUT : %s" % logical_path
  snake_path = os.path.join(logical_path.split(':')[0], Flags.SNAKE_NAME)
  return GetAbsPath(snake_path, abort = True)


def IsInDir(file_path, dir_path):
  """Is file_path in dir_path?"""
  dir_path = os.path.normpath(dir_path) + '/'
  return file_path.startswith(dir_path)


def GetLogicalPath(cur_dir, path):
  """('~/$ROOT/base/internal/', 'base') --> '//base/internal:base'"""
  if path.startswith('//'):
    return path
  if path.startswith(':'):
    assert path.find('/') == -1, "INPUT %s" % path
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
  """//base/internal:base --> //base/internal/base"""
  assert path.startswith('//'), "INPUT %s" % path
  return path[2:].replace(':', '/')

def RemoveSnake(path):
  return path.replace('//libs/SNAKE', '//libs')
 
def GetPrivateLogicalPath(path):
  """"""
  assert path.startswith(Flags.STATIC_LIB_PREFIX), "INPUT %s" % path
  return Flags.STATIC_SNAKE_PREFIX + '/' + path[len(Flags.STATIC_LIB_PREFIX):]

def AddBaseDir(path):
  """path -> ~/$ROOT/path"""
  return os.path.join(GetBaseDir(), path)


def GetAbsPath(path, abort = True):
  """//base/internal:base --> ~/$ROOT/base/internal/base"""
  assert path.startswith('//'), "INPUT %s" % path
  path = GetRelativePath(path)
  result = os.path.join(GetBaseDir(), path)
  if os.path.exists(result):
    return result
  if abort:
    Util.Abort('can not find absolute path: %s' % result)
  else:
    return ''
  pass


def GetThriftOutPath():
  return os.path.join(GetOutputDir(), Flags.THRIFT_OUT)

def GetProtoOutPath():
  return os.path.join(GetOutputDir(), Flags.PROTO_OUT)

def GetAbsRelativePath(path):
  """//base/internal:base  --> ~/$ROOT/.snake/dbg/base/internal/base or
                               ~/$ROOT/.snake/opt/base/internal/base """
  assert path.startswith('//') , "INPUT %s" % path
  return os.path.join(GetOutputDir(), path[2:]).replace(':', '/')


def GetBaseName(name):
  """//base/internal:base --> base"""
  idx = name.index(':')
  return name[idx+1:]


def IsStaticLib(lib_name):
  """//libs/  --> true"""
  assert lib_name.startswith('//'), "INPUT %s" % lib_name
  if lib_name.startswith(Flags.STATIC_LIB_PREFIX):
    return True
  return False


def IsSVNClient():
  """Is svn?"""
  return os.path.exists(os.path.join(GetBaseDir(), '.svn'))


def IsGITClient():
  """Is git?"""
  return os.path.exists(os.path.join(GetBaseDir(), '.git'))

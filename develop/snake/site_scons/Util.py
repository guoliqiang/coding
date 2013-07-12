from SCons.Script import ARGUMENTS
import glob
import hashlib
import inspect
import os
import re
import subprocess
import sys
import time
import Flags
import SCons
import socket
import fcntl
import struct
import stat

_verbose = int(ARGUMENTS.get('verbose', '1'))

def GetIp(ifname):
  """get the ip of host"""
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  return socket.inet_ntoa(fcntl.ioctl(s.fileno(),
                                      0x8915,  # SIOCGIFADDR
                                      struct.pack('256s', ifname[:15])
                                     )[20:24]
                         )

def Log(msg, v = 0):
  """print log info"""
  if v < _verbose: return
  s = inspect.stack()
  prefix = ''
  if len(s) > 2:
    prefix = '%s:%d: ' % (os.path.basename(s[1][1]), s[1][2])
  print('%s%s' % (prefix, str(msg)))


def BuildMessage(msg, msg_type = 'INFO'):
  """build msg with color"""
  message_head = Highlight("[INFO]", 1, False)
  if msg_type in ['WARNING', 'TIMEOUT', 'FAIL']:
    message_head = Highlight("[%s]" % msg_type, 2, False)
  if msg_type == 'PASS':
    message_head = Highlight("[PASS]", 1, False)
  return "%s %s" % (message_head, msg)


def Abort(msg):
  """abort"""
  print BuildMessage(msg, 'FAIL')
  sys.exit(-1)


def LogWithTimestamp(msg):
  """print log info with timestamp"""
  print '[%s]: %s' % (time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())), msg)
  pass

def LocalTime():
  """return local time"""
  return "%s" % time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))

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


def EnsureSnakeDir():
  """check and creak snake dir"""
  def _GetHashedName():
    path = os.getcwd()
    m = hashlib.md5()
    m.update(path)
    return m.hexdigest()

  def _CreateDirs():
    if not os.path.exists(Flags.LOCAL_SNAKE_DIR):
      Abort('No [%s], please create and chmod 777 for it! ' % Flags.LOCAL_SNAKE_DIR)
    if not ModeMatch(Flags.LOCAL_SNAKE_DIR, 511): # 511 (decimal) is the same as 777 (octal).
      Abort('Please chmod 777 for [%s]' % Flags.LOCAL_SNAKE_DIR)
    path = os.path.join(Flags.LOCAL_SNAKE_DIR, _GetHashedName())
    if not os.path.exists(path):
      try:
        os.mkdir(path)
      except:
        Abort('create %s failed, maybe no access?' % path)
    try:
      os.symlink(path, Flags.SNAKE_OUT)
    except:
      Abort('please try to rm -rf [%s]' % Flags.SNAKE_OUT)
    MkDir(os.path.join(path, 'opt'))
    MkDir(os.path.join(path, 'dbg'))

  if not os.path.islink(Flags.SNAKE_OUT):
    if os.path.exists(Flags.SNAKE_OUT):
      print BuildMessage('Deleting the old output dir %s' % Flags.SNAKE_OUT,
                         'WARNING')
      os.spawnlp(os.P_WAIT, 'rm', 'rm', '-rf', Flags.SNAKE_OUT)
    _CreateDirs()
  elif not os.path.exists(Flags.SNAKE_OUT):
    _CreateDirs()


def QuietBuild(s, target, src, env):
  """Save real cmd to log file"""
  open(env['CMD_LOGFILE'], 'a').write('%s\n' % s);


def ColorfulBuild(env, log_in_one_line):
  """color the infomation of building"""
  ctl = {}
  ctl['cyan']   = '\033[96m'
  ctl['purple'] = '\033[95m'
  ctl['blue']   = '\033[94m'
  ctl['green']  = '\033[92m'
  ctl['yellow'] = '\033[93m'
  ctl['end']    = '\033[0m'
  ctl['moveon'] = '\033[1A\033[K'
  if not log_in_one_line:
    ctl['moveon'] = ''

  # If the output is not a terminal, remove the ctl
  if not sys.stdout.isatty() or os.environ.get('TERM') == 'dumb':
    for key, value in ctl.iteritems():
      ctl[key] = ''
  elif log_in_one_line:
    env['SPAWN'] = EchoSpawn

  compile_source_message = ('%s%sCompiling %s==> %s$SOURCE%s' %
      (ctl['moveon'], ctl['blue'], ctl['purple'], ctl['yellow'], ctl['end']))
  compile_shared_source_message = ('%s%sCompiling shared %s==> %s$SOURCE%s' %
      (ctl['moveon'], ctl['blue'], ctl['purple'], ctl['yellow'], ctl['end']))
  link_program_message = ('%s%sLinking Program %s==> %s$TARGET%s' %
      (ctl['moveon'], ctl['green'], ctl['purple'], ctl['yellow'], ctl['end']))
  link_library_message = ('%s%sLinking Static Library %s==> %s$TARGET%s' %
      (ctl['moveon'], ctl['green'], ctl['purple'], ctl['yellow'], ctl['end']))
  ranlib_library_message = ('%s%sRanlib Library %s==> %s$TARGET%s' %
      (ctl['moveon'], ctl['green'], ctl['purple'], ctl['yellow'], ctl['end']))
  link_shared_library_message = ('%s%sLinking Shared Library %s==> %s$TARGET%s' %
      (ctl['moveon'], ctl['green'], ctl['purple'], ctl['yellow'], ctl['end']))
  gen_cpp_message = ('%s%sGenerating C++ file from %s==> %s$SOURCE%s' %
      (ctl['moveon'], ctl['green'], ctl['purple'], ctl['yellow'], ctl['end']))
  gen_java_message = ('%s%sGenerating java file from %s==> %s$SOURCE%s' %
      (ctl['moveon'], ctl['green'], ctl['purple'], ctl['yellow'], ctl['end']))
  gen_php_message = ('%s%sGenerating php file from %s==> %s$SOURCE%s' %
      (ctl['moveon'], ctl['green'], ctl['purple'], ctl['yellow'], ctl['end']))
  gen_python_message = ('%s%sGenerating python file from %s==> %s$SOURCE%s' %
      (ctl['moveon'], ctl['green'], ctl['purple'], ctl['yellow'], ctl['end']))

  env['CXXCOMSTR'] = compile_source_message
  env['CCCOMSTR'] = compile_source_message
  env['SHCCCOMSTR'] = compile_shared_source_message
  env['SHCXXCOMSTR'] = compile_shared_source_message
  env['ARCOMSTR'] = link_library_message
  env['RANLIBCOMSTR'] = ranlib_library_message
  env['SHLINKCOMSTR'] = link_shared_library_message
  env['LINKCOMSTR'] = link_program_message
  env['THRIFTCCOMSTR'] = gen_cpp_message
  env['THRIFTJAVACOMSTR'] = gen_java_message
  env['THRIFTPHPCOMSTR'] = gen_php_message
  env['THRIFTPYCOMSTR'] = gen_python_message


def MatchOnePattern(file_path, pattern, group_num = 1):
  result = []
  pattern_re = re.compile(pattern)
  for line in open(str(file_path), 'r'):
    m = pattern_re.match(line)
    if m:
      result.append(m.group(group_num))
  return result


def EchoSpawn(sh, escape, cmd, args, env):
  p = subprocess.Popen(' '.join(args),
                       env = env,
                       stderr = subprocess.PIPE,
                       stdout = subprocess.PIPE,
                       shell = True,
                       universal_newlines = True)
  (stdout, stderr) = p.communicate()

  if len(stdout) > 0:
    sys.stdout.write(stdout + '\n')
  if len(stderr) > 0:
    sys.stderr.write(stderr + '\n')
  return p.returncode


def PrepareEnv(env):
  log = ARGUMENTS.get('log', 'vs')
  if log == 'q':
    env['CMD_LOGFILE'] = os.path.join(Flags.SNAKE_OUT, 'build.log')
    env['PRINT_CMD_LINE_FUNC'] = QuietBuild
    print('%s        %s' %
          (Highlight('Build log:', 1, False), env['CMD_LOGFILE']))
    if os.path.exists(env['CMD_LOGFILE']):
      os.remove(env['CMD_LOGFILE'])
  elif log == 's':
    ColorfulBuild(env, False)
  elif log == 'vs':
    ColorfulBuild(env, True)

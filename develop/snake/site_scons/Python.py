from LanguageBuilder import LanguageBuilder
from LanguageBuilder import RegisterObj
from SCons.Script import Action
from SCons.Script import Builder
from SCons.Script import ARGUMENTS
from pylint import lint
from pylint.reporters.text import TextReporter

import Util
import os
import Flags
import Path
import sys

input_target = ARGUMENTS.get('t')

class WritableObject(object):
    def __init__(self):
        self.content = []
    def write(self, st):
        self.content.append(st)
    def read(self):
        return self.content

def py_binary(name, srcs, sibling = [], deps=[], paths=[]):
    opt = {}
    opt['paths'] = paths
    assert len(srcs) == 1
    RegisterObj(name, srcs, deps, opt, 'py_binary')

class PythonBuilder(LanguageBuilder):
    """Python code builder"""
    def __init__(self):
        LanguageBuilder.__init__(self)
        self._build_mode = ARGUMENTS.get('c', 'dbg')

    def GetSnakeRegisterers(self):
        return ['py_binary']

    def RegisterSnakeBuilders(self):
        action = Action('$PYTHONCOM', '$PYTHONCOMSTR')
        builder = Builder(action = action)
        return {'PythonBuilder' : builder}

    def GenerateEnv(self, env):
        env['PYTHONCOM'] = ('export PYTHONPATH=$OPTIONS && '
                            '$PYTHONBIN $PY_SOURCE')
        env['PYTHONBIN'] = Path.GetAbsPath(Flags.PYTHON_BIN)
        env['PYTHONBIN'] = '/usr/bin/python'
        env['PYTHONCOMSTR'] = '\x1b[32m[BEGIN RUN: $PY_SOURCE]\x1b[0m'

    def PreProcessObject(self, env, obj):
        self._CheckSpecialDependency(obj)
        self._GenerateConfiguration()

    def BuildObject(self, env, obj):
        source = Path.GetAbsPath(obj.sources_[0])
        target = Path.GetAbsRelativePath(obj.name_)
        target_dir = os.path.dirname(target)

        if not os.path.isdir(target_dir):
            os.makedirs(target_dir)

        basename = Path.GetBaseName(obj.name_)
        options = []
        if obj.option_['paths']:
            options.append(self._GetImportPaths(obj.option_['paths']))
        if basename == input_target:
            for item_i in options:
                for item_j in item_i.strip().split(':'):
                    sys.path.append(item_j)

            pylint_output = WritableObject()
            lint.Run(['-r', 'n', source], reporter = TextReporter(pylint_output), exit = False)
            for line in pylint_output.read():
                if len(line.strip()) != 0:
                  if line.startswith("C:") or line.startswith("R:"):
                      pass
                  elif line.startswith("W:"):
                      if line.find('bad-indentation'): continue
                      print Util.BuildMessage('[%s] %s' % (basename, line), 'INFO')
                  elif line.startswith('E:') or line.startswith('F:'):
                      print Util.BuildMessage('[%s] %s' % (basename, line), 'WARNING')
            env.PythonBuilder(target, source,
                              PY_SOURCE = source, OPTIONS = ' '.join(options))
    def _GetImportPaths(self, paths):
        abs_paths = []
        for path in paths:
            abs_paths.append(Path.GetAbsPath(path))
        return ':'.join(abs_paths)

    def _GenerateConfiguration(self):
        pass

    def _CheckSpecialDependency(self, obj):
        dep_paths = set()
        has_thrift_dep = False
        for d in obj.depends_:
            if d.endswith('_thrift'):
                has_thrift_dep = True
                path = os.path.join(Path.GetThriftOutPath(), Path.GetRelativePath(d))
                path = os.path.join(os.path.dirname(path), 'gen-py')
                dep_paths.add(Path.GetRelativePath(path))
                dep_obj = self.build_manager_.GetObjByName(d)
                dep_obj.option_['gen_cpp'] = False
                dep_obj.option_['gen_php'] = False
                dep_obj.option_['gen_py'] = True
        if has_thrift_dep:
            obj.option_['paths'].extend(dep_paths)

        dep_paths = set()
        has_proto_dep = False
        for d in obj.depends_:
            if d.endswith('_proto'):
                has_proto_dep = True
                path = os.path.join(Path.GetProtoOutPath(), Path.GetRelativePath(d))
                path = os.path.dirname(path)
                dep_paths.add(Path.GetRelativePath(path))
                dep_obj = self.build_manager_.GetObjByName(d)
                dep_obj.option_['gen_cpp'] = False
                dep_obj.option_['gen_php'] = False
                dep_obj.option_['gen_py'] = True
        if has_proto_dep:
            obj.option_['paths'].extend(dep_paths)

        for d in obj.depends_:
            if not d.endswith('_proto') and not d.endswith('_thrift'):
                obj.option_['paths'].append(Path.Dep2Path(d))

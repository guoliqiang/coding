#! /usr/bin/env python

import sys

Mode="dbg"
cur_path_prefix=sys.path[0].split('/yr/')[0]
binary_dir=cur_path_prefix + "/yr/.ymake-out/" + Mode + "/util/mysql_client2/"

class bcolors:
  HEADER = '\033[95m'
  OKBLUE = '\033[94m'
  OKGREEN = '\033[92m'
  WARNING = '\033[93m'
  FAIL = '\033[91m'
  ENDC = '\033[0m'

  def disable(self):
    self.HEADER = ''
    self.OKBLUE = ''
    self.OKGREEN = ''
    self.WARNING = ''
    self.FAIL = ''
    self.ENDC = ''

def DBG_INFO(Level, string):
  if Level=="HEADER":
    print bcolors.HEADER + string + bcolors.ENDC
  if Level=="OK":
    print bcolors.OKGREEN + string + bcolors.ENDC
  if Level=="WARNING":
    print bcolors.WARNING + string + bcolors.ENDC
  if Level=="FAIL":
    print bcolors.FAIL + string + bcolors.ENDC

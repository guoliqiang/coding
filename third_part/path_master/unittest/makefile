# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-30 11:04:22


WORK_ROOT=../../..
PROJ_NAME=third_part/path_master/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/path_master_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/path_master_benchmark $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/path_test $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/path_test2 $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/path_test3 $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:

clean:
	rm -rf  path_master_unittest
	rm -rf  path_master_benchmark
	rm -rf  path_test
	rm -rf  path_test2
	rm -rf  path_test3


# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-06 17:20:23


WORK_ROOT=../../..
PROJ_NAME=third_part/file_monitor/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/notify_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/file_monitor_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:
	$(WORK_ROOT)/develop/snake/snake.sh t=... c=opt
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/notify_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/file_monitor_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


clean:


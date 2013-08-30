# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-12 11:05:51


WORK_ROOT=../../..
PROJ_NAME=third_part/googleurl/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/*** $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:
	$(WORK_ROOT)/develop/snake/snake.sh t=* c=opt
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/*** $(WORK_ROOT)/$(PROJ_NAME)/; \


clean:


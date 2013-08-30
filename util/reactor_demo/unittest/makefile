# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-02 14:02:57


WORK_ROOT=../../..
PROJ_NAME=util/reactor_demo/internal


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/reactor_client $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/reactor_server $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:
	$(WORK_ROOT)/develop/snake/snake.sh t=... c=opt
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/reactor_client $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/reactor_server $(WORK_ROOT)/$(PROJ_NAME)/; \


clean:


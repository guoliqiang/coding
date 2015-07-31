# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-09 15:26:45


WORK_ROOT=../../..
PROJ_NAME=third_part/binlog_parser/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/replaybinlog $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/replay_sys_vars $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/basic-1 $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/basic-2 $(WORK_ROOT)/$(PROJ_NAME)/; \

opt:


clean:
	rm -rf replaybinlog
	rm -rf replay_sys_vars
	rm -rf basic-1
	rm -rf basic-2

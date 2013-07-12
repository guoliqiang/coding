# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-07-03 19:49:15


WORK_ROOT=../../..
PROJ_NAME=third_part/zookeeper/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  #cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/zookeeper $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/zookeeper_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:
	$(WORK_ROOT)/develop/snake/snake.sh t=* c=opt
	#if test $$? -ne 0; then exit 1; fi; \
  #cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/*** $(WORK_ROOT)/$(PROJ_NAME)/; \


clean:
	#rm -rf $(WORK_ROOT)/$(PROJ_NAME)/* \
  #rm -rf $(WORK_ROOT)/.ymake-out/dbg/$(PROJ_NAME)/* \
  #rm -rf $(WORK_ROOT)/.ymake-out/opt/$(PROJ_NAME)/* \


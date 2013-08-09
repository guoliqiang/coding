# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-08 16:09:59


WORK_ROOT=../../..
PROJ_NAME=third_part/http_server/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/http_server_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:
	$(WORK_ROOT)/develop/snake/snake.sh t=* c=opt
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/http_server_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


clean:


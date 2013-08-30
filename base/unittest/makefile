# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-28 10:07:22


WORK_ROOT=../..
PROJ_NAME=base/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/icu_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:


clean:


# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-30 17:23:32


WORK_ROOT=../../../..
PROJ_NAME=third_part/mempool/arena/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/arena_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:


clean:
	rm -rf arena_unittest

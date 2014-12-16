# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-09 15:26:45


WORK_ROOT=../../..
PROJ_NAME=third_part/leveldb_src_1.12/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=* c=opt
	if test $$? -ne 0; then exit 1; fi; \
	cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/variable_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \

opt:


clean:
	rm -rf variable_unittest


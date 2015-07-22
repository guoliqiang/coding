# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-09 15:26:45


WORK_ROOT=../../..
PROJ_NAME=third_part/kdtree++/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test_kdtree $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test_hayne $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test_find_within_range $(WORK_ROOT)/$(PROJ_NAME)/; \

opt:


clean:
	rm -rf test_kdtree
	rm -rf test_hayne
	rm -rf test_find_within_range

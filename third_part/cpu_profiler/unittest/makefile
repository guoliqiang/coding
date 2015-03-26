# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-09 15:26:45


WORK_ROOT=../../..
PROJ_NAME=third_part/cpu_profiler/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/profile_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/profile_analyzer_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/timer_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \

opt:


clean:
	rm -rf profile_unittest
	rm -rf profile_analyzer_unittest
	rm -rf cpu_profiler.prof
	rm -rf cpu_profiler.symbol
	rm -rf timer_unittest

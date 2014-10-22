# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-09 15:26:45


WORK_ROOT=../../..
PROJ_NAME=third_part/uthread/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/uthread_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test1 $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test2 $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test3 $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test4 $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test5 $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test6 $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test7 $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test8 $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/test9 $(WORK_ROOT)/$(PROJ_NAME)/; \

opt:


clean:
	rm -rf uthread_unittest 
	rm -rf test1
	rm -rf test2
	rm -rf test3
	rm -rf test4
	rm -rf test5
	rm -rf test6
	rm -rf test7
	rm -rf test8
	rm -rf test9

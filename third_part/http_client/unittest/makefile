# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-19 13:58:29


WORK_ROOT=../../..
PROJ_NAME=third_part/http_client/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/http_client_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/curl_wrapper_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/wget $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:
	$(WORK_ROOT)/devel/snake/snake.sh t=* c=opt
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/http_client_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/curl_wrapper_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/opt/$(PROJ_NAME)/wget $(WORK_ROOT)/$(PROJ_NAME)/; \


clean:
	rm -rf http_client_unittest
	rm -rf curl_wrapper_unittest
	rm -rf wget
	rm -rf out.html

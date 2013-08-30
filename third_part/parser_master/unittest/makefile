# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-27 13:44:36


WORK_ROOT=../../..
PROJ_NAME=third_part/parser_master/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/parse_master_util_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/dom_node_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/html_tag_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/cdata_test $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/html_lexer_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/parse_master_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/parse_master_benchmark $(WORK_ROOT)/$(PROJ_NAME)/; \


clean:
	rm -rf  parse_master_util_unittest
	rm -rf  dom_node_unittest
	rm -rf  html_tag_unittest
	rm -rf  cdata_test
	rm -rf  html_lexer_unittest
	rm -rf  parse_master_unittest
	rm -rf  parse_master_benchmark

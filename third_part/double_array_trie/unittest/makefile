# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-09 15:26:45


WORK_ROOT=../../..
PROJ_NAME=third_part/double_array_trie/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/da_trie2_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/da_trie_string16_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/da_trie_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \

opt:


clean:
	rm -rf da_trie2_unittest
	rm -rf da_trie_string16_unittest
	rm -rf da_trie_unittest

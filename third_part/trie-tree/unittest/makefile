# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-09 15:26:45


WORK_ROOT=../../..
PROJ_NAME=third_part/trie-tree/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/trie_tree_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/trie_tree_unit_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \

opt:


clean:
	rm -rf trie_tree_unit_unittest
	rm -rf trie_tree_unittest


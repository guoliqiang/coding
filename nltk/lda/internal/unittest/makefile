# Copyright 2015. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang2006@gmail.com)
# Date  : 2015-09-07 14:18:29


WORK_ROOT=../../../..
PROJ_NAME=/nltk/lda/internal/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/train_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/predict_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:


clean:
	rm -rf  train_unittest
	rm -rf  predict_unittest
	rm -rf  ./data/model*
	rm -rf  ./data/doc-topic*
	rm -rf  ./data/topic-word*
	rm -rf  ./data/pmodel*
	rm -rf  ./data/pdoc-topic*
	rm -rf  ./data/ptopic-word*

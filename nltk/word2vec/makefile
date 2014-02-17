# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-02 16:14:54


WORK_ROOT=../..
PROJ_NAME=nltk/word2vec


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/word2vec $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:


clean:

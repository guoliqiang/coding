# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-03 07:58:09


WORK_ROOT=../../..
PROJ_NAME=nltk/perceptron/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/perceptron_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:


clean:


# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-08 03:12:40


WORK_ROOT=../../../../..
PROJ_NAME=nltk/lda/third_part/GibbsLDA++-0.2/src


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/lda $(WORK_ROOT)/$(PROJ_NAME)/; \

opt:


clean:
	rm -rf lda

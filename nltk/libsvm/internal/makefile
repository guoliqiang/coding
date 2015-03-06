# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-02 16:14:54


WORK_ROOT=../../..
PROJ_NAME=nltk/libsvm/internal


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/svm_train $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/svm_predict $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/svm_scale $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:


clean:
	rm -rf svm_train
	rm -rf svm_predict
	rm -rf svm_scale
	rm -rf nohup.out
	rm -rf pinput.txt.out
	rm -rf input.txt.model

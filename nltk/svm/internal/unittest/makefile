# Copyright 2013. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang2006@gmail.com)
# Date  : 2013-09-07 14:18:29


WORK_ROOT=../../../..
PROJ_NAME=/nltk/svm/internal/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/train_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/problem_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/predict_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/cache_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/kernel_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:


clean:
	rm -rf  train_unittest
	rm -rf  problem_unittest
	rm -rf  predict_unittest
	rm -rf  cache_unittest
	rm -rf  kernel_unittest
	rm -rf  svm_predict.prof
	rm -rf  svm_train.prof
	rm -rf  output
	rm -rf  output.debug
	rm -rf  pinput.txt.out






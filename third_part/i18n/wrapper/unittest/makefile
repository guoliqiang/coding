# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-08-29 10:30:17


WORK_ROOT=../../../..
PROJ_NAME=third_part/i18n/wrapper/unittest


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/mozilla_encoding_detector_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/mozilla_language_detector_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/invalid_code_detector_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
  cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/utf8_converter_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \


opt:


clean:
	rm -rf mozilla_encoding_detector_unittest
	rm -rf mozilla_language_detector_unittest
	rm -rf invalid_code_detector_unittest
	rm -rf utf8_converter_unittest

# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com) # Date  : 2013-09-09 15:26:45


WORK_ROOT=../../..
PROJ_NAME=third_part/gperftools_src/tests


all:
	$(WORK_ROOT)/develop/snake/snake.sh t=*
	if test $$? -ne 0; then exit 1; fi; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/profiler_unittest $(WORK_ROOT)/$(PROJ_NAME)/profiler1_unittest; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/profiler_unittest $(WORK_ROOT)/$(PROJ_NAME)/profiler2_unittest; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/profiler_unittest $(WORK_ROOT)/$(PROJ_NAME)/profiler3_unittest; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/profiler_unittest $(WORK_ROOT)/$(PROJ_NAME)/profiler4_unittest; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/profiledata_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/profile-handler_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/addressmap_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/atomicops_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/current_allocated_bytes_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/debugallocation_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/frag_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/getpc_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/heap-checker_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/heap-profiler_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/large_heap_fragmentation_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/low_level_alloc_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/malloc_extension_c_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/malloc_extension_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/malloc_hook_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/markidle_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/memalign_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/packed-cache_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/page_heap_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/pagemap_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/raw_printer_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/sampler_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/sampling_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/simple_compat_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/stack_trace_table_test $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/stacktrace_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/system-alloc_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/tcmalloc_large_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/tcmalloc_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/thread_dealloc_unittest $(WORK_ROOT)/$(PROJ_NAME)/; \
	cp $(WORK_ROOT)/.snake/dbg/$(PROJ_NAME)/tcmalloc_unittest2 $(WORK_ROOT)/$(PROJ_NAME)/; \

opt:


clean:
	rm -rf profiler1_unittest
	rm -rf profiler2_unittest
	rm -rf profiler3_unittest
	rm -rf profiler4_unittest
	rm -rf profiledata_unittest
	rm -rf profile-handler_unittest
	rm -rf addressmap_unittest
	rm -rf atomicops_unittest
	rm -rf current_allocated_bytes_test
	rm -rf debugallocation_test
	rm -rf frag_unittest
	rm -rf frag_unittest
	rm -rf getpc_test
	rm -rf heap-checker_unittest
	rm -rf heap-profiler_unittest
	rm -rf heap-profiler_unittest
	rm -rf large_heap_fragmentation_unittest
	rm -rf low_level_alloc_unittest
	rm -rf malloc_extension_c_test
	rm -rf malloc_extension_test
	rm -rf malloc_hook_test
	rm -rf markidle_unittest
	rm -rf memalign_unittest
	rm -rf packed-cache_test
	rm -rf page_heap_test
	rm -rf pagemap_unittest
	rm -rf raw_printer_test
	rm -rf sampler_test
	rm -rf sampling_test
	rm -rf simple_compat_test
	rm -rf stack_trace_table_test
	rm -rf stacktrace_unittest
	rm -rf system-alloc_unittest
	rm -rf tcmalloc_large_unittest
	rm -rf tcmalloc_unittest
	rm -rf thread_dealloc_unittest
	rm -rf tcmalloc_unittest2

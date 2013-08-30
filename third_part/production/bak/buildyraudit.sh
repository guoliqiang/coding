#!/bin/sh
g++ -D_GNU_SOURCE -shared -fPIC -O2 .ymake-out/opt/util/production/yraudit.o .ymake-out/opt/util/production/production_safety_checker.o .ymake-out/opt/base/libbase.a -oyraudit.so -ldl

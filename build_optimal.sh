#!/bin/sh
g++ optimal.cpp -std=c++17 -I../lib/or_tools/include/ -I. -Wno-deprecated -DUSE_GLOP -Wl,-rpath,../lib/or_tools/lib -L../lib/or_tools/lib -lortools -o optimal

#!/bin/sh
/usr/bin/c++ -std=c++11 -I ../lib/udgcd -I ../lib/boost_1_76_0 -I../lib/eigen-3.3.9 -o graph_test graph_test.cpp graph.cpp parser.cpp

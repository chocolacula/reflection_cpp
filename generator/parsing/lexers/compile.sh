#!/bin/bash

PWD=`dirname $0`

$PWD/../../RE-flex/bin/reflex $PWD/cpp_source.l −−outfile=$PWD/compiled/cpp_source.yy.cpp −−header-file=$PWD/compiled/cpp_source.yy.h
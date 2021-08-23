#!/bin/bash

PWD=`dirname $0`

$PWD/../../RE-flex/bin/reflex $PWD/cpp_source.l −−outfile=$PWD/compiled/cpp_source.yy.cpp −−header-file=$PWD/compiled/cpp_source.yy.h
$PWD/../../RE-flex/bin/reflex $PWD/json.l −−outfile=$PWD/compiled/json.yy.cpp −−header-file=$PWD/compiled/json.yy.h
$PWD/../../RE-flex/bin/reflex $PWD/yaml.l −−outfile=$PWD/compiled/yaml.yy.cpp −−header-file=$PWD/compiled/yaml.yy.h
#!/bin/bash

PWD=`dirname $0`

rm -rf $PWD/compiled
mkdir $PWD/compiled

$PWD/../../../../../generator/RE-flex/bin/reflex $PWD/json.l −−outfile=$PWD/compiled/lexer_json.yy.h
$PWD/../../../../../generator/RE-flex/bin/reflex $PWD/yaml.l −−outfile=$PWD/compiled/lexer_yaml.yy.h
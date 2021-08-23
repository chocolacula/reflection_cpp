#!/bin/bash

PWD=`dirname $0`

# $PWD/../../../../../generator/RE-flex/bin/reflex $PWD/json.l −−outfile=$PWD/compiled/lexer_json.yy.dec.h −−header-file=$PWD/compiled/lexer_json.yy.h
$PWD/../../../../../generator/RE-flex/bin/reflex $PWD/yaml.l −−outfile=$PWD/compiled/lexer_yaml.yy.h #−−header-file=$PWD/compiled/lexer_yaml.yy.h
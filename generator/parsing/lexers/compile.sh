#!/bin/bash

../../RE-flex/bin/reflex ./cpp_source.l −−outfile=compiled/cpp_source.yy.cpp −−header-file=compiled/cpp_source.yy.h
../../RE-flex/bin/reflex ./json.l −−outfile=compiled/json.yy.cpp −−header-file=compiled/json.yy.h
../../RE-flex/bin/reflex ./yaml.l −−outfile=compiled/yaml.yy.cpp −−header-file=compiled/yaml.yy.h
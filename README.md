# Reflection header only solution for C++

The namespace ```rr``` translates as Required Reflection</br>
It parses C++ sourcecode, reads attributes and generates static classes via template files.

It brings custom <b>RunTime Type Checking</b> for some type agnostic operations.

### Installation

Update all submodules at first</br>
```shell
git submodule update --init --recursive
```

Go to ```generator/RE-flex``` and build <b>RE-flex</b> binary
```shell
./clean.sh
./build.sh
```

Then go to ```generator/parsing/lexers``` and run
```shell
./compile.sh
```

### Examples

Please read ```example/main.cpp``` for an example of use
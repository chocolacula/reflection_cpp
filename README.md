# Required Reflection header only solution for C++

It parses C++ sourcecode, reads attributes and generates static classes via template files.

The main idea is use kinda dynamic typing and <b>Run Time Type Checking</b> for some type agnostic operations.</br>
It makes possible determine a variable type and do right job - print, serialize/deserialize or whatever.

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
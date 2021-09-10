# Easy Reflection solution for C++

<b> Work In Progress </b>

It parses C++ sourcecode for special attributes. In the simplesst situation you only need mark an object by ```ER_REFLECT``` macro. All other work will be done by code generation tool and reflection library.

The main idea is use kinda dynamic typing and <b>Run Time Type Checking</b> for some type agnostic operations, like copying or taking name of a type.</br>
It makes possible determine a variable type and do right job - print, serialize/deserialize or whatever.

### Installation

Update all submodules at first</br>

```shell
git submodule update --init --recursive
```

<b>Note:</b> Unfortunately you have to download and build ```llvm``` project to use code analysis and generation. It appears in submodules list and common submodules update takes a lot of time.





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
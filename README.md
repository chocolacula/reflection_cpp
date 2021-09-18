# Easy Reflection solution for C++

<b> Work In Progress </b>

It parses C++ sourcecode for special attributes. In the simplest situation you only need mark an object by ```ER_REFLECT``` macro.All other work will be done by code generation tool and reflection library.

The main idea is use kinda dynamic typing and <b>Run Time Type Checking</b> for some type agnostic operations, like copying or taking name of a type.<br>
It makes possible determine a variable type and do right job - print, serialize/deserialize or whatever.

## Installation

At first you have to download and compile <b>Clang</b>. The generator as a part of this project uses Clang to analyse C++ source code. You can build Clang [from sources](https://clang.llvm.org/docs/LibASTMatchersTutorial.html), install from <b>vcpkg</b> package manager or from repositories of your distro. Please pay attention to ```generator/CMakeLists.txt```, it uses LLVM and Clang libraries from <b>Manjaro Linux</b> repos, change the file if you use Clang libs from a different source.<br>
<b>Note:</b> Compile Clang from sources takes <b>A LOT</b> of time, literally hours with freezes and fails on average laptop, think about using precompiled libs from your distro.<br>
<b>Note:</b> If you faced with errors like ```stddef.h``` or ```stdarg.h``` not found, check include folders, perhaps you need few symlinks, it's quite old problem and easy to google.

Then update all submodules<br>

```shell
git submodule update --init --recursive
```

To use yaml and json native serialization go to ```RE-flex``` and build <b>RE-flex</b> binary
```shell
./clean.sh
./build.sh
```

## Examples

Please read ```example/main.cpp``` for an example of use
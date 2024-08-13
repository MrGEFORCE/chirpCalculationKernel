# FMCW chirp calculation kernel

- supports multiple programming languages for calculating FMCW radar chirp parameters.
- it is designed as a kernel, meant to be integrated into other software that may have a GUI.
- in addition to performing radar performance calculations, the library also detects and reports invalid settings.
- for high-level languages, this library provides a standard .cfg file API to describe parameters, making it easy to load and save configurations between different languages.
- still in early stage of development...

---
### Environments

1. for python:

- interpreter: python 3.12 (theoretically support any version not earlier than 3.7)
- IDE: PyCharm (recommended)

2. for c & c++:

- builder: cmake 3.28.
- toolchains:
  - MSVC (pending validation)
  - MinGW (recommended, confirmed)
  - clang & llvm (pending validation)
  - GNU (for linux)
- IDE: CLion or VsCode (recommended)

3. for matlab: to do

4. for GoLang: to do


---

### Attentions

0. general
- the .cfg description file is case-sensitive.

1. for python:
- the decorator "dataclass" is used. it's a newly added feature in python 3.7.
- The description of data type in class is only a hint to the IDE, and the actual assignment is likely to change its default type. Because the program has made a clear distinction between float and int, the user needs to pay special attention to the writing of assignment when using it. Among them, the value represented by Scientific notation will be automatically stored in float format, and the value represented by non-Scientific notation needs to be explicitly followed by a decimal point if it is identified as float.

2. for c & c++:
- c version DO NOT provide .cfg API, because it's mainly targeting for embedded development. The development on computer should always use c++ version.
- can be exported as a lib, either in static or dynamic, but need to change CMakeLists.

---
### Acknowledgement and Reference

[c++ lib for parse .cfg file](https://blog.csdn.net/baidu_31872269/article/details/82766200)

---
### Author

Mr_GEFORCE

National Key Laboratory of Antennas and Microwave Technology

mr_geforce@qq.com, mrgeforce514@gmail.com, jychen1@stu.xidian.edu.cn

中国开发者可用中文来联系，但你发issue或pr之类的请用英文，以防别人看不懂。

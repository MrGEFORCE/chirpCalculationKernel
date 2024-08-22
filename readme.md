# FMCW chirp calculation kernel

- Supports multiple programming languages for calculating FMCW radar chirp parameters.
- It is designed as a kernel, meant to be integrated into other software that may have a GUI.
- In addition to performing radar performance calculations, the library also detects and reports invalid settings.
- For high-level languages, this library provides a standard .cfg file API to describe parameters, making it easy to load and save configurations between different languages.
- Still in early stage of development...

---

### Prerequisites

You should be familiar with the basic theory of FMCW radar signal processing, otherwise the code is very hard to understand. Except for this, almost nothing else need to know because this project just do some simple calculation, saving and loading.

---

### Environments

1. for python:
- interpreter: python 3.12 (theoretically support any version not earlier than 3.7)
- IDE: PyCharm (recommended)

2. for c & c++:
- builder: cmake 3.28.
- toolchains:
    - MSVC (confirmed)
    - MinGW (recommended, confirmed)
    - clang & llvm (Apple, confirmed)
    - GNU (for linux)
- IDE: CLion or VsCode (recommended)

3. for C#: to do
- .NET8.0
- IDE: Rider or MSVC (recommended)

4. for MATLAB:
- version: R2023a, no dependency (recommended)

5. for GoLang: to do
-

6. for Rust: to do
-

---

### Instructions and Example

See the file named "example.*" for every language.

---

### Attentions

0. General
- The .cfg description file is case-sensitive.
- There is a certain relationship between bandwidth, ramp time, and slope. Users can choose slope priority or bandwidth priority. The default setting of the program is bandwidth priority, which calculates the slope given the bandwidth.
- Error and waring infos support multiple languages (currently support zh_CN and en_US).
- Language pack folder path and language type must be explicitly initialized (c version uses error index to indicate the error types).

1. For python:
- The decorator "dataclass" is used. it's a newly added feature in python 3.7.
- The description of data type in class is only a hint to the IDE, and the actual assignment is likely to change its default type. Because the program has made a clear distinction between
  float and int, the user needs to pay special attention to the writing of assignment when using it. Among them, the value represented by Scientific notation will be automatically stored in
  float format, and the value represented by non-Scientific notation needs to be explicitly followed by a decimal point if it is identified as float.

2. For c & c++:
- Can be exported as a lib, either in static or dynamic, but need to change CMakeLists. The profiles for executable, static library and dynamic library are all in the CMakeLists. When using, uncomment the chosen one and reload CMakeLists (don't forget this).
- c version DO NOT provide .cfg API, because it's mainly targeting for embedded development. The development on computer should always use c++ version.

3. For c#:
- Dependency: SharpConfig.
- The example is a full project.

4. For MATLAB:
- All the data is hold in a big struct called "chirp parameters handler" (cph).

5. For GoLang:
-

6. For Rust:
-

---

### Acknowledgements and References

[c++ lib for parsing .cfg file](https://blog.csdn.net/baidu_31872269/article/details/82766200)

---

### Author

Mr_GEFORCE

National Key Laboratory of Antennas and Microwave Technology

mr_geforce@qq.com, mrgeforce514@gmail.com, jychen1@stu.xidian.edu.cn

中国开发者可用中文来联系，但你发issue或pr之类的请用英文，以防别人看不懂。

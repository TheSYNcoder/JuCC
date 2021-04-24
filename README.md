# JuCC

(todo: add logo)

-----------------

![build](https://github.com/TheSYNcoder/JuCC/actions/workflows/build_tests.yml/badge.svg?branch=main)
[![codecov](https://codecov.io/gh/TheSYNcoder/JuCC/branch/main/graph/badge.svg?token=1EIRMRKODX)](https://codecov.io/gh/TheSYNcoder/JuCC)

This is the official Jadavpur University Compiler Compiler repository.

## Key Features
* Nothing yet

## Quickstart
The JuCC project is built and tested on **Ubuntu 20.04**.

```
$ git clone https://github.com/TheSYNcoder/JuCC
$ cd JuCC
$ sudo ./script/installation/packages.sh
$ mkdir build
$ cd build
$ cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
$ ninja jucc
$ ./bin/jucc
```

To run the unit tests provided, 

```
$ mkdir build
$ cd build
$ cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
$ ninja
$ ./bin/jucc_test
```

Before pushing or making a pull request ( These tests must pass, compulsory !! )

```
$ ninja
$ ninja check-format
$ ninja check-clang-tidy
$ ninja check-lint
$ ninja test
```

To add a new unit test, make a folder with the same relative path as in the src folder, and define your test. Please refer to [docs](https://github.com/TheSYNcoder/JuCC/tree/main/docs/) for more details about writing tests using the [googletest](https://github.com/google/googletest) framework.


Additional Notes:
- If you know what you're doing, install the prerequisite packages from `./script/installation/packages.sh` manually.


## For Developers

Please see the [docs](https://github.com/TheSYNcoder/JuCC/tree/main/docs/).  

## Contributing

Contributions from everyone are welcome!

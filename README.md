# JuCC

<img src="https://user-images.githubusercontent.com/42897033/116920657-8acdf480-ac70-11eb-8d3f-46cf27fcb1a1.png" alt="JuCC logo" height="200">

-----------------

![build](https://github.com/TheSYNcoder/JuCC/actions/workflows/build_tests.yml/badge.svg?branch=main)
[![codecov](https://codecov.io/gh/TheSYNcoder/JuCC/branch/main/graph/badge.svg?token=1EIRMRKODX)](https://codecov.io/gh/TheSYNcoder/JuCC)

This is the official Jadavpur University Compiler Compiler repository.

## Key Features
* Supports a subset of the C language for now.
* Custom grammar files to easily extend the language.
* LL(1) parsing with panic mode error recovery.
* Generates .json parse tree outputs for easy visualization with [Treant.js](https://fperucic.github.io/treant-js/).
* 100% Open Source (Apache-2.0 License)

## Quickstart
The JuCC project is built and tested on **Ubuntu 20.04**.

```
$ git clone https://github.com/TheSYNcoder/JuCC
$ cd JuCC
$ sudo ./script/installation/packages.sh
$ cd server
$ npm i
$ cd ..
$ mkdir build
$ cd build
$ cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
$ ninja jucc
$ ./bin/jucc -g <grammar_file> -f <input_file> -o <output_json_file>
```

To run the unit tests provided, 

```
$ mkdir build
$ cd build
$ cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
$ ninja
$ ./bin/jucc_test
```

Before pushing or making a pull request  ( The tests must pass, compulsory !! )

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

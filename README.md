# JuCC

(todo: add logo)

-----------------

![build](https://github.com/TheSYNcoder/JuCC/actions/workflows/build_tests.yml/badge.svg?branch=main)
(todo: add codecov status)

This is the official Jadavpur University Compiler Compiler repository.

## Key Features
* Nothing yet

## Quickstart
The JuCC project is built and tested on **Ubuntu 20.04**.

```
git clone https://github.com/TheSYNcoder/JuCC
cd JuCC
sudo ./script/installation/packages.sh
mkdir build
cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
ninja jucc
./bin/jucc
```

Additional Notes:
- If you know what you're doing, install the prerequisite packages from `./script/installation/packages.sh` manually.


## For Developers

Please see the [docs](https://github.com/TheSYNcoder/JuCC/tree/main/docs/).  

## Contributing

Contributions from everyone are welcome!

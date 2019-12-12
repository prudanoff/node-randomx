## node-randomx
NodeJS addon for RandomX https://github.com/tevador/RandomX

RandomX is a blockchain PoW consensus algorithm developed for CPU mining.
This simple code helps you tu use RandomX C++ library in your Nodejs projects. 

Note: it works in "miner" mode with 2Gb RAM by default. You can manually set all flags in `./src/addon.cc` file.

### How to use
Build RandomX from sources:
```
git clone https://github.com/tevador/RandomX.git
cd RandomX
mkdir build && cd build
cmake -DARCH=native ..
make
```
Clone this repo:
```
git clone https://github.com/prudanoff/node-randomx.git
cd node-randomx
```
Put `librandomx.a` next to `index.js` file. Or change library path in `binding.gyp`.

Set your path to randomx.h in `./src/vm.h`:
```
#include "../../src/randomx.h"
``` 

Install node-gyp:
```
npm install -g node-gyp
```
Build addon:
```
npm i
```
That's it! 

###Examples
There are two files:
* [example.js](example.js) - simple usage of RandomX.
* [index.js](index.js) - "PoW-miner" example. You can set `target` which represents how many leading zeroes you want to get in your block's hash.
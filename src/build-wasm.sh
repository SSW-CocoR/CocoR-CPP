#!/bin/sh
#emsdk-env
em++ -Wall -O2 -m32 -fno-rtti -fno-exceptions  *.cpp -o coco-release-emscripten-32.bc
[ -e coco-release-emscripten-32.bc ] && emcc coco-release-emscripten-32.bc -o coco-wasm.html

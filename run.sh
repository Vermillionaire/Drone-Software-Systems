#!/bin/bash

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:libs/libfreenect/build/lib/:libs/seasocks/bin
export LD_LIBRARY_PATH
./drone_camera $@

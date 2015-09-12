#!/bin/bash

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:libs/libfreenect/build/lib/
export LD_LIBRARY_PATH
./drone_camera $@

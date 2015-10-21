#!/bin/bash

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:libs/libfreenect/build/lib/:libs/seasocks/bin:libs/pcl/build/lib
EPIPHANY_HOME=/opt/adapteva/esdk
export EPIPHANY_HOME
export LD_LIBRARY_PATH

./drone_camera $@

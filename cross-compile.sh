#!/bin/bash

scp *.o parallella@parallella:~/Projects/Drone-Software-Systems/obj
scp e_calc.* parallella@parallella:~/Projects/Drone-Software-Systems

ssh parallella@parallella 'export EPIPHANY_HOME=/opt/adapteva/esdk; cd Projects/Drone-Software-Systems; make remote_link'

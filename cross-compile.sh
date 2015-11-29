#!/bin/bash

scp *.o parallella@parallella:~/storage/Projects/Drone-Software-Systems/obj
scp e_calc.* parallella@parallella:~/storage/Projects/Drone-Software-Systems
ssh parallella@parallella 'export EPIPHANY_HOME=/opt/adapteva/esdk; cd storage/Projects/Drone-Software-Systems; make remote_link'


#scp -P 1942 *.o bmv@107.161.21.122:~/drone
#scp -P 1942 e_calc.* bmv@107.161.21.122:~/drone
#ssh -p 1942 bmv@107.161.21.122 'cd drone; scp -P 5555 *.o parallella@localhost:~/Projects/Drone-Software-Systems/obj; scp -P 5555 e_calc.* parallella@localhost:~/Projects/Drone-Software-Systems;'
#ssh -p 1942 bmv@107.161.21.122 'ssh -p 5555 parallella@localhost '"'"'export EPIPHANY_HOME=/opt/adapteva/esdk; cd Projects/Drone-Software-Systems; make remote_link'"'"

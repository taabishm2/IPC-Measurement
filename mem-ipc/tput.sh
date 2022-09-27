#!/bin/bash

run_test () {
    for i in {1..50}
    do
        ./producer ${RETURN_TYPE} ${MSG_SIZE} >> tput_${MSG_SIZE}.txt
    done
}

# 0 for ACK, 1 for Round Trip

RETURN_TYPE=0
MSG_SIZE=$[100*1024*1024]
run_test

RETURN_TYPE=0
MSG_SIZE=$[512*1024*1024]
run_test

RETURN_TYPE=0
MSG_SIZE=$[1024*1024*1024]
run_test

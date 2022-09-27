#!/bin/bash

run_test () {
    echo "RETURN TYPE = ${RETURN_TYPE}"
    echo "MSG_SIZE = ${MSG_SIZE}"
    for i in {1..50}
    do
        ./server ${RETURN_TYPE} ${MSG_SIZE} >> tput_${MSG_SIZE}.txt
    done
}

RETURN_TYPE=0
MSG_SIZE=$[100*1024*1024]
run_test

RETURN_TYPE=0
MSG_SIZE=$[512*1024*1024]
run_test

RETURN_TYPE=0
MSG_SIZE=$[1024*1024*1024]
run_test

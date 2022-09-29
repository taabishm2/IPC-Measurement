#!/bin/bash

rm -rf tcp/*

PORT=34675
run_test () {
    echo "RETURN TYPE = ${RETURN_TYPE}"
    echo "MSG_SIZE = ${MSG_SIZE}"
    for i in {1..50}
    do
        echo $i
        ./server ${MSG_SIZE} ${RETURN_TYPE} ${PORT} &
        sleep 0.01
        ./client ${MSG_SIZE} ${RETURN_TYPE} ${PORT} >> tcp/tput_${MSG_SIZE}.txt
        sleep 0.01
        PORT=$[$PORT + 1]
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

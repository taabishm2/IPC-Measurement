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
        ./client ${MSG_SIZE} ${RETURN_TYPE} ${PORT} >> tcp/${MSG_SIZE}.txt
        sleep 0.01
        PORT=$[$PORT + 1]
    done
}

# 0 for ACK, 1 for Round Trip
RETURN_TYPE=1
MSG_SIZE=$[4]
run_test

RETURN_TYPE=1
MSG_SIZE=$[16]
run_test

RETURN_TYPE=1
MSG_SIZE=$[64]
run_test

RETURN_TYPE=1
MSG_SIZE=$[256]
run_test

RETURN_TYPE=1
MSG_SIZE=$[1024]
run_test

RETURN_TYPE=1
MSG_SIZE=$[4*1024]
run_test

RETURN_TYPE=1
MSG_SIZE=$[16*1024]
run_test

RETURN_TYPE=1
MSG_SIZE=$[256*1024]
run_test

RETURN_TYPE=1
MSG_SIZE=$[512*1024]
run_test


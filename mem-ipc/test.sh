#!/bin/bash

run_test () {
    for i in {1..50}
    do
        ./producer ${RETURN_TYPE} ${MSG_SIZE} >> ${MSG_SIZE}.txt
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

# LATENCY, send 100MB
RETURN_TYPE=0
MSG_SIZE=$[100*1024*1024]
run_test

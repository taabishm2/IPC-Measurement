#!/bin/bash
MSG_SIZE=$[256*1024]

# 0 for ACK, 1 for Round Trip
RETURN_TYPE=0

./producer ${RETURN_TYPE} ${MSG_SIZE}

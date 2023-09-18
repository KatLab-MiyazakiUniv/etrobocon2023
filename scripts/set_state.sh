#!/bin/bash

IP="172.20.1.1:8000"
state=$1

if [ -z ${state} ]; then
    echo "Error: state is NULL"
    exit 1
fi

curl -X POST -d ${state} http://${IP}/robot_info/state

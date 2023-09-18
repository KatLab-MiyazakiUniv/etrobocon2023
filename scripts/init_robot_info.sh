#!/bin/bash

IP="172.20.1.1:8000"
state=$1

curl http://${IP}/robot_info/init

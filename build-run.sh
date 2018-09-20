#!/bin/sh
if [ -z $1 ]
then
    echo "[INFO] Supply tty port please"
    exit 1
fi
platformio run --target upload --upload-port $1
sleep 1
platformio device monitor --port $1 --baud 115200

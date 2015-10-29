#!/bin/bash
export http_proxy="http://webproxy.nordstrom.net:8181"
export https_proxy=${http_proxy}

#terminate any currently running server processes
cd server

#read the currenrly running server process id from pid.txt
SERVER_ID=$(< pid.txt)

kill -9 $SERVER_ID 2>/dev/null

cd ../

open glitch_photo_booth.app

sleep 5
# start the server process in the background and 
# store that process id in a file
# log stderr to stdout
cd server

/usr/local/bin/node server.js >> log.txt 2>&1 &

SERVER_PID=$!
echo $SERVER_PID > pid.txt

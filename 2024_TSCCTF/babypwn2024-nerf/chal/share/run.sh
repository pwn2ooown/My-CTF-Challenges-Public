#!/bin/bash

exec 2>/dev/null
cd /home/chal
python3 -u /home/chal/pow.py
valid_hash=$?
if [ $valid_hash -eq 1 ]; then
    timeout 60 /home/chal/chal
else
    echo 'Proof of work failed! Are you a robot?'
fi

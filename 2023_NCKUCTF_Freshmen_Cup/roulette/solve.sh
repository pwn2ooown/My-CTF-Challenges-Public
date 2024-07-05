#!/bin/bash
for i in `seq 0 36`; do echo $i | ./roulette | grep NCKUCTF; done;

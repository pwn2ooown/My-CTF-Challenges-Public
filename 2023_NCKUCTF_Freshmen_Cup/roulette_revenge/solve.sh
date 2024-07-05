#!/bin/bash
for i in `seq 0 36`; do echo $i | ./roulette_revenge | grep NCKUCTF; done;

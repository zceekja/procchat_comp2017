#!/bin/bash

declare -i x=1

for i in my_tests/test_*.sh
do
	echo  Test $x $i
	./$i
	echo ""
	x=$(( x + 1))
	sleep 0.5
done

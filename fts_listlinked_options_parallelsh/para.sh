#!/bin/bash
i=1
commands=()
while [ $i -le $# ] ; 
do 
	if [ ${!i} != '//' ] ; then
		commands+=("${!i}")
	else 
		commands+=("&")
	fi
	i=`expr $i + 1`
done 
eval ${commands[*]}
wait

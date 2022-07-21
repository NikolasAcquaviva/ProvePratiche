#!/bin/bash
out=$(echo "#include<sys/syscall.h>" | gcc -dD -E -)
syscalls=$(echo "${out}" | head -n 735 | tail -n 347)
#echo "${syscalls}"
if [ -z $1 ] ; then
	echo insert argument
	exit 1
fi
re='^[0-9]+$'
if [[ $1 =~ $re ]] ; then
   echo "${syscalls}" | grep $1 | head -n 1 | cut -d " " -f 2 | sed 's/__NR_//'
else
   echo "${syscalls}" | grep $1 | head -n 1 | cut -d " " -f 3
fi
exit 0

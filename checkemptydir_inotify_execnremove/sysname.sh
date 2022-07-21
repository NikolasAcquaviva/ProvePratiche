#!/bin/bash
syscalls=$(cat /usr/include/x86_64-linux-gnu/asm/unistd_64.h | grep NR)
def="char *syscall_name[] = {"
nlines=$(echo "${syscalls}" | wc -l)

for (( i=0;i<436;i++ )) do
	syscall=$(echo "${syscalls}" | grep $i | head -n 1 | cut -d " " -f 2 | sed 's/__NR_//')
	if [[ $syscall != "" ]]; then 
		if [[ $i -ne 435 ]]; then
			def="$def\"$syscall\"",
		else
			def="$def\"$syscall\"}"
		fi	
	fi
done

echo $def | sed 's/{/{\n  /' | sed 's/,/,\n  /g' | sed 's/}/\n}/'

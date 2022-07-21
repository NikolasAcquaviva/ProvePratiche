#!/bin/bash
list=$(cat /proc/*/status | grep Uid | sort)
lines=$(echo "${list}" | wc -l)
numbers=""
for (( i=1;i<=$lines;i++ )) do
	number=$(echo "${list}" | head -n $i | tail -n 1 \
	       	| sed 's/	/\ /g' | cut -d " " -f 2)
	numbers=$numbers"\n"$number
done
echo -e "${numbers}" | uniq -c

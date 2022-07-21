#!/bin/bash
files=$(find $1 -maxdepth 1 -type f)
max=$(echo "${files}" | wc -l)
matched=()
numGroups=1
for (( i=1; i <= $max; i++ )) do
	equals=($(echo "${files}" | head -n $i | tail -n 1))
	current=$(echo "${files}" | head -n $i | tail -n 1)
	if [[ "${matched[*]}" =~ "${current}" ]]; then
		continue
	fi

	for (( j=$i+1; j <= $max; j++ )) do
		check=$(echo "${files}" | head -n $j | tail -n 1)
		echo testo $current con $check
		diff $current $check > /dev/null
		if [ $? -eq 0 ] ; then
			equals+=($check)
		fi	
	done
	if [ ${#equals[@]} -eq 1 ] ; then 
	       	continue
	fi
	newF="newfile$numGroups"
	cp ${equals[0]} ./${newF}
	for f in "${equals[@]}" 
	do
		matched+=($f)
	        rm $f
		ln ./${newF} $f	
	done
	numGroups=`expr $numGroups + 1`
done

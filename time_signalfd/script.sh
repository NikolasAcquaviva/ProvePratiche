#!/bin/bash
scripts=$(find $1 -type f)
nf=$(echo "${scripts}" | wc -l)
did=()
for (( i=1;i<$nf;i++ )) do
	matched=()
	file=$(echo "${scripts}" | head -n $i | tail -n 1)

	if [[ "${did[*]}" =~ "$file" ]] ; then
		continue;
	fi

	yes=0
	ch=$(cat $file | head -c 2)
	if [ $ch == "#!" ]; then
		first=$(cat $file | head -n 1)
		matched+=($first)
		for (( j=$i+1;j<=$nf;j++ )) do
			file2=$(echo "${scripts}" | head -n $j | tail -n 1)
			first2=$(cat $file2 | head -n 1)
			if [ $first == $first2 ]; then
				matched+=($file2)
				did+=($file2)
			fi
		done
		
			echo "${matched[*]}: $first"
	fi

done

echo "${matched[*]}"

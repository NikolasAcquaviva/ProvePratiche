#!/bin/bash
files=$(find -type f -name "*.*" | sed 's/.\///g')
num=$(echo "${files}" | wc -l)
did=()
for (( i=1; i<$num; i++ )) do
	f=()
	file1=$(echo "${files}" | head -n $i | tail -n 1)
	if [[ "${did[*]}" =~ "$file1" ]]; then
		continue;
	fi
	ext=$(echo "${files}" | head -n $i | tail -n 1 | cut -d "." -f 2)
	f+=($file1)
	for (( j=$i+1; j<=$num; j++ )) do
		ext2=$(echo "${files}" | head -n $j | tail -n 1 | cut -d "." -f 2)
		file2=$(echo "${files}" | head -n $j | tail -n 1)
		if [ $ext == $ext2 ]; then
			f+=($file2)
			did+=($file2)
		fi
	done
	echo "$ext: ${f[*]}"
done
notex=$(find -type f ! -name "*.*" | sed 's/.\///g')
echo -e without extension"\n${notex}"

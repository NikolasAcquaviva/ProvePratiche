#!/bin/bash
first=$1
second=$2
to=$3
checkSame=""
firstfiles=$(find $first -maxdepth 1 -type f | sed 's/.\///g')
secondfiles=$(find $second -maxdepth 1 -type f | sed 's/.\///g')
numfirst=$(echo "${firstfiles}" | wc -l)
numsecond=$(echo "${secondfiles}" | wc -l)
for (( i=1;i<=$numfirst;i++ )) do
	file1=$(echo "${firstfiles}" | head -n $i | tail -n 1)
	path1=$first/$file1
	for (( j=1;j<=$numsecond;j++ )) do
		file2=$(echo "${secondfiles}" | head -n $j | tail -n 1)
		path2=$second/$file2
		if [ $file1 == $file2 ]; then
			date1=$(date -r $path1 "+%s")
			date2=$(date -r $path2 "+%s")
			if [ $date1 -ge $date2 ]; then
				checkSame=$path2
			else
				checkSame=$path1
			fi
		fi
	done
done

for (( i=1;i<=$numfirst;i++ )) do
	file=$first/$(echo "${firstfiles}" | head -n $i | tail -n 1)
	if [ $file != $checkSame ]; then
		mv $file $to
	fi
done
for (( i=1;i<=$numsecond;i++ )) do
        file=$second/$(echo "${secondfiles}" | head -n $i | tail -n 1)
        if [ $file != $checkSame ]; then
                mv $file $to
        fi
done




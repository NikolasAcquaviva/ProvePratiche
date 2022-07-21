#!/bin/bash
cfiles=$(find $1 -type f -name "*.c")
numc=$(echo "${cfiles}" | wc -l)
hfiles=$(find $1 -type f -name "*.h")
numh=$(echo "${hfiles}" | wc -l)
make=$(find $1 -type f -name "[M|m]akefile")
nmake=$(echo "${make}" | wc -l)
tot=0
all=0
for (( i=1; i <= $numc; i++ )) do
	file=$(echo "${cfiles}" | head -n $i | tail -n 1)
	nlines=$(cat $file | wc -l)
	tot=`expr $tot + $nlines`
	echo $file $nlines
done
echo .c totlines: $tot
all=`expr $all + $tot`
tot=0
for (( i=1; i <= $numh; i++ )) do
	file=$(echo "${hfiles}" | head -n $i | tail -n 1)
	nlines=$(cat $file | wc -l)
	tot=`expr $tot + $nlines`
	echo $file $nlines
done
echo .h totlines: $tot
all=`expr $all + $tot`
tot=0
for (( i=1; i <= $nmake; i++ )) do
	file=$(echo "${make}" | head -n $i | tail -n 1)
	nlines=$(cat $file | wc -l)
	tot=`expr $tot + $nlines`
        echo $file $nlines
done
echo make totlines: $tot
all=`expr $all + $tot`
echo 
echo
echo total lines in the whole: $all

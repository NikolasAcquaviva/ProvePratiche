#!/bin/bash
if [ $# != 1 ] ; then
	echo 'usage ./searchln DIR'
	exit -1
fi
inodes=$(find -type f -exec stat {} ";" | grep -G Coll.:\ "\([1-9][0-9]+\|[2-9]\)" | grep Inode | cut -d " " -f 3 | sort -u)
num=$(echo "${inodes}" | wc -l)
i=1
while [ $i -le $num ] ; do
	inode=$(echo "${inodes}" | head -n $i | tail -n 1)
	list=$(find -inum $inode)
	echo $list 
	i=`expr $i + 1`
done

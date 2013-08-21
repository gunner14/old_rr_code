#!/bin/sh
echo ""
echo "----stage3: make service"
echo ""
while read LINE
do
	echo "read: $LINE"
	name=`echo $LINE|awk -F ' ' '{print $2}'`
	output=`echo $LINE|awk -F ' ' '{print $3}'`
	if [ "$name"x != "NULL"x ];then
		echo ">> make $name"
		make $name
		if [ $? -ne 0 ];then
			echo $LINE >> $2
			sed -i -e "/$LINE/d" $1
		else
			if [ "$output"x != "NULL"x ];then
				echo "(stage3 note) to be done: deploy the .so file"
				#scp $output root@10.2.8.45:/opt/XiaoNei/lib64/
				#if [ $? -ne 0 ];then
				#	echo $LINE >> $2
				#	sed -i -e "/$LINE/d" $1
				#fi
			fi
		fi
	fi
done < $1


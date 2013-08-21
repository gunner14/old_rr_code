#!/bin/sh
echo ""
echo "----stage4: run testcase"
echo ""

for i in ./depends/jars/*
do
	export CLASSPATH=$CLASSPATH:$i
done
export CLASSPATH=$CLASSPATH:./OceTest/src/:./OceTest/classes/:./release/oce-dbjavapool.jar:./release/oce-util.jar:./release/oce-adapter.jar

while read LINE
do
	Tester=`echo $LINE|awk -F ' ' '{print $1}'`
        tester=`echo $Tester | tr [A-Z] [a-z]`
	echo ">> javac OceTest/src/com/xiaonei/xce/test/$tester/${Tester}TestSuite.java"
	javac -d OceTest/classes/  OceTest/src/com/xiaonei/xce/test/$tester/${Tester}TestSuite.java
        if [ $? -ne 0 ];then
                echo $LINE >> $2
		sed -i -e "/$LINE/d" $1
        else
		echo ">> java OceTest/classes/com/xiaonei/xce/$tester/${Tester}TestSuite"
		java com.xiaonei.xce.test.$tester.${Tester}TestSuite
	        if [ $? -ne 0 ];then
	                echo $LINE >> $2
			sed -i -e "/$LINE/d" $1
	        fi
	fi
done < $1


#!/bin/bash
#kill掉当前目录下的tomcat
CURDIR=`pwd`
echo "kill $CURDIR/tomcat"
ps aux|grep $CURDIR/tomcat|grep startup|awk '{print $2}'|xargs kill -9

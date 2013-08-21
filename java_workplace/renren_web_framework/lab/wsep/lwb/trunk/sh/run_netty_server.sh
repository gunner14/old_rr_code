#!/bin/sh
export LANG=zh_CN.GBK
export LANGVAR=zh_CN

ulimit -s 128
ulimit -n 8192

_CLASSPATH=$CLASSPATH
_CLASSPATH=$CLASSPATH:"/opt/wsep/WEB-INF/classes/"
for i in /opt/wsep/WEB-INF/lib/*.jar
do
    _CLASSPATH=$_CLASSPATH:"$i"
done

for i in /opt/resin/lib/my*.jar
do
    _CLASSPATH=$_CLASSPATH:"$i"
done

#echo $_CLASSPATH
echo ================================================


_CLASSPATH=bin:$_CLASSPATH

echo "== starting Netty Server =="
/opt/j2sdk/bin/java -Xms4000M -Xmx4000M -server -Xss128k -XX:ThreadStackSize=128  -verbosegc -XX:+PrintGCDetails -XX:+UseParallelGC -XX:+PrintGCTimeStamps -classpath $_CLASSPATH org.jboss.netty.example.http.snoop.HttpServer 80
#!/bin/sh
JAVA_HOME="/opt/j2sdk"
WSEP_PATH="/opt/wsep"
CLASSPATH="."
for i in "$WSEP_PATH/lib"/*.jar
do
    CLASSPATH=$CLASSPATH:"$i"
done

JVM_ARGS="-server -Xms5500M -Xmx5500M -Xmn2000M -Xss128k -XX:PermSize=256m -XX:MaxPermSize=512m -XX:+UseParNewGC -XX:ParallelGCThreads=8 -XX:SurvivorRatio=6 -XX:TargetSurvivorRatio=90 -XX:MaxTenuringThreshold=30 -XX:SoftRefLRUPolicyMSPerMB=0 -XX:+UseAdaptiveSizePolicy -XX:+UseConcMarkSweepGC -XX:+CMSIncrementalMode -XX:+UseCMSCompactAtFullCollection -XX:CMSFullGCsBeforeCompaction=5 -XX:CMSInitiatingOccupancyFraction=55 -XX:CMSIncrementalSafetyFactor=20 -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -Djava.library.path=/opt/resin/libexec:$JAVA_HOME/lib:/usr/lib64"

ulimit -s 128
ulimit -n 8192

echo $CLASSPATH

$JAVA_HOME/bin/java -classpath $CLASSPATH $JVM_ARGS com.xiaonei.click.bootstrap.WsepStarter $WSEP_PATH &
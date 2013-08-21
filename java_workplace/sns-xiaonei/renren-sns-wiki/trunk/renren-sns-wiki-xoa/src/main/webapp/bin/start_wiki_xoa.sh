##############################################################
#           used for build wiki xoa project
#	create by yi.li@renren-inc.com since 2012-05-22
#############################################################
#!/bin/bash

JAVA_HOME=/data/web/jdk

XOA2_SERVER_NAME=xoa2_wiki

XOA2_DEPLOY_HOME=/data/web/xoa-wiki/sns_wiki_xoa

CURRENT_DIR=$XOA2_DEPLOY_HOME/bin

ASPECTJ_JAR=`ls $XOA2_DEPLOY_HOME/WEB-INF/lib | grep aspectjweaver`

ASPECTJ_PATH=$XOA2_DEPLOY_HOME/WEB-INF/lib/$ASPECTJ_JAR

JAVA_OPTS="-server -Xms2048m -Xmx2048m -Xmn512m -XX:PermSize=32m -XX:MaxPermSize=256m -XX:+UseParNewGC -XX:ParallelGCThreads=8 -XX:+UseConcMarkSweepGC -XX:TargetSurvivorRatio=90 -XX:+UseCMSCompactAtFullCollection -XX:CMSFullGCsBeforeCompaction=10 -XX:CMSInitiatingOccupancyFraction=85 -verbose:gc -XX:+PrintGC -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -Xloggc:$CURRENT_DIR/${XOA2_SERVER_NAME}.gc.log -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=$CURRENT_DIR/$XOA2_SERVER_NAME.dump -javaagent:$ASPECTJ_PATH -Drenren.wiki.useSeg"

LOG4J_ARG="-Dlog4j.configuration=file:$XOA2_DEPLOY_HOME/WEB-INF/classes/log4j.xml"

XOA2_SERVER_BOOTSTRAP=com.renren.xoa2.server.Bootstrap

XOA2_SERVER_CONF=$XOA2_DEPLOY_HOME/WEB-INF/xoa.xml

XOA2_SERVER_LOG=$CURRENT_DIR/xoa2.$XOA2_SERVER_NAME.log

XOA2_PID_FILE=$CURRENT_DIR/$XOA2_SERVER_NAME.pid

# compose classpath
for jarDir in $XOA2_DEPLOY_HOME/WEB-INF/lib; do
        for jarFile in `ls $jarDir`; do
                CLASS_PATH=$jarDir/$jarFile:$CLASS_PATH
        done
done
CLASS_PATH=$XOA2_DEPLOY_HOME/WEB-INF/classes:$CLASS_PATH
#echo $CLASS_PATH

"$JAVA_HOME/bin/java" $JAVA_OPTS $LOG4J_ARG -cp $CLASS_PATH $XOA2_SERVER_BOOTSTRAP $XOA2_SERVER_CONF >> $XOA2_SERVER_LOG 2>&1 &

echo "xoa server started. "

#write pid
if [ ! -z "$XOA2_PID_FILE" ] ; then
        echo $! > $XOA2_PID_FILE
fi


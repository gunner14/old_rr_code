#!/bin/bash

JAVA_HOME="/data/web/jdk";
#echo $JAVA_HOME

# must be specified
PROJECT_NAME="renren-sns-wiki-statistic"
EXECUTION_MAIN_CLASS="com.renren.sns.wiki.stat.worker.HotFeedWorker"
#EXECUTION_MAIN_CLASS="com.renren.sns.wiki.stat.worker.UVWorker"
#EXECUTION_MAIN_CLASS="com.renren.sns.wiki.stat.worker.PVWorker"
PROJECT_PATH="/data/web/xiaonei_page_liyi/build/wiki/deploy"

# generated from above configurations
PROJECT_LIB="$PROJECT_PATH/lib"
PROJECT_JAR="$PROJECT_PATH/${PROJECT_NAME}-1.0-SNAPSHOT.jar"
LOG4J_PATH="$PROJECT_PATH/log4j.xml"


CLASS_PATH=$PROJECT_JAR:$CLASS_PATH
for jarFile in `ls $PROJECT_LIB`; do
	CLASS_PATH=$PROJECT_LIB/$jarFile:$CLASS_PATH
done
#echo $CLASS_PATH

GC_LOG_FILE="/data/logs"

# Program arguments
JAVA_OPTS="-Xms1024m -Xmx1024m -XX:MaxPermSize=256m -verbose:gc -XX:+PrintGC -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -Xloggc:$GC_LOG_FILE/${PROJECT_NAME}-gc.log -Dlog4j.configuration=file:$LOG4J_PATH";

LASTHOUR=`date -d '1 hour ago' +%Y%m%d%H`
REMOTE_LOG_FILE="/data/logs/wiki-remote.log.$LASTHOUR"
if [ ! -f $REMOTE_LOG_FILE ];then
	echo `date "+%Y-%m-%d %H:%M:%S"`" file $REMOTE_LOG_FILE does not exists."
	exit 1;
fi

$JAVA_HOME/bin/java $JAVA_OPTS -cp $CLASS_PATH $EXECUTION_MAIN_CLASS $REMOTE_LOG_FILE 

echo `date "+%Y-%m-%d %H:%M:%S"`" statistic started"

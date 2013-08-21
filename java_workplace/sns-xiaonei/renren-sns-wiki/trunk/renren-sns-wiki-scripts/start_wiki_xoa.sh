##############################################################
#           used for build wiki xoa project
#	create by yi.li@renren-inc.com since 2012-05-22
#############################################################
#!/bin/bash

JAVA_HOME=/data/web/jdk

XOA2_SERVER_NAME=xoa2_wiki

XOA2_DEPLOY_HOME=/data/web/deploy/xoa/deploy/sns_wiki_xoa

CURRENT_DIR=`pwd`

JAVA_OPTS="-Xms1024m -Xmx1024m -XX:MaxPermSize=256m -verbose:gc -XX:+PrintGC -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -Xloggc:$CURRENT_DIR/${XOA2_SERVER_NAME}.gc.log"

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
echo $CLASS_PATH

"$JAVA_HOME/bin/java" $JAVA_OPTS $LOG4J_ARG -cp $CLASS_PATH $XOA2_SERVER_BOOTSTRAP $XOA2_SERVER_CONF >> $XOA2_SERVER_LOG 2>&1 &

#write pid
if [ ! -z "$XOA2_PID_FILE" ] ; then
        echo $! > $XOA2_PID_FILE
fi


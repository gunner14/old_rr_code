#!/bin/sh
WORK_DIR="/opt/logserver"
#WORK_DIR="."
#set -x
/opt/j2sdk/bin/java -cp ${WORK_DIR}/log4j-1.2.15.jar org.apache.log4j.net.SocketServer 58778 ${WORK_DIR}/logserver.properties ${WORK_DIR}/ &

SPID=$!
sleep 1
echo $SPID > ${WORK_DIR}/logServer.pid

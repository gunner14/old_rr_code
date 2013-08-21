#!/bin/bash

if [ $# -ne 1 ]
then
  echo "Usage: $0 <nickname>"
  exit 3;
fi

PACKAGE=redis-2.4.6.tar.gz
UNPACKAGE=redis-2.4.6
CONF1=redis_6600.conf
CONF2=redis_6601.conf
CONF3=redis_6602.conf
CONF4=redis_6603.conf
SERVER=$1
ACCT=xce

#****************************************
# Setup StrictHostKeyChecking
#****************************************
ssh -o "StrictHostKeyChecking no" ${ACCT}@${SERVER} "echo Initializing ${SERVER}"

scp ${PACKAGE} ${CONF1} ${CONF2} ${CONF3} ${CONF4} ${ACCT}@${SERVER}:~/
ssh ${ACCT}@${SERVER} "mkdir -p redis/conf; mkdir -p redis/run"
ssh ${ACCT}@${SERVER} "mv ${CONF1} ${CONF2} ${CONF3} ${CONF4} redis/conf"
ssh ${ACCT}@${SERVER} "tar zxf ${PACKAGE}; cd ${UNPACKAGE}; make PREFIX=/data/xce/redis install"
ssh ${ACCT}@${SERVER} "rm -f ${PACKAGE}; rm -rf ${UNPACKAGE}"


./redis_start.sh $1



#!/bin/bash

if [ $# -ne 1 ]
then
  echo "Usage: $0 <nickname>"
  exit 3;
fi

ACCT=xce
SERVER=$1

ssh ${ACCT}@${SERVER} "cd redis/run; ../bin/redis-server ../conf/redis_6600.conf; ../bin/redis-server ../conf/redis_6601.conf; ../bin/redis-server ../conf/redis_6602.conf; ../bin/redis-server ../conf/redis_6603.conf"

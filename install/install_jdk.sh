#!/bin/bash

if [ $# -ne 1 ]
then
  echo "Usage: $0 <nickname>"
  exit 3;
fi

SERVER=$1
ACCT=xce
JDK="jdk-6u20-linux-x64.bin"

scp ${JDK} ${ACCT}@${SERVER}:~/

ssh ${ACCT}@${SERVER} "chmod +x ${JDK}"
ssh ${ACCT}@${SERVER} "echo yes | ./${JDK}"
ssh ${ACCT}@${SERVER} "ln -s jdk1.6.0_20 java"

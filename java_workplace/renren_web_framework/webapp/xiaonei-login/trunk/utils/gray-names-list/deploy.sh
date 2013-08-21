#!/bin/sh

WORK_DIR=""
TARGET_DIR="/data"

cd ${WORK_DIR}

for ip in `cat ./iplist|grep -v "#"`
do
	scp ./username.log root@${ip}:${TARGET_DIR}/
done

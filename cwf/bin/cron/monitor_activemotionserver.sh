#!/bin/bash

count=`ps aux | grep actions/activemotionserver | grep -v debug | grep -v grep | wc -l`

if [ $count -eq 14 ]
then
	exit 0
fi

NICKNAME=`cat /data/cwf/.nickname`
#wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=15801208687&message=dj_${NICKNAME}_server_left_${count}"
cd /data/web/cwf/bin && sh +x restart_cwf >> /data/web/cwf/logs/restart.log
wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=13810091260&message=dj_${NICKNAME}_server_left_${count}"

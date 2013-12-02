#!/bin/bash
#source vars
for i in AdTwS0 AdTwS1 AdTwS2 AdTwS3 AdTwS4
do
  #echo "${ICE_HOME}/bin/icegridadmin --Ice.Config=`pwd`/../etc/admin_tw.cfg -e 'server state $i'"
  ICE_HOME=/opt/Ice-3.3
  twstatus=`${ICE_HOME}/bin/icegridadmin --Ice.Config=/data/xce/Ad/etc/admin_tw.cfg -e "server state $i"`
  twdisable=`echo $twstatus|grep disable|wc -l`
  #echo $twstatus  
  #echo $twinactive  
  message="$i:$twstatus"
  if(($twdisable == 1))
  then
#张平 
    wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=18600883672&message=$message";
#孙致钊  
   # wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=15910650244&message=$message";
    echo `date +%Y/%m/%d\ %H:%M` $message
    ${ICE_HOME}/bin/icegridadmin --Ice.Config=/data/xce/Ad/etc/admin_tw.cfg -e "server enable $i"
  else
    echo `date +%Y/%m/%d\ %H:%M` $message
  fi;
done

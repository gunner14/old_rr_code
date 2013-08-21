#!/bin/sh

cd /opt/logserver

if test $( ps -p `cat logServer.pid`|wc -l ) -eq 1
then
  #not exist
  echo "0"
else
  #exist
  echo "1"
fi

#
#crontab
#
#* */1 * * * test `sh /opt/logserver/checkLogServer.sh` == "0" && sh /opt/logserver/startLogServer.sh

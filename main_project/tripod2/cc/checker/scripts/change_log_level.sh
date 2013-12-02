#!/bin/bash
# $1 old log level
# $2 new log level

old="INFO"
new="DEBUG"

if [ $# -eq 2 ]; then
  old=$1
  new=$2
fi

sed_cmd='sed -i s/"-t 10 -l '$old'"/"-t 10 -l '$new'"/g user_*.checker/user_*.checker.sh'
echo $sed_cmd && eval $sed_cmd
echo "---------------------------"
grep "\-t 10 \-l $new" user_*.checker/user_*.checker.sh

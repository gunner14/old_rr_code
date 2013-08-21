#!/bin/sh

grep -c "\[ERROR\] \[passportLog\]  \[loginHttp\] \[10.3.18.17" /data/resin/logs/passport/passportServer.log | tee $1

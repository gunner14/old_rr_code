#!/bin/sh

grep -c "status:failNotifyGrayUser" /data/resin/logs/passport/passportServer.log | tee $1

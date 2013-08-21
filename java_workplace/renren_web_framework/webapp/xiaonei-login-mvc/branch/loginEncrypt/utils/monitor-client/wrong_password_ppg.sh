#!/bin/sh

grep -c "Wrong_Password_Login" /data/resin/logs/passport/passportServer.log | tee $1

#!/bin/sh


grep -c "status:Invalid_User_Name" /data/resin/logs/passport/passportServer.log | tee $1

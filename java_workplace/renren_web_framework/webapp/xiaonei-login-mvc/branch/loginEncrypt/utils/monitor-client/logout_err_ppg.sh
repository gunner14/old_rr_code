#!/bin/sh

grep -c "Logout_Url_Error" /data/resin/logs/passport/passportServer.log | tee $1

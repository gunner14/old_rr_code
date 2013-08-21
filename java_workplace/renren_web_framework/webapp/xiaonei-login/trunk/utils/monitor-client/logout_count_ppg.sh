#!/bin/sh

grep -cE "(status:Logout_Succ|Logout Succ|status:Logout_Url_Error)" /data/resin/logs/passport/passportServer.log| tee $1

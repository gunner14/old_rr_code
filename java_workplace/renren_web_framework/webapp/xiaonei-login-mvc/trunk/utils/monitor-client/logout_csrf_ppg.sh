#!/bin/sh

grep -cE "(Logout CSRF Error|Logout_CSRF_Error)" /data/resin/logs/passport/passportServer.log | tee $1

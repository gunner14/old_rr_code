#!/bin/sh

set -x

cd /opt/logserver
kill -9 `cat logServer.pid`

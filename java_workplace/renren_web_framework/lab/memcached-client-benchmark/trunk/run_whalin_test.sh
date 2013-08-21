#!/bin/sh
export LANG=zh_CN.GBK
export LANGVAR=zh_CN

echo == Run Test ==
./myjava com.renren.memcached.client.test.whalin.MultiThreadGetTest $1 $2 $3 $4

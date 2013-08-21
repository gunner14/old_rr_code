#!/bin/bash

CURDIR=`pwd`
BUILD_LOG="$CURDIR/build.log"

LOCAL_TRUNK_PATH="/data/wikiweb/trunk"
SVN_TRUNK="http://svn.d.xiaonei.com/sns/xiaonei/renren-sns-wiki/trunk"
TARGET_WWW_PATH="/data/wikiweb/www"

echo "============== svn up file ==================="
if [ ! -d $LOCAL_TRUNK_PATH ]; then
    echo "web trunk not exist, create it: $LOCAL_TRUNK_PATH"
    mkdir -p $LOCAL_TRUNK_PATH
    svn co $SVN_TRUNK $LOCAL_TRUNK_PATH
fi

echo "$LOCAL_TRUNK_PATH && svn up"
cd $LOCAL_TRUNK_PATH && svn up >> $BUILD_LOG 2>&1

echo "========  maven packge ============"
cd $LOCAL_TRUNK_PATH && mvn -U clean package >> $BUILD_LOG 2>&1

echo "============= maven end =============="

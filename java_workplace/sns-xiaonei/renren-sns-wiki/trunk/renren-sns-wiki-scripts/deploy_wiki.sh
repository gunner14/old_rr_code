#!/bin/bash

#替换版本号的脚本
CURDIR=`pwd`

#关闭容器
kill_server(){
    cd $CURDIR && sh kill_resin.sh
}

#开启容器
start_server(){
   sh $CURDIR/resin/bin/resinctl start &
   tail -f /data/logs/wiki-resin-stdout.log $CURDIR/resin/log/jvm*.log
}

kill_server

BUILD_LOG="$CURDIR/build.log"

PREPARE_PATH="$CURDIR/data/deploy/wiki"
LOCAL_TRUNK_PATH="$PREPARE_PATH/trunk"
STATIC_PATH="$PREPARE_PATH/static"
#INC_PATH="$PREPARE_PATH/inc"

SVN_TRUNK="http://svn.d.xiaonei.com/sns/xiaonei/renren-sns-wiki/trunk"
SVN_STATIC="http://svn.d.xiaonei.com/frontend/xn.static"
#SVN_INC="http://svn.d.xiaonei.com/frontend/xn.inc"
#STATIC_NAMES="css csspro js jspro img imgpro m n utm kaixin apps home core n/core n/apps/home"
STATIC_NAMES="css csspro js jspro img imgpro m n utm apps modules"
#STATIC_NAMES="css csspro js jspro img imgpro m n utm kaixin n/core n/apps apps"

TARGET_STATIC_PATH="$CURDIR/static"
TARGET_WWW_PATH="$CURDIR/www"
TARGET_WWW_PATH_OLD="$TARGET_WWW_PATH.old"
TEMP_WEBROOT_DIR="$LOCAL_TRUNK_PATH/renren-sns-wiki-web/target/sns_wiki"

JAVA_PATH="java"
FULL="-U clean"
RSYNC="rsync --include='core' -ztrvlC"

if [ "$1" = "--fast" ]; then
    FULL=""
    echo "running in fast mode"
fi

checkexe(){
    type -P $1 > /dev/null
    if [ $? -ne 0 ]; then
        echo "$1 not found"
        echo "pls set $1 in 'PATH'"
        exit -1
    fi
}


echo "================ deploy start at "`date "+%Y-%m-%d %H:%M:%S"`
echo "================ deploy start at "`date "+%Y-%m-%d %H:%M:%S"` > $BUILD_LOG
echo "================ check enviroment ===================="
checkexe java
checkexe mvn
checkexe svn

echo "================ back up web ========================"
if [ -d $TARGET_WWW_PATH ]; then
    rm -fr $TARGET_WWW_PATH_OLD
    mv $TARGET_WWW_PATH $TARGET_WWW_PATH_OLD
fi
mkdir -p $TARGET_WWW_PATH

checklocal(){
    echo "================ check web source ==================="
    if [ ! -d $LOCAL_TRUNK_PATH ]; then
        echo "web trunk not exist, create it: $LOCAL_TRUNK_PATH"
        mkdir -p $LOCAL_TRUNK_PATH
        svn co $SVN_TRUNK $LOCAL_TRUNK_PATH
    fi

    echo "================ check xn.static source =============="
    if [ ! -d $STATIC_PATH ]; then
        echo "static temp not exist, create it: $STATIC_PATH"
        mkdir -p $STATIC_PATH
        svn co -N $SVN_STATIC $STATIC_PATH
    fi
    svn co -N $SVN_STATIC $STATIC_PATH
    cd $STATIC_PATH
    for i in $STATIC_NAMES
    do
        if [ ! -d $STATIC_PATH/$i ]; then
            echo "check out svn static directory: $i"
            svn co $SVN_STATIC/$i $i
        fi
    done
    
    #TARGET_STATIC_PATH不存在,创建
    if [ ! -d $TARGET_STATIC_PATH ]; then
	mkdir -p $TARGET_STATIC_PATH
    fi
}

checkresult(){
    if [ $1 -ne 0 ]; then
        echo "Build project fail, please contact yi.li."
        echo "log file: $BUILD_LOG"
        exit 1
    fi
}

checklocal

echo "================ svn up file ========================="
echo  "$LOCAL_TRUNK_PATH-> "
cd $LOCAL_TRUNK_PATH && svn up >> $BUILD_LOG 2>&1
echo "$STATIC_PATH && svn up $STATIC_NAMES"
cd $STATIC_PATH && svn up $STATIC_NAMES >> $BUILD_LOG 2>&1

echo "================ rsync static file ==================="
$RSYNC $STATIC_PATH/* $TARGET_STATIC_PATH >> $BUILD_LOG 2>&1

echo "================ prepare web files ===================="
cd $LOCAL_TRUNK_PATH
rm -fr $TEMP_WEBROOT_DIR/WEB-INF/lib
mvn -f pom.xml $FULL package >> $BUILD_LOG 2>&1
#mvn -f pom.xml $FULL compile war:exploded >> $BUILD_LOG 2>&1
ret=$?
echo "================ build-result: $ret ===================="
checkresult $ret

echo "================ replace static ======================"
cp -fr $TEMP_WEBROOT_DIR/* $TARGET_WWW_PATH
#$RSYNC $INC_PATH $TARGET_WWW_PATH >> $BUILD_LOG 2>&1
$JAVA_PATH -DDEBUG=true -jar ${CURDIR}/xiaonei-split-version.jar $STATIC_PATH $TARGET_STATIC_PATH $TARGET_WWW_PATH >> $BUILD_LOG 2>&1
ret=$?
checkresult $ret

echo "================ rsync www static ================================"
#rsync --include='core' -ztrvlC --ignore-existing root@10.3.18.170:/opt/static/ /opt/static/
echo "================ done ================================"
date "+%Y-%m-%d %H:%M:%S"

kill_server
start_server

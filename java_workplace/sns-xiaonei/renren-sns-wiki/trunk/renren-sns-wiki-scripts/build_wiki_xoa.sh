###############################################################
#   used to build wiki project
#   create by yi.li since 2012-04-25
###############################################################
#!/bin/bash

CUR_DIR=`pwd`
BUILD_LOG="$CUR_DIR/build.log"

# must be specified
PROJECT_NAME="renren-sns-wiki-xoa"
BUILD_WORK_PATH="/data/web/xoa-wiki"
SVN_ADDRESS="http://svn.d.xiaonei.com/sns/xiaonei/renren-sns-wiki/trunk"

# generated from above configurations
SVN_LOCAL_PATH="$BUILD_WORK_PATH/svn_temp"
#SVN_LOCAL_LOG4J_PATH="$SVN_LOCAL_PATH/$PROJECT_NAME/src/main/resources/log4j.xml"

TARGET_DEPLOY_PATH="$BUILD_WORK_PATH/sns_wiki_xoa"
TARGET_DEPLOY_PATH_OLD="$TARGET_DEPLOY_PATH.old"
TARGET_COMPILE_PATH="$SVN_LOCAL_PATH/$PROJECT_NAME/target"
TARGET_COMPILE_WAR_PATH="$TARGET_COMPILE_PATH/sns_wiki_xoa"
#TARGET_DEPENDENCY_PATH="$TARGET_COMPILE_PATH/lib"
#TARGET_LIB_PATH="$TARGET_COMPILE_PATH/${PROJECT_NAME}-1.0-SNAPSHOT.jar"

# check if needed exe is available
checkexe(){
	type -P $1 > /dev/null
	if [ $? -ne 0 ]; then
		echo "$1 not found"
		echo "pls set $1 in 'PATH'"
		exit -1
	fi
}

checkresult(){
	if [ $1 -ne 0 ];then
		echo "Build project fail, please read the build.log file, or contact yi.li"
		echo "Build.log: $BUILD_LOG"
		exit 1
	fi
}

checkexe mvn
checkexe svn

echo "================= back up last build ============="
if [ -d $TARGET_DEPLOY_PATH ];then
	echo "========= begin to back ============"
	rm -rf $TARGET_DEPLOY_PATH_OLD
	mv $TARGET_DEPLOY_PATH $TARGET_DEPLOY_PATH_OLD
fi
mkdir -p $TARGET_DEPLOY_PATH

# check svn dir is created
if [ ! -d $SVN_LOCAL_PATH ];then
	echo "svn local path does not exists, create it: $SVN_LOCAL_PATH"
	mkdir -p $SVN_LOCAL_PATH
	svn co $SVN_ADDRESS $SVN_LOCAL_PATH
fi

# update svn files
echo "================== update svn files =============="
cd $SVN_LOCAL_PATH && svn up >> $BUILD_LOG 2>&1

# build project
echo "================== begin to build ================"
mvn -f pom-xoa.xml -U clean package -DskipTests >> $BUILD_LOG 2>&1
ret=$?
checkresult $ret

# copy to deploy path
echo "================= copy to deploy path ============"
rsync -trzvl $TARGET_COMPILE_WAR_PATH/ $TARGET_DEPLOY_PATH
#cp -rf $TARGET_LIB_PATH $TARGET_DEPLOY_PATH
#cp -rf $TARGET_DEPENDENCY_PATH $TARGET_DEPLOY_PATH
#cp -rf $SVN_LOCAL_LOG4J_PATH $TARGET_DEPLOY_PATH

echo `date "+%Y-%m-%d %H:%M:%S"` " build finished deploy path: $TARGET_DEPLOY_PATH"
echo "================= finish build at" `date "+%Y-%m-%d %H:%M:%S"` >> $BUILD_LOG


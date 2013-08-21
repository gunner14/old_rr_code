#!/bin/bash
#部署测试环境脚本,当前目录下存在kill_tomcat.sh
CURDIR=`pwd`
SVN_ADDRESS="http://svn.d.xiaonei.com/sns/xiaonei/renren-sns-wiki/trunk"
SVNDIR=$CURDIR/svn_temp

if [ ! -d $SVNDIR ] ; then
    echo "创建目录$SVNDIR"
    mkdir -p $SVNDIR
    echo "svn co"
    svn co $SVN_ADDRESS $SVNDIR
fi
echo "svn up"
cd $SVNDIR&&svn up

echo "compile"
mvn -f $SVNDIR/pom.xml clean package
#mvn -f $SVNDIR/pom.xml -pl renren-sns-wiki-model,renren-sns-wiki-service clean install && mvni -f $SVNDIR/pom.xml -pl renren-sns-wiki-web clean compile process-classes war:exploded

SUFFIX=`date "+%y%m%d%H%M%S"`
echo "新建目录 $CURDIR/$SUFFIX"
mkdir $CURDIR/www.$SUFFIX



echo "copy project"
cp -rf $SVNDIR/renren-sns-wiki-web/target/sns_wiki/. $CURDIR/www.$SUFFIX/.

echo "stop tomcat"
cd $CURDIR&&sh $CURDIR/kill_tomcat.sh

echo "rm link www"
rm $CURDIR/www
echo "create link www"
ln -s $CURDIR/www.$SUFFIX $CURDIR/www

echo "start tomcat"
sh $CURDIR/tomcat/bin/startup.sh start;tail -f $CURDIR/tomcat/logs/catalina.out






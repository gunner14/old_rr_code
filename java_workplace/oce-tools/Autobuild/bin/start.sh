#!/bin/sh
WORKPATH="/home/autobuild"
MAINLOG="/home/autobuild/log/main.log"


echo "auto build started ..."

. /etc/profile

rm $WORKPATH/log/*
touch $MAINLOG
echo ">> touch $MAINLOG" >> $MAINLOG
touch $WORKPATH/log/stage3.failed
echo ">> touch $WORKPATH/log/stage3.failed" >> $MAINLOG
touch $WORKPATH/log/stage4.failed
echo ">> touch $WORKPATH/log/stage4.failed" >> $MAINLOG
touch $WORKPATH/log/final.success
echo ">> touch $WORKPATH/log/final.success" >> $MAINLOG


cd $WORKPATH
echo ">> cd $WORKPATH" >> $MAINLOG
#:<<BLOCK
. $WORKPATH/bin/stage1.sh $WORKPATH >> $MAINLOG 2>&1 
if [ $? -ne 0 ];then
	. $WORKPATH/bin/mailer.sh $WORKPATH/etc/mail.list $WORKPATH/log/main.log $WORKPATH/log/stage3.failed $WORKPATH/log/stage4.failed $WORKPATH/log/final.success >> $MAINLOG 2>&1
	exit 0
fi
#BLOCK


cd $WORKPATH/trunk/release
echo ">> cd $WORKPATH/trunk/release" >> $MAINLOG
#:<<BLOCK
. $WORKPATH/bin/stage2.sh >> $MAINLOG 2>&1 
if [ $? -ne 0 ];then
	. $WORKPATH/bin/mailer.sh $WORKPATH/etc/mail.list $WORKPATH/log/main.log $WORKPATH/log/stage3.failed $WORKPATH/log/stage4.failed $WORKPATH/log/final.success >> $MAINLOG 2>&1
	exit 0
fi
#BLOCK


cp $WORKPATH/etc/service.list $WORKPATH/log/final.success
echo ">> cp $WORKPATH/etc/service.list $WORKPATH/log/final.success" >> $MAINLOG


#:<<BLOCK
. $WORKPATH/bin/stage3.sh $WORKPATH/log/final.success $WORKPATH/log/stage3.failed >> $MAINLOG 2>&1
#BLOCK


cd $WORKPATH/trunk/
echo ">> cd $WORKPATH/trunk/" >> $MAINLOG
#:<<BLOCK
. $WORKPATH/bin/stage4.sh $WORKPATH/log/final.success $WORKPATH/log/stage4.failed >> $MAINLOG 2>&1
#BLOCK


cd $WORKPATH
echo ">> cd $WORKPATH" >> $MAINLOG
#:<<BLOCK
. $WORKPATH/bin/mailer.sh $WORKPATH/etc/mail.list $WORKPATH/log/main.log $WORKPATH/log/stage3.failed $WORKPATH/log/stage4.failed $WORKPATH/log/final.success >> $MAINLOG 2>&1
#BLOCK


echo "auto build finished !"



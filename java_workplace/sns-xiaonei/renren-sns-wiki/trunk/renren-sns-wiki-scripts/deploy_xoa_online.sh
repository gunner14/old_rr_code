##################################################################
#	use to release a new version of social wiki web
#	created by yi.li@renren-inc.com since 2012-06-19
##################################################################
#!/bin/bash

# make sure the operator know what is she/he doing
echo ""
echo "=================  Welcome!!! This is a social wiki xoa deployment script  ==============="
echo ""

echo -e "Before you continue, I could like to ask if you know what you are doing. \nThis script will upload local tested code to online environment, do you want to continue?(y/n)"

read flag

if [ "$flag" != "y" -a "$flag" != "Y" ] ; then
	echo "WARN exit."
	exit 1
fi

echo -e "Do you use 'screen'(screen can avoid interrupt during network unavailable) to run this script?(y/n)"

read flag
if [ "$flag" != "y" -a "$flag" != "Y" ] ; then
	echo "WARN exit because of not using screen."
	exit 1
fi

# variable definition
REMOTE_HOSTS=`cat ./conf/backendhosts_xoa.txt`
REMOTE_USER=web
DEPLOY_ROOT=/data/web/xoa-wiki
TARGET_WWW_PATH=$DEPLOY_ROOT/sns_wiki_xoa
REMOTE_RESIN_ROOT_DIR=/data/web/web-wiki/resin
XOA_ADMIN_DIR=$DEPLOY_ROOT/xoa_admin

RELEASE_TIME=`date "+%Y%m%d%H%M"`

dateecho() {
	echo [`date`] $1
}

# Begin to rsync local code to remote server

rsync_code() {
	if [ -z $1 ]; then
		dateecho "WARN please input host ip"
		return
	fi
	dateecho "begin to rsync code to $1"
	rsync -trzl $TARGET_WWW_PATH/ $REMOTE_USER@$1:$TARGET_WWW_PATH.$RELEASE_TIME
}

check_connections() {
	if [ -z $1 ]; then
		dateecho "no server defined"
		return 
	fi
	
	dateecho "check connections of server $1"	
	# TODO
	sleep 5
	
}

ping_host() {
	dateecho "$1 waiting xoa server to start. wait 5 seconds"
	sleep 5
	dateecho "$1 xoa server started"
}

replace_web_version() {
	dateecho "$1 replace web with version $TARGET_WWW_PATH.$RELEASE_TIME"
	`ssh $REMOTE_USER@$1 "unlink $TARGET_WWW_PATH; ln -s $TARGET_WWW_PATH.$RELEASE_TIME $TARGET_WWW_PATH"`
	dateecho "$1 replace finished"
}

kill_webserver() {
	dateecho "$1 kill web server"
	dateecho `ssh $REMOTE_USER@$1 "cat $TARGET_WWW_PATH/bin/xoa2_wiki.pid | xargs kill -9 "`	
}

start_webserver() {
	dateecho "$1 restart xoa server"
	dateecho `ssh $REMOTE_USER@$1 "sh $TARGET_WWW_PATH/bin/start_wiki_xoa.sh"`
	dateecho "$1 xoa server is starting"
}

hangup_webserver() {
	echo "do you want to continue hangup xoa server $1? (y/n)"
	#sh hang.sh -u -f $1	
	read hangup_confirm
	if [ $hangup_confirm != 'y' -a $hangup_confirm != 'Y' ]; then
		#statements
		echo "exit process"
		exit 1
	fi

	echo "python2.6 $XOA_ADMIN_DIR/xoa2_admin.py disable social_wiki.sns.xoa.renren.com $1:9100"	
	echo `python2.6 $XOA_ADMIN_DIR/xoa2_admin.py disable social_wiki.sns.xoa.renren.com $1:9100`	
}

hangback_webserver() {
	echo ""
	#sh hang.sh -b -f $1
	echo "python2.6 $XOA_ADMIN_DIR/xoa2_admin.py enable social_wiki.sns.xoa.renren.com $1:9100"
	echo `python2.6 $XOA_ADMIN_DIR/xoa2_admin.py enable social_wiki.sns.xoa.renren.com $1:9100`
}

for REMOTE_HOST in $REMOTE_HOSTS
do
	dateecho "Begin to update server $REMOTE_HOST"	
	hangup_webserver $REMOTE_HOST
	rsync_code $REMOTE_HOST	
	check_connections $REMOTE_HOST
	kill_webserver $REMOTE_HOST
	replace_web_version $REMOTE_HOST
	start_webserver $REMOTE_HOST
	ping_host $REMOTE_HOST
	hangback_webserver $REMOTE_HOST
done

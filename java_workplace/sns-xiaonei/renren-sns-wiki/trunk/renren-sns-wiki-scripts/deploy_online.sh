##################################################################
#	use to release a new version of social wiki web
#	created by yi.li@renren-inc.com since 2012-06-19
##################################################################
#!/bin/bash

# make sure the operator know what is she/he doing
echo ""
echo "=================  Welcome!!! This is a social wiki web deployment script  ==============="
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
REMOTE_HOSTS=`cat ./conf/backendhosts.txt`
REMOTE_USER=web
DEPLOY_ROOT=/data/web/web-wiki
TARGET_WWW_PATH=$DEPLOY_ROOT/www
REMOTE_RESIN_ROOT_DIR=/data/web/web-wiki/resin

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
	
	while true
	do
		CONS=`ssh $REMOTE_USER@$1 "netstat -anp | grep '$1:8080' | wc -l"`
		if [ $CONS -lt 20 ];then
			echo $CONS
			break
		fi
		
		dateecho "connections of $1: $CONS waiting"
		sleep 5;
	done
}

ping_host() {
	dateecho "$1 waiting resin to start"
	while true
	do
		CODE="`curl -o /dev/null -s -w %{http_code}:%{time_connect}:%{time_starttransfer}:%{time_total} -m 5 http://$1:8080/ok.html`"
		TCODE="`echo $CODE | gawk -F: '{print $1}'|egrep -i '2|3[0-9]+'`"
		if [ -z $TCODE ]; then
			TCODE="`echo ${CODE} | gawk -F: '{ print $1}'`"
			dateecho "$1 Http Response Code[${TCODE}],[http_code:time_connect:time_starttransfer:time_total]=[${CODE}] waiting...."
                        sleep 5;
		else
			break
		fi
	done

	dateecho "$1 resin started"
}

replace_web_version() {
	dateecho "$1 replace web with version $TARGET_WWW_PATH.$RELEASE_TIME"
	`ssh $REMOTE_USER@$1 "unlink $TARGET_WWW_PATH; ln -s $TARGET_WWW_PATH.$RELEASE_TIME $TARGET_WWW_PATH"`
	dateecho "$1 replace finished"
}

kill_webserver() {
	dateecho "$1 kill web server"
	dateecho `ssh $REMOTE_USER@$1 "ps -ef | grep $REMOTE_RESIN_ROOT_DIR |awk '"'{print $2}'"'""| xargs kill -9"`	
	#echo `ssh $REMOTE_USER@$1 "ps -ef | grep $REMOTE_RESIN_ROOT_DIR |awk '"'{print $2}'"'"`	
}

start_webserver() {
	dateecho "$1 restart web server"
	dateecho `ssh $REMOTE_USER@$1 "sh $REMOTE_RESIN_ROOT_DIR/bin/resinctl start"`
	dateecho "$1 webserver is starting"
}

hangup_webserver() {
	sh hang.sh -u -f $1	
}

hangback_webserver() {
	sh hang.sh -b -f $1
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

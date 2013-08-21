############################################################################
#    this script used to hang up or hang back a server
#    created by yi.li@renren-inc.com since 2012-06-21
############################################################################
#!/bin/bash

# for debug
#set -x

REOMTE_USER=web

dateecho() {
	echo [`date`] $1
}

TEMP_FILE_DIR=/data/logs
TEMP_NGINX_CONF=$TEMP_FILE_DIR/nginx.conf.tmp
REMOTE_NGINX_CONF=/data/web/nginx/conf/nginx.conf

hangup() {
	if [ -z $1 ]; then
		dateecho "Plz input the host you want to hang up."
		return
	fi

	dateecho "You are going to hang up $1"
	rm -f $TEMP_NGINX_CONF
	cp conf/nginx.conf $TEMP_NGINX_CONF
	sed -i "s/server *$1/#server $1/" $TEMP_NGINX_CONF
	dateecho `scp $TEMP_NGINX_CONF $REOMTE_USER@$1:$REMOTE_NGINX_CONF.tmp`
	dateecho `ssh -t $REOMTE_USER@$1 "mv $REMOTE_NGINX_CONF.tmp $REMOTE_NGINX_CONF; sudo /data/web/nginx/sbin/nginx -s reload"`
	dateecho "hang up $1 finished"
}

hangback() {
	if [ -z $1 ]; then 
		dateecho "Plz input the host you want to hang back"
		return
	fi
	
	dateecho "You are going to hang back $1"
	dateecho `ssh -t $REOMTE_USER@$1 "cat $REMOTE_NGINX_CONF.bak > $REMOTE_NGINX_CONF; sudo /data/web/nginx/sbin/nginx -s reload"`
	dateecho "hang back $1 finished"
}

backupnginx() {
	if [ -z $1 ]; then 
		dateecho "Plz input the host you want to back up"
		return
	fi
	
	dateecho "You are going to back up nginx config of $1"
	dateecho `ssh $REOMTE_USER@$1 "cp $REMOTE_NGINX_CONF $REMOTE_NGINX_CONF.bak"`
	dateecho "back up nginx config of $1 finished"
}

usage() {
	echo "================================================================================"
	echo "Usage: sh hang.up OPTIONS HOSTIP "
	echo "OPTIONS:"
	echo "-u : hang up option. this will hang up a server"
	echo "-b : this will hang back a server"
	echo "-R : this will hang back all server. when this is apply HOSTIP is not needed"
	echo "-bak : this will back up servers nginx.conf file to nginx.conf.bak"
	echo "-f : this means no need to promote confirm dialog."
	echo "================================================================================"
}

HANG_OPTION=""
HANG_FORCE=0
HANG_HOST_IP=""

# options check
if [ $# = 0 ];then
	usage
	exit 1
fi

while [ $1 ]
do
	if [ $1 = "-u" ]; then
		HANG_OPTION="hang_up"
	elif [ $1 = "-b" ]; then
		HANG_OPTION="hang_back"
	elif [ $1 = "-R" ]; then
		HANG_OPTION="hang_back_all"
	elif [ $1 = "-bak" ]; then
		HANG_OPTION="back_up_nginx"
	elif [ $1 = "-f" ]; then
		HANG_FORCE=1
	else
		HANG_HOST_IP=$1
	fi

	shift
done

if [ -z $HANG_OPTION ]; then
	usage
	exit 1
fi

if [ $HANG_OPTION != "hang_back_all" -a $HANG_OPTION != "back_up_nginx" ] && [ -z "$HANG_HOST_IP" ]; then
	usage
	exit 1
fi

echo "you chose $HANG_OPTION $HANG_HOST_IP"

if [ $HANG_FORCE -le 0 ]; then
	dateecho "are you sure to $HANG_OPTION ? (y/n)"
	read flag
	if [ $flag != "y" -a $flag != "Y" ]; then
		dateecho "exit"
		exit 1
	fi
fi
# option check end


# hang up logic 
if [ $HANG_OPTION = "hang_up" ]; then
	hangup $HANG_HOST_IP
fi

# hang back logic
if [ $HANG_OPTION = "hang_back" ]; then
	hangback $HANG_HOST_IP
fi

# hang back all logic
if [ $HANG_OPTION = "hang_back_all" ]; then
	for H_HOST in `cat conf/backendhosts.txt`
	do
		hangback $H_HOST
	done
fi

# back up nginx config
if [ $HANG_OPTION = "back_up_nginx" ]; then
	for H_HOST in `cat conf/backendhosts.txt`
	do
		backupnginx $H_HOST
	done
fi

dateecho "finish process"


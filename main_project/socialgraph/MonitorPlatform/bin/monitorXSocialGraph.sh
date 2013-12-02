#!bin/bash

#/**
#*	@file:			monitorXSocialGraph.sh
#*	@author:		lei.yang3@renren-inc.com
#*	@date:			2012-08-01
#*	@version:		1.0.0.0
#*	@brief:			日志监控模块
#*
#*/


#服务部署路径
moduleLogPath="/data/xce/XSocialGraph/log"
SERVICENAME="xce@sg1.xce.d.xiaonei.com"
SERVICELOGPATH="/data/xce/SGMonitor/MonitorPlatform/log"

function Print()
{
	local tty_mode=$1
	local message="$2"

	local Time=`date "+%Y-%m-%d %H:%M:%S"`
	echo "${LOG_LEVEL_TEXT[$tty_mode]} $Time * $message" >> ${LOG_FILE}
	echo -e "\e[${TTY_MODE_COLOR[$tty_mode]}m${TTY_MODE_TEXT[$tty_mode]} ${message}\e[m"
	return $?

}

function Env()
{
	source $HOME/.bash_profile

	LOG_ERROR=1
	LOG_WARN=2
	LOG_INFO=4
	LOG_DEBUG=8

	LOG_LEVEL_TEXT=(
	[1]="[ERROR]"
	[2]="[WARN]"
	[4]="[INFO]"
	[8]="[DEBUG]"
	)

	TTY_ERROR=1
	TTY_WARN=2
	TTY_INFO=4
	TTY_DEBUG=8

	TTY_MODE_TEXT=(
	[1]="[FAIL]"
	[2]="[PASS]"
	[4]="[INFO]"
	)

	#0	OFF
	#1	高亮显示
	#4	underline
	#5	闪烁
	#7	反白显示
	#8	不可见

	#30 40	黑色
	#31 41	红色
	#32 42	绿色
	#33 43	黄色
	#34 44	蓝色
	#35	45	紫红色
	#36 46	青蓝色
	#37 47	白色
	TTY_MODE_COLOR=(
	[1]="1;31"
	[2]="1;32"
	[4]="0;36"
	[8]="1;33"
	)

}

function Init()
{
	LOCAL_PATH=`dirname $1`
	if [ $LOCAL_PATH == "." ]
	then
		LOG_FILE="../log/monitorLog.log"
		LOG_DIR="../log/"
		MAILERLIST_CONF="../conf/mail.conf"
		MONITOR_XSOCIALGRAPH_CONF="../conf/monitorXSocialGraph.conf"
		ERROR_FILE="../data/XSocialGraphErrFile"
	elif [ $LOCAL_PATH == "bin" ]
	then
		LOG_FILE="./log/monitorLog.log"
		LOG_DIR="./log/"
		MAILERLIST_CONF="../conf/mail.conf"
		MONITOR_XSOCIALGRAPH_CONF="./conf/monitorXSocialGraph.conf"
		ERROR_FILE="./data/XSocialGraphErrFile"
	else
		showHelp
	fi

	source $MAILERLIST_CONF
	Mailer=(${MailerList//,/ })

	#获取服务器上的服务器列表
	ModuleNameList=(`ls ${moduleLogPath}`)
	if [ ${#ModuleNameList[@]} == 0 ]
	then
		Print LOG_ERROR "No Any Service Module in $moduleLogPath."
		exit -1
	fi

	source $MONITOR_XSOCIALGRAPH_CONF
}

function Excute()
{
	for ((i=0; i<${#ModuleNameList[@]}; i++))
	do
		#模块log日志
		modulelog=$moduleLogPath"/"${ModuleNameList[$i]}"/oce_log"
		if [ ! -f $modulelog ]
		then
			flag=`echo "$modulelog" | grep -E "node|IPCollector"`
			if [ "$flag" == "" ]
			then
				echo "The module [${ModuleNameList[$i]}] is inactive." >> $ERROR_FILE
			fi
		else
			log_interval=`expr $service_active_interval/60 | bc`
			logY=`stat $modulelog | grep -E "Modify" | awk '{print $2}'`
			logH=`stat $modulelog | grep -E "Modify" | awk '{print $3}' | cut -d. -f1 | cut -d: -f1`
			logM=`stat $modulelog | grep -E "Modify" | awk '{print $3}' | cut -d. -f1 | cut -d: -f2`
			nowY=`date "+%Y-%m-%d"`
			nowH=`date -d"$log_interval minute ago" "+%H"`
			nowM=`date -d"$log_interval minute ago" "+%M"`

			if [ "$logY" != "$nowY" ]
			then
				echo "The module [${ModuleNameList[$i]}] is inactive." >> $ERROR_FILE
			else
				if [ $logH == $nowH ]
				then
					if [ $logM -lt $nowM ]
					then
						echo "The module [${ModuleNameList[$i]}] is inactive." >> $ERROR_FILE
					fi
				else
					if [ $logH -lt $nowH ]
					then
						echo "The module [${ModuleNameList[$i]}] is inactive." >> $ERROR_FILE
					fi
				fi
			fi
		fi
	done
}

function SendBackLog()
{
	if [ -f $ERROR_FILE ]
	then
		hostname=`hostname`
		ssh ${SERVICENAME} "cd ${SERVICELOGPATH}; mkdir $hostname"
		scp -r $ERROR_FILE ${SERVICENAME}":"${SERVICELOGPATH}"/"${hostname}
		mv $ERROR_FILE $ERROR_FILE"_""`date "+%Y-%m-%d_%H-%M-%S"`"
	fi
}

Main()
{

	#初始化
	Env
	Init "$0"

	while true
	do
		#开始监控日志
		Excute

		#发送报警日志
		SendBackLog

		Print LOG_INFO "monitorXSocialGraph is still running."

		sleep $service_active_interval

	done

}

Main "$@"


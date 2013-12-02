#!bin/bash

SG_STATS_REALTIME="/data/xce/SGProject/data/sg_stats_realtime/hourly/"

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
		DATA_DIR="../data/"
		RESULT_FILE="./data/PVresult"
		MAILERLIST_CONF="../conf/mail.conf"
		MONITOR_PV_CONF="../conf/monitorPV.conf"
	elif [ $LOCAL_PATH == "bin" ]
	then
		LOG_FILE="./log/monitorLog.log"
		DATA_DIR="./data/"
		RESULT_FILE="./data/PVresult"
		MAILERLIST_CONF="./conf/mail.conf"
		MONITOR_PV_CONF="./conf/monitorPV.conf"
	fi

	yesterday=`date -d yesterday "+%Y-%m-%d"`

	if [ ! -f $DATA_DIR"PV" ]
	then
		mkdir -p $DATA_DIR"PV"
	fi

	source $MAILERLIST_CONF
	source $MONITOR_PV_CONF
	Mailer=(${MailerList//,/ })
}

function GetDataFile()
{

	lastfilename=`date -d "-1 day -2 hour" "+%Y-%m-%d-%H"`
	LastFile=$SG_STATS_REALTIME"/position_history."$lastfilename

	nowfilename=`date -d "-2 hour" "+%Y-%m-%d-%H"`
	NowFile=$SG_STATS_REALTIME"/position_history."$nowfilename

	if [ -f $LastFile -a -f $NowFile ]
	then
		cp $LastFile $DATA_DIR"PV" 
		cp $NowFile $DATA_DIR"PV"
	fi
}

function AnalysisFile()
{

	#计算昨天此时刻和今天此时刻的数据对比
	subname=0
	while [ $subname -lt ${#name[@]} ]
	do
		nowline=`cat $NowFile | grep "${name[$subname]}"`
		lastline=`cat $LastFile | grep "${name[$subname]}"`
		if [ ! -n "$nowline" -o ! -n "$lastline" ]
		then
			((subname++))
			continue
		else
			i=0
			while [ $i -lt ${#template[@]} ] 
			do
				if [ ${template[$i]} == "@" ]
				then
					((i++))
					continue
				else
					num=`expr $i + 1`
					nowValue=`echo $nowline | awk '{print $'$num'}'`
					lastValue=`echo $lastline | awk '{print $'$num'}'`
					if [ $nowValue -lt $lastValue ]
					then
						dis=`expr $lastValue - $nowValue`
						percent=`expr $dis*100/$lastValue | bc`
						if [ $percent -gt ${threshold[$subname]} ]
						then
							echo -e "$nowfilename ${name[$subname]}:${chinesename[$subname]} declined $percent% [yesterday:$lastValue,today:$nowValue]" >> $RESULT_FILE"."$nowfilename
						fi
					fi
				fi
				((i++))
			done
		fi
		((subname++))
	done

	if [ -f $RESULT_FILE"."$nowfilename ]
	then
		python $LOCAL_PATH"/"sendmail.py $RESULT_FILE"."$nowfilename "报警邮件：PV监控出现异常;时间[$nowfilename]"
	fi

}

Main()
{

	Env

	Init "$0"

	while true
	do
		GetDataFile

		AnalysisFile

		Print LOG_INFO "monitorPV is still running."

		sleep 3600

	done

}

Main "$@"


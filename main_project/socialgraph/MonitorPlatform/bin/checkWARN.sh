#!bin/bash

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

	LOCAL_PATH=`dirname $0`
	if [ $LOCAL_PATH == "." ]
	then
		LOG_FILE="../log/checkWARN.log"
		SERVICE_CONF="../conf/service_owner.conf"
	elif [ $LOCAL_PATH == "bin" ]
	then
		LOG_FILE="./log/checkWARN.log"
		SERVICE_CONF="./conf/service_owner.conf"
	fi

}

Main()
{

	Env
	Init "$0"

	if [ "$#" -ne 1 ] 
	then
		Print LOG_ERROR "Input Argc Error.[$#]"
		exit -1
	fi

	#检查本地log中是否有报警日志
	while true
	do
		LOG_DIR=$1

		logList=(`ls ${LOG_DIR}`)
		for logdir in ${logList[@]}
		do
			echo "$logdir" | grep -E 'opi' > /dev/null
			if [ $? -eq 0 ]
			then
				for dir in $logdir
				do
					warnlog=(`ls $LOG_DIR/$dir`)
					if [ ${#warnlog[@]} -ne 0 ]
					then
						for file in ${warnlog[@]}
						do
							local Time=`date "+%Y-%m-%d %H:%M:%S"`
							ownerlist=(`cat $SERVICE_CONF | grep "$file" | cut -d= -f2`)
							if [ ${#ownerlist[@]} -ne 0 ]
							then
								echo "麻烦：${ownerlist[@]}  及时处理线上问题." >> $LOG_DIR"/"$dir"/"$file
							fi
							python $LOCAL_PATH"/"sendmail.py $LOG_DIR"/"$dir"/"$file "报警邮件：机器[$dir];模块名[$file];时间[$Time]"
						done
					fi
					
					rm -rf $LOG_DIR"/"$dir
				done
			fi
		done

		Print LOG_INFO "checkWARN is still running."
		sleep 20
	done

}


Main "$@"







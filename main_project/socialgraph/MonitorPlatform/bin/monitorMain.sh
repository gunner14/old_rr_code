#!bin/bash

#/**
#*	@file:			monitorMain.sh
#*	@author:		lei.yang3@renren-inc.com
#*	@date:			2012-07-30
#*	@version:		1.0.0.0
#*	@brief:			监控平台总控模块
#*
#*/

function showHelp()
{
	echo "samples:"
	echo "===================================================================="
	echo "all monitors in monitor/conf/monitor.conf will be deployed."
	echo "sh ./bin/monitorMain.sh					:	in monitor dirctory"
	echo "sh ./monitorMain.sh					:	in monitor/bin dirctory"
	echo "--------------------------------------------------------------------"
	echo "specific monitor in monitor/conf/monitor.conf to deploy."
	echo "@param:		number is in monitor.conf"
	echo "@eg:		number=2 which MonitorName_"2" will be deployed"
	echo "sh ./bin/monitorMain.sh part number			:	in monitor dirctory"
	echo "sh ./monitorMain.sh part number			:	in monitor/bin dirctory"
	echo "===================================================================="

}

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
		[4]="[DEBUG]"
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
		LOG_DIR="../log"
		LOG_FILE="../log/monitorLog.log"
		MONITOR_CONF="../conf/monitor.conf"
	elif [ $LOCAL_PATH == "bin" ]
	then
		LOG_DIR="./log"
		LOG_FILE="./log/monitorLog.log"
		MONITOR_CONF="./conf/monitor.conf"
	else
		showHelp
	fi

	source $MONITOR_CONF

	if [ ! -n "$ModuleNum" ]
	then
		Print LOG_ERROR "No ModuleNum in monitor.conf."
		exit -1
	fi

}

function Excute()
{
	if [ "$#" != 2 ]
	then
		Print LOG_ERROR "file num error."
		return -1
	fi

	filename=$1
	filemode=`echo $1| cut -d. -f2`
	machinename=$2

	if [ $filemode == "sh" ]
	then
		nohup ssh $machinename "cd ~/SGMonitor/MonitorPlatform; svn up; cd ./bin/; nohup sh $filename &" &
	elif [ $filemode == "py" ]
	then
		nohup ssh $machinename "cd ~/SGMonitor/MonitorPlatform; svn up; cd ./bin/; nohup python $filename &" &
	else
		Print LOG_ERROR "invalid file mode---[$filename]"
		exit -1
	fi
}

function ExcuteFull()
{

	for ((i=1; i<=$ModuleNum; i++))
	do
		ModuleName=$(eval echo \$ModuleName_$i)
		tempMachineName=$(eval echo \$MachineNameList_$i)
		MachineNameList=(${tempMachineName//,/ })
		if [ "${#MachineNameList[@]}" == 1 ]
		then
			Excute $ModuleName $tempMachineName
		else
			for machine in ${MachineNameList[@]}
			do
				Excute $ModuleName $machine
			done
		fi
	done

}

function ExcutePart()
{
	for arg in "$@"
	do
		PartModuleName=$(eval echo \$ModuleName_$arg)
		ParttempMachineName=$(eval echo \$MachineNameList_$arg)
		PartMachineNameList=(${ParttempMachineName//,/ })
		if [ "${#PartMachineNameList[@]}" == 1 ]
		then
			Excute $PartModuleName $ParttempMachineName
		else
			for partmachine in ${PartMachineNameList[@]}
			do
				Excute $PartModuleName $partmachine
			done
		fi
	done

}

function ExcuteCmd()
{
	if [ $1 == "delete" ]
	then
		for ((i=1; i<=$ModuleNum; i++))
		do
			ModuleName=$(eval echo \$ModuleName_$i)
			tempMachineName=$(eval echo \$MachineNameList_$i)
			MachineNameList=(${tempMachineName//,/ })
			if [ "${#MachineNameList[@]}" == 1 ]
			then
				templist=(`ssh $tempMachineName "ps -ef | grep -E 'monitorXSocialGraph|monitorSGProject|monitorPV|monitorRcd' | awk '{print $"2"}'"`)
				ssh $tempMachineName "kill -9 ${templist[@]}"
			else
				for machine in ${MachineNameList[@]}
				do
					templist=(`ssh $tempMachineName "ps -ef | grep -E 'monitorXSocialGraph|monitorSGProject|monitorPV|monitorRcd' | awk '{print $"2"}'"`)
					ssh $machine "kill -9 ${templist[@]}"
				done
			fi
		done
	fi
}

Main()
{

	#环境变量加载
	Env 

	#功能初始化
	Init "$0"

	#根据配置去各个机器上部署监控服务
	if [ "$#" == 0 ]
	then
		ExcuteFull
	elif [ "$#" == 1 ]
	then
		ExcuteCmd $1
		exit -1
	elif [ "$#" == 2 ]
	then
		if [ "$1" == "part" ]
		then
			ExcutePart $2
		else
			showHelp
			exit -1
		fi
	else
		showHelp
		exit -1
	fi

	#发送报警日志
	nohup sh $LOCAL_PATH"/"checkWARN.sh $LOG_DIR  &

}

Main "$@"



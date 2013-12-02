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
		RCDMOCK_DIR="."
		RESULT_DIR="./rcdmockresult/"
		MAILERLIST_CONF="../conf/mail.conf"
	elif [ $LOCAL_PATH == "bin" ]
	then
		RCDMOCK_DIR="./bin/"
		RESULT_DIR="./bin/rcdmockresult/"
		MAILERLIST_CONF="../conf/mail.conf"
	fi

	source $MAILERLIST_CONF
	Mailer=(${MailerList//,/ })
}

function StartRcdMock()
{

	cd $RCDMOCK_DIR

	nomhup ./rcdmock_control &

	cd -
}

function checkRcdMockResult()
{

	localtime=`date "+%Y-%m-%d_%H"`
	RESULT_FILE="result_""$localtime"

	if [ -f "$RESULT_DIR$RESULT_FILE" ]
	then
		python $LOCAL_PATH"/"sendmail.py "$RESULT_DIR$RESULT_FILE" "报警邮件：正常服务监控出现异常;时间[$localtime]"
	fi
}

Main()
{

	Env

	Init "$0"

	#启动rcdmock
	StartRcdMock

	while true
	do

		checkRcdMockResult

		sleep 3600

	done

}

Main "$@"


T0=`tail -n200 /data/xce/Ad/log/AdTwS0/oce_log | grep try | awk '{print $1}' | tail -n1`
T1=`tail -n200 /data/xce/Ad/log/AdTwS1/oce_log | grep try | awk '{print $1}' | tail -n1`
T2=`tail -n200 /data/xce/Ad/log/AdTwS2/oce_log | grep try | awk '{print $1}' | tail -n1`
T3=`tail -n200 /data/xce/Ad/log/AdTwS3/oce_log | grep try | awk '{print $1}' | tail -n1`
T4=`tail -n200 /data/xce/Ad/log/AdTwS4/oce_log | grep try | awk '{print $1}' | tail -n1`
sleep 10s
T10=`tail -n200 /data/xce/Ad/log/AdTwS0/oce_log | grep try | awk '{print $1}' | tail -n1`
T11=`tail -n200 /data/xce/Ad/log/AdTwS1/oce_log | grep try | awk '{print $1}' | tail -n1`
T12=`tail -n200 /data/xce/Ad/log/AdTwS2/oce_log | grep try | awk '{print $1}' | tail -n1`
T13=`tail -n200 /data/xce/Ad/log/AdTwS3/oce_log | grep try | awk '{print $1}' | tail -n1`
T14=`tail -n200 /data/xce/Ad/log/AdTwS4/oce_log | grep try | awk '{print $1}' | tail -n1`
if [ "$T0" == "$T10" ]
then
	PID=`ps -ef|grep "AdTwS0"|grep -v grep|head -n1|awk '{print $2}'`
	echo `date +%Y/%m/%d\ %H:%M`" AdTwS0 need kill, pid = $PID"
	kill -9 $PID
fi
if [ $T1 == $T11 ]
then
	PID=`ps -ef|grep "AdTwS1"|grep -v grep|head -n1|awk '{print $2}'`
	echo `date +%Y/%m/%d\ %H:%M`" AdTwS1 need kill, pid = $PID"
	kill -9 $PID
fi
if [ $T2 == $T12 ]
then
	PID=`ps -ef|grep "AdTwS2"|grep -v grep|head -n1|awk '{print $2}'`
	echo `date +%Y/%m/%d\ %H:%M`" AdTwS2 need kill, pid = $PID"
	kill -9 $PID
fi
if [ $T3 == $T13 ]
then
	PID=`ps -ef|grep "AdTwS3"|grep -v grep|head -n1|awk '{print $2}'`
	echo `date +%Y/%m/%d\ %H:%M`" AdTwS3 need kill, pid = $PID"
	kill -9 $PID
fi
if [ $T4 == $T14 ]
then
	PID=`ps -ef|grep "AdTwS4"|grep -v grep|head -n1|awk '{print $2}'`
	echo `date +%Y/%m/%d\ %H:%M`" AdTwS4 need kill, pid = $PID"
	kill -9 $PID
fi
echo `date +%Y/%m/%d\ %H:%M`" check done "

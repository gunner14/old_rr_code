#!/bin/sh

#计算商街指数用到2个临时文件和一个log文件。
#datafile是存储从数据库中抓取到的内容。
#resfile是用来发送邮件时存储邮件内容的文件
#三个文件都要写成绝对路径
#发送邮件时用到python脚本sendmail.py，在本脚本最后调用的时候也要写成绝对路径
#sendmail.py脚本中发件人和收件人需自己指定

datafile="/home/sunxch/Shell/mysql/mysql.txt"
resfile="/home/sunxch/Shell/mysql/result.jsp"
logfile="/home/sunxch/Shell/mysql/log.log"

echo "<table border=1>" > $resfile
echo '<tr><th>时间</th><th>商街指数</th></tr>' >> $resfile

#每天的数据分为两行，30天所以limit是60
#第三列是pv和，第四列是uv
mysql -h10.3.19.142 -uad -padjb### -e "use gj_db;select dt,type2, sum(pv), uv from hot_index group by dt, type2 order by dt desc limit 60;" > $datafile
while [ ! -f $datafile ]
do
sleep 1s
done
#从第2行开始读取文件
linenum=`wc -l $datafile |awk -F' ' '{print $1}'`
line=2;

#每次处理两行，商街指数是第一行的第三个参数 除以 第二行的第四个参数
while [ $linenum -gt $line ]
do
date=`date "+%Y-%m-%d %H:%M:%S"`

time=`awk "NR==$line,NR==$line"' {print $1}' $datafile`
s=`awk "NR==$line,NR==$line"' {print $3}' $datafile`
line=`echo $line+1|bc`
p=`awk "NR==$line,NR==$line"' {print $4}' $datafile`
r=`echo "scale=2; $s/$p" |bc`

#output log
echo "[$date] $time  $s / $p = $r" >> $logfile
echo "<tr><td>$time</td><td>$r</td></tr>"  >> $resfile
line=`echo $line+1|bc`
done

date=`date "+%Y-%m-%d"`
echo "</table>" >> $resfile

#发邮件的python脚本，绝对路径
/home/sunxch/Shell/mysql/sendmail.py $date $resfile
rm $resfile
rm $datafile

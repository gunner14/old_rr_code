#!bin/sh

#if [ $# -lt 3 ];then
#  echo "Usage $0 TotalChangedCount CheckFailedCount ReloadFailedCount"
#  exit
#fi
TmpDir="../tmp"
mkdir -p $TmpDir
HtmlFile="$TmpDir/mail.html"
EmailPropertyFile="$TmpDir/email_properties"
EmailSenderClassPath="/data/xce/XiaoNei/bin/cron/EmailSender.jar"
EmailSenderClass="xce.tools.EmailSender"
Java="/data/xce/java/bin/java"

echo $1
exit

#TotalChangedCount=$1
#CheckFailedCount=$2
#ReloadFailedCount=$3

# send email 
echo "<html> <head> <style>
table { border-collapse:collapse; border:none; }
th { border-top:solid #F79646 1.0pt; border-left:solid #F79646 1.0pt; background: #F79646; }
td { border:solid #F79646 1.0pt; }
</style> </head>
<body>" > $HtmlFile
echo $1 >> $HtmlFile

echo "</body> </html> " >> $HtmlFile


echo "email.from=TripodMonitor@renren-inc.com
email.to=xin.huang@renren-inc.com
email.subject=TripodMonitor
content.type=text/html
content.body=$HtmlFile
" > $EmailPropertyFile

$Java -cp $EmailSenderClassPath $EmailSenderClass $EmailPropertyFile

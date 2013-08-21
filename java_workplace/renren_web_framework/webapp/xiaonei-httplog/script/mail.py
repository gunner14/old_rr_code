import smtplib,email,email.MIMEBase,email.MIMEText
import sys
from email import Utils

message = ""
f = file('list.txt')
while True:
	line = f.readline()
	if(len(line)==0):
		break
	message += line+"</br>"

msg = email.MIMEText.MIMEText(message, _subtype="html", _charset="utf-8")
msg['To']='zhangjie@opi-corp.com,zhenya.hong@opi-corp.com,zhang.zhang@opi-corp.com,shiyang.zhao@opi-corp.com,fusong.li@opi-corp.com,yunlong.bai@opi-corp.com'
msg['From']='zhangjie@opi-corp.com'
msg['Subject']='XiaoNei 404 Top 20 Error List'
msg['Date'] = Utils.formatdate(localtime = 1)

s = smtplib.SMTP('mail.opi-corp.com')
s.sendmail('zhangjie@opi-corp.com',msg['To'],msg.as_string())

#!/usr/bin/python
# -*- coding: utf-8 -*-

import email
import mimetypes
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
from email.MIMEImage import MIMEImage
from email import Utils
import smtplib
import time, datetime


import sys;
python_script = sys.argv[0];

if __name__ == '__main__' :
	''''''
	femail = "xiaochen.sun@renren-inc.com"
#	temail = ["kevin.li@renren-inc.com","xiong.hua@renren-inc.com","xiaodong.yuan@renren-inc.com","zhen.deng@renren-inc.com","jun.xie@renren-inc.com","chong.cheng@renren-inc.com","he.li@renren-inc.com","jinshan.li@renren-inc.com","song.li@renren-inc.com","xiaochen.sun@renren-inc.com","peng.zhang1@renren-inc.com"]
	temail = ["xiong.hua@renren-inc.com","xiaochen.sun@renren-inc.com"]
	date = sys.argv[1]
	filename= sys.argv[2]
	file = open(filename)
	message = file.read()
	msg = email.MIMEText.MIMEText(message, _subtype="html", _charset="utf-8")
	title="[商街指数-"
	msg['Subject'] =title.decode('utf-8') + date +"]"
	msg['To'] = ", ".join(temail)
	msg['From'] = femail
	s = smtplib.SMTP("mail.renren-inc.com")
	s.sendmail(femail, temail, msg.as_string())
	s.close()

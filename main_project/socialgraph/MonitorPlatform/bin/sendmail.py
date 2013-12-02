#/***************************************************************************
# * 
# * Copyright (c) 2012 renren.com, Inc. All Rights Reserved
# * $Id$ 
# * 
# **************************************************************************/
 
 
 
#/**
# * @file sendmail.py
# * @author lei.yang3(lei.yang3@renren-inc.com)
# * @date 2012/08/02 14:12:38
# * @version $Revision$ 
# * @brief 
# *  
# **/

import sys
import os
import subprocess
import email
import smtplib
from email.Header import Header  
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
from email.MIMEImage import MIMEImage

MailList=[]

msgRoot=MIMEMultipart('related')
service='irobot@sg1.xce.d.xiaonei.com'

def LoadConf():

	localpath = os.getcwd()
	localdir=localpath[ localpath.rfind('/') + 1: ]

	if localdir == "bin":
		mailconfpath="../conf/mail.conf"
	elif localdir == "MonitorPlatform":
		mailconfpath="./conf/mail.conf"
	else:
		print "No Mail.conf in " %localdir
		return False

	fd = open(mailconfpath, "r")
	for mail in fd.readlines():
		MailList.append(mail[:mail.find('/\n')])

	fd.close()

	return True

def SendMail( filename, subject ):

	#import pdb;pdb.set_trace()
	msgRoot['Subject'] = Header(subject, 'utf-8')
	msgRoot['From'] = service
	msgRoot['To'] = ";".join(MailList)

	fd = open(filename, "rb")
	msgText=MIMEText(fd.read(), 'plain', 'utf-8')
	msgRoot.attach(msgText)
	fd.close()

	sendhandle = smtplib.SMTP("mail.renren-inc.com")
	sendhandle.sendmail(service, MailList, msgRoot.as_string())
	sendhandle.quit()
	return True

def showHelp():

	print "send specified file with certain subject to mailer whicn configurated in ../conf/mail.conf"
	print "Usage:		python sendmail.py filename subject"
	print "filename:	the file which will be send"
	print "subject:	the subject in mail"

if __name__ == "__main__":

	if len(sys.argv) != 3:
		showHelp()
		sys.exit(-1)
	else:
		filename=sys.argv[1]
		subject=sys.argv[2]

	if LoadConf() is not True:
		print "Load Mailer Failed."
		sys.exit(-1)

	if SendMail( filename, subject ) is not True:
		print "Send Mail to Mailer Failed."
		sys.exit(-1)













#/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

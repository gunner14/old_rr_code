#!/usr/bin/python
# -*- coding: utf-8 -*-

from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
import smtplib
import sys
class Emailer:
	_auth = [
		{"from": "ad.report@renren-inc.com", "user": "ad.report", "server": "10.7.2.18", "port": 587, "password": "AdEngine9999"},
		{"from": "post_master_rr@126.com", "user": "post_master_rr", "server": "smtp.126.com", "port": 587, "password": "12345678"},
		{"from": "boxlee1983@126.com", "user": "boxlee1983", "server": "smtp.126.com", "port": 587, "password": "Asdf1234"},
		{"from": "xj19830801@126.com", "user": "xj19830801", "server": "smtp.126.com", "port": 587, "password": "xj1983"},
		{"from": "xj19830801@163.com", "user": "xj19830801", "server": "smtp.163.com", "port": 587, "password": "xj1983"},
		{"from": "post_master_rr@163.com", "user": "post_master_rr", "server": "smtp.163.com", "port": 587, "password": "xj1983"}
	]

	def sendEmail(self, toAddr, subject, htmlText):
		# 设定root信息
		msgRoot = MIMEMultipart('related')
		msgRoot['Subject'] = subject
		msgRoot['To'] = toAddr
		msgRoot.preamble = 'This is a multi-part message in MIME format.'

		# Encapsulate the plain and HTML versions of the message body in an
		# 'alternative' part, so message agents can decide which they want to display.
		msgAlternative = MIMEMultipart('alternative')
		msgRoot.attach(msgAlternative)

		#设定HTML信息
		msgText = MIMEText(htmlText, 'html', 'utf-8')
		msgAlternative.attach(msgText)

		#发送邮件
		succeed = False
		for authInfo in self._auth:
			smtp = smtplib.SMTP()
			try:
				msgRoot['From'] = authInfo["from"]

				smtp.connect(authInfo["server"], authInfo["port"])
				smtp.login(authInfo["user"], authInfo["password"])
				smtp.sendmail(authInfo["from"], toAddr.rstrip(";").split(";"), msgRoot.as_string())

				succeed = True
				print 'succeed!'
				break
			except Exception, e:
				print >> sys.stderr, 'error sending mail with ' + str(authInfo)
				print >> sys.stderr, 'error info: ' + str(e)
				print >> sys.stderr, 'change to next stmp server'

		if succeed == False:
			print 'tried all smtp servers, none successed'

		return

if __name__ == '__main__' :
	if len(sys.argv) < 4:
		print >> sys.stderr, 'Usage: %s to subject content' % (sys.argv[0])
		sys.exit()

	toAddr = sys.argv[1]
	subject = sys.argv[2]
	htmlText = sys.argv[3]

	emailer = Emailer()
	emailer.sendEmail(toAddr, subject, htmlText)

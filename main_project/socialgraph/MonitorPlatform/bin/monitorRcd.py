#/***************************************************************************
# * 
# * Copyright (c) 2012 RenRen.com, Inc. All Rights Reserved
# * $Id$ 
# * 
# **************************************************************************/
 
 
 
#/**
# * @file monitorRcd.py
# * @author lei.yang3(lei.yang3@renren-inc.com)
# * @date 2012/08/21 10:34:52
# * @version $Revision$ 
# * @brief 
# *  
# **/

import sys
import os
import re
import urllib2
import urllib
import cookielib
import ConfigParser
from HTMLParser import HTMLParser  

class Renren(object):
	
	def __init__(self):
		self.name = self.pwd = self.content = self.domain = self.origURL =  ''
		self.operate = ''
		self._html = ''
		self.cj = cookielib.LWPCookieJar()
		self.opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(self.cj))
		urllib2.install_opener(self.opener)
		self._error = ''
		
	def setinfo(self,username,password,domain,origURL):
		self.name = username
		self.pwd = password
		self.domain = domain
		self.origURL = origURL

	def gethtml(self):
		if len(self._html) == 0:
			self._error = "No No No http response from rcd.renren.com/cwf_nget_home."
		return self._html
	
	def gethtmlpart(self, key):
		#import pdb;pdb.set_trace()
		htmlpart = ''
		firsthtml = self._html.split("##########")[0]
		secondhtml = self._html.split("##########")[1]

		if (key == "recommendFriend"):
			htmlpart = firsthtml
		elif (key == "commonFriend"):
			#import pdb;pdb.set_trace()
			commonnum = 0
			for part in secondhtml.split("<li "):
				if (part.find("RecMixed_cancle") != -1 and part.find("RecMixed_imgToPrf") != -1 and
						part.find("RecMixed_nameToPrf") != -1 and part.find("RecMixed_addFriend") !=
						-1 and
						part.find("RecMixed_info") != -1 and part.find("RecMixed_show") != -1 and
						part.find("RecMixed_addFriendEnd") != -1):
					htmlpart += "<li "
					htmlpart += part
					commonnum += 1
			#print commonnum
					
		elif (key == "app"):
			appnum = 0
			for part in secondhtml.split("<li "):
				if (part.find("RCDAPP") != -1):
					htmlpart += "<li "
					htmlpart += part
					appnum += 1
			#print appnum

		elif (key == "forum"):
			forumnum = 0
			for part in secondhtml.split("<li "):
				if (part.find("RCDFORUM") != -1):
					htmlpart += "<li "
					htmlpart += part
					forumnum += 1
			#print forumnum

		elif (key == "renrendest"):
			destnum = 0
			for part in secondhtml.split("<li "):
				if (part.find("RCDDESK") != -1):
					htmlpart += "<li "
					htmlpart += part
					destnum += 1
			#print destnum

		elif (key == "page"):
			pagenum = 0
			for part in secondhtml.split("<li "):
				if (part.find("PAGE") != -1):
					htmlpart += "<li "
					htmlpart +=part
					pagenum += 1
			#print pagenum

		else:
			print "unvalid key %s" %(key)

		return htmlpart

	def login(self):
		params = {'domain':self.domain,'origURL':self.origURL,'email':self.name, 'password':self.pwd}
		req = urllib2.Request('http://www.renren.com/PLogin.do', urllib.urlencode(params))
		self.operate = self.opener.open(req) 

		print self.operate.geturl()
		if self.operate.geturl() == 'http://rcd.renren.com/cwf_nget_home': 
			self.cj.save('renren.coockie')
			self.__viewnewinfo()
		else:
			print 'Logged on error'
			
	def __viewnewinfo(self):
		self.__caiinfo()
       
	def __caiinfo(self):
		infocontent = self.operate.readlines()
		for i in infocontent:
			self._html += i

	def cleanhtml(self):
		self._html = ''

	def geterror(self):
		return self._error

	def check(self):

		if (len(self._error) != 0):
			return -1
		else:
			return 0

	def genErrFile(self,file):
		fd = open(file, 'a')
		fd.write(repr(self._error) + "\n")
		fd.close()

class configure(object):

	def __init__(self):
		self._conf = ConfigParser.ConfigParser()
		self._confpath = ''
		self._domain = ''
		self._origUrl = ''
		self._user_num = 0
		self._user_password_dict = {}
		self._template_num = 0
		self._template_path = {}
		self._error_file = ''

	def loadconf(self, confpath):
		self._conf.readfp( open(confpath, "r") )

	def getconf(self, confname, itemname):
		return self._conf.get( confname, itemname )

	def getconfdict(self, confname, itemname, num = "0"):
		confdict = {}
		for i in range (1, int(num)+1):
			temp = itemname + repr(i)
			name = self.getconf( confname, temp ) 
			confdict[ name[:name.find(" ")] ] = name[ name.find(" ") + 1:]
		
		return confdict

	def getconflist(self, confname, itemname, num = "0"):
		conflist = []
		for i in range (1, int(num) + 1):
			temp = itemname + repr(i)
			template = self.getconf( confname, temp )
			conflist.append(template)
		
		return conflist

	def start(self, confpath):
		self.loadconf(confpath)
		self._domain = self.getconf( "site", "domain" )
		self._origUrl = self.getconf( "site", "origUrl" )
		self._user_num = self.getconf( "info", "USER_NUM" )
		self._template_num = self.getconf( "template", "TEMPLATE_NUM" )
		self._user_password_dict = self.getconfdict( "info", "user", self._user_num )
		self._template_path = self.getconfdict( "template", "templatepath", self._template_num )
		self._error_file = self.getconf( "error", "errorfile" )

class htmlParser(HTMLParser):

	#self._template = {key:[[],[],[]]}
	def __init__(self):
		HTMLParser.__init__(self)  
		self._template = {}
		self._error = {}

	def _getValue(self, line):

		result = {}
		list = line.split(" ")
		for i in range (0, len(list)):
			srcpost = list[i].find("=")
			destpost = list[i].find(">")
			if (srcpost != -1) and (destpost != -1) :
				result[list[i][:srcpost]] = list[i][srcpost+1:destpost]
			elif (srcpost != -1) and (destpost == -1):
				result[list[i][:srcpost]] = list[i][srcpost+1:]
			else:
				continue
		return result

	def _getKey(self, line):

		list = line.split(" ")
		for i in range(0, len(list)):
			pos = list[i].find("<")
			if (pos != -1):
				return list[i][pos + 1:]
			else:
				continue

	def deleterepeat(self, newdict, oldlist):
		#oldlist: [{},{},{}]
		for itemdict in oldlist:
			if (cmp(newdict, itemdict) == 0):
				return 0

		return -1

	def loadtemplate(self, templatepath):

		if (len(templatepath) == 0):
			print "template path is error."
			return

		fd = open(templatepath, "r")
		for line in fd.readlines():
			if ((line.find("*") != -1) and (line.find("<") != -1) and (line.find(">") != -1)):
				key = self._getKey(line)
				subvalue = self._getValue(line)
				
				if key in self._template.keys():
					if (self.deleterepeat(subvalue, self._template[key]) == 0):
						continue
					else:
						self._template[key].append(subvalue)
				else:
					value = []
					value.append(subvalue)
					self._template[key] = value
			else:
				pass

		#print self._template
		fd.close()
		
	def checkitem(self, tag, actuallist, template):						#checkitem([(),(),(),...], {k:v,k:v,....})

		#import pdb;pdb.set_trace()
		actualdict = {}
		templatedict = {}
		for key in template.keys():
			templatedict[key] = template[key]

		for i in range (0, len(actuallist)):
			actualdict[ actuallist[i][0] ] = actuallist[i][1]

		for key in actualdict.keys():
			if (key in templatedict.keys()):
				if (templatedict[key] == "*"):
					templatedict[key] = actualdict[key]
			else:
				return -1

		for key in actualdict.keys():
			if (templatedict[key] != actualdict[key]):
				#import pdb;pdb.set_trace()
				if tag not in self._error.keys():
					attrslist = []
					attrslist.append(actuallist)
					self._error[tag] = attrslist
				else:
					self._error[tag].append(actuallist)

				return -1

		return 0

	def handle_starttag(self, tag, attrs):

		#import pdb;pdb.set_trace()
		if (len(attrs) == 0):
			return

		if tag in self._template.keys():					#{key:[{key:value},{key:valeu},...]}
			if (len(attrs) == 1):
				for name,value in attrs:
					if(value == ''):
						return
			else:
				attrslen = len(attrs)
				templatelenlist = []
				for item in self._template[tag]:
					templatelenlist.append( len(item) )

				error_num = 0
				for i in range (0, len(templatelenlist)):
					if (attrslen != templatelenlist[i]):
						error_num += 1
					else:
						if (self.checkitem(tag, attrs, self._template[tag][i]) == 0):
							break
						else:
							error_num += 1

				if (error_num == len(templatelenlist)):
					#import pdb;pdb.set_trace()
					if tag not in self._error.keys():
						attrslist = []
						attrslist.append(attrs)
						self._error[tag] = attrslist
					else:
						self._error[tag].append(attrs)
		else:
			#import pdb;pdb.set_trace()
			if tag not in self._error.keys():
				attrslist = []
				attrslist.append(attrs)
				self._error[tag] = attrslist
			else:
				self._error[tag].append(attrs)

	def check(self):

		if (len(self._error) != 0):
			return -1
		else:
			return 0

	def cleantemplate(self):
		self._template.clear()

	def geterror(self):
		return self._error

	def genErrFile(self, user, type, file):

		fd = open(file, 'a')
		fd.write( repr("UserMail : " + user) + '\n' )
		fd.write( repr("RecommendType : " + type) + '\n')

		for key in self._error.keys():
			fd.write( "{ " + repr(key) + '\n')
			for item in self._error[key]:
				#import pdb;pdb.set_trace()
				for name, value in item:
					fd.write( repr(name) + "=" + repr(value) + " " )
				fd.write( " " + '\n' )

		fd.write('}\n\n')
		fd.close()

def showHelp():
	print "spider for rcd.renren.com with specify email and password"
	print "Usage:		python monitorRcd.py -c confpath"
	print "confpaht:	conf will be loaded"

if __name__ == "__main__":

	if (len(sys.argv) != 3):
		showHelp()
		sys.exit(-1)
	else:
		flag = sys.argv[1]
		confpath = sys.argv[2]	
	if (flag != "-c"):
		showHelp()
		sys.exit(-1)

	conf = configure()
	conf.start(confpath)

	for username in conf._user_password_dict.keys():
		ren = Renren()
		ren.setinfo(username, conf._user_password_dict[username], conf._domain, conf._origUrl)
		ren.login()
		html = ren.gethtml()
		if(ren.check() == -1):
			ren.genErrFile(conf._error_file)
		else:
			template = htmlParser()
			for key in conf._template_path.keys():
				#print conf._template_path[key]
				htmlpart = ren.gethtmlpart(key)
				#print htmlpart
				template.loadtemplate( conf._template_path[key] )
				template.feed(repr(htmlpart))
				template.close()
				
				if(template.check() == -1):
					template.genErrFile(username, key, conf._error_file)
				else:
					#print ("rcd web same with template for %s" %(username))
					pass
				
				template.cleantemplate()

		ren.cleanhtml()

	
























#/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */

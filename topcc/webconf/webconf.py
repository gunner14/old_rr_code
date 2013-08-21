#!/usr/bin/env python
#
# Copyright 2009 Facebook
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License. You may obtain
# a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.

import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web
import tornado.auth
import re, md5
import tornado.database
from tornado.options import define, options
import os.path, os
import sys
import tarfile
import datetime
import smtplib
from email.mime.text import MIMEText

reload(sys)
sys.setdefaultencoding('utf-8')

# database settings
define("port", default=9527, help="run on the given port", type=int)
define("mysql_host", default="10.7.18.21:3306", help="web database host")
define("mysql_database", default="web", help="web database name")
define("mysql_user", default="root", help="web database user")
define("mysql_password", default="", help="web database password")

settings = {"login_url":"/login", }

FROMADDR = "photo@renren.com"
PASSWORD = "photo_sparta_1207"
FIX_TOADDRS = ("qinghui.liu@renren-inc.com",
               "huabin.zheng@renren-inc.com",
               "jili.chen@renren-inc.com",
               )

def SendEmail(fromaddr, password, toaddrs, msg, subject="no title"):
    """
    Send email util function
    @param toaddrs: list of recipents 
    """
    mail = MIMEText(msg, _charset='utf-8')
    mail['Subject'] = subject
    mail['From'] = fromaddr 
    mail['To'] = ";".join(toaddrs)
    smtp_host = 'smtp.' + fromaddr.split('@')[1]
    smtp = smtplib.SMTP(smtp_host)
    #smtp.set_debuglevel(1)
    smtp.login(fromaddr, password)
    smtp.sendmail(fromaddr, toaddrs, mail.as_string())
    smtp.quit()
    
def UpdateTestEnvironment(db, operator):
    """
    Generate upload configure files, namely service.conf and template.conf, from
    the database storage. Then restart the upload.fcgi to reload new configures 
    """
    template = "template.conf" 
    service = "service.conf" 
    conf_generator = ConfigFileGenerator(db, operator) 
    conf_generator.gen_config_files(template, service) 
    os.system('restart_upload_test')


class Application(tornado.web.Application):
    def __init__(self):
        handlers = [
            (r"/", MainHandler),
            (r"/registry", RegistryHandler),
            (r"/login", LoginHandler),
            (r"/service", ServiceHandler),
            (r"/logout", LogoutHandler),
            (r"/genconfs", GenConfsHandler),
            (r"/import", ImportHandler),
            (r"/view", ViewHandler),
            (r"/delete", DeleteHandler),
        ]
        settings = dict(
            login_url="/login",
            template_path=os.path.join(os.path.dirname(__file__), "templates"),
            static_path=os.path.join(os.path.dirname(__file__), "static"),
            cookie_secret="11oETzKXQAGaYdkL5gEmGeJJFuYh7EQnp2XdTP1o/Vo=",
            web_title="Upload Service Apply",
        )
        tornado.web.Application.__init__(self, handlers, **settings)

        self.db = tornado.database.Connection(
            host=options.mysql_host, database=options.mysql_database,
            user=options.mysql_user, password=options.mysql_password)

class BaseHandler(tornado.web.RequestHandler):
    @property
    def db(self):
       return self.application.db

    def get_current_user(self):
        return  self.get_secure_cookie("username")
    
#       if not user_name: return None
#       return self.db.get("SELECT * FROM authors WHERE name = %s", user_name)

    def handleerror(self, template, title, errmsg):
        self.render(template, title=title, errmsg=errmsg)
        
    def _add_service(self, service):
        print('***************************************************')
        print(service)
        print('***************************************************')
        self.db.execute("INSERT INTO services (user_id, channel_id, name, " 
        "servicecomment, watermark, markpath, savesubdir, handleexif, "
        "checkcomment, outputtype_id, tickettype_id, ticketsuffix, autorotate, "
        "domain) VALUES (%s, %s, %s, %s, %s, %s, %s, %s,%s, %s, %s, %s, %s, %s)",
        service['userid'], service['channelid'], service['name'],
        service['servicecomment'], service['watermark'], service['markpath'],
        service['savesubdir'], service['handleexif'], service['checkcomment'],
        service['outputtypeid'], service['tickettypeid'],
        service['ticketsuffix'], service['autorotate'], service['domain'])
        
    def _add_channel(self, channel):
        print('***************************************************')
        print(channel)
        print('***************************************************')
        self.db.execute("INSERT INTO channels (name, savegroup_id, "
        "handleimages, minwidth, minheight, maxfilesize, exts) VALUES (%s, %s, " 
        "%s, %s, %s, %s, %s)", channel['name'], channel['savegroupid'],
                        channel['handleimages'], channel['minwidth'],
                        channel['minheight'], channel['maxfilesize'],
                        channel['exts'])
     

class MainHandler(BaseHandler):
    @tornado.web.authenticated
    def get(self):
        services = dict()
        isadmin = self.get_secure_cookie('privilege') != '0'
        if isadmin:
            query = "SELECT * FROM services"
        else:
            query = "SELECT * FROM services WHERE user_id = %s" % (self.get_secure_cookie("userid"),)
            
        items = self.db.iter(query)
        for item in items:
            services[item.id] = item.name
        self.render("myservice.html", title="Services", services=services, isadmin=isadmin) 

class LoginHandler(BaseHandler):
    def get(self):
        self.render("login.html", title="Login", errmsg=None)
    def post(self):
        username = self.get_argument("username")
        if not username:
            errmsg = "Username is required"
            return self.handleerror("login.html", "Login", errmsg)
        password = self.get_argument("password")
        m = md5.new()
        m.update(password + RegistryHandler.password_salt)
        password = m.hexdigest()
        item = self.db.get("SELECT * FROM users WHERE username = %s LIMIT 1", username)
        if not item or item.password != password:
            errmsg = "Username or password error"
            return self.handleerror("login.html", "Login", errmsg)
        self.set_secure_cookie("username", item.username)
        self.set_secure_cookie("userid", str(item.id))
        self.set_secure_cookie("name", item.name)
        self.set_secure_cookie("privilege", str(item.privilege))
        self.redirect("/")
        
class LogoutHandler(BaseHandler):
    @tornado.web.authenticated
    def get(self):
        self.clear_cookie("username")
        self.clear_cookie("userid")
        self.clear_cookie("name")
        self.clear_cookie("privilege")
        self.redirect(self.get_argument("next", "/"))
        
class RegistryHandler(BaseHandler):
    password_salt = "bfmgfs"
    def get(self):
        self.render("registry.html", title="Registry", errmsg=None)
    def post(self):
        username = self.get_argument("username")
        item = self.db.get("SELECT username FROM users WHERE username = %s", username)
        if item:
            errmsg = "User %s has already exited" % username
            return self.handleerror("registry.html", "Registry", errmsg)
        password = self.get_argument("password")
        m = md5.new()
        m.update(password + RegistryHandler.password_salt)
        password = m.hexdigest()
        
        name = self.get_argument("name")
        department = self.get_argument('department')
        email = self.get_argument("email")
        self.db.execute('INSERT INTO users (username, password, name, department, email)' 
        'VALUES (%s, %s, %s, %s, %s)', username, password, name, department, email)
        self.redirect('/')

        
class ServiceHandler(BaseHandler):
    ticket_suffix_salt = "tjjtds"
    @tornado.web.authenticated
    def get(self):
        self.render("service.html", title="New Service", errmsg=None)
    @tornado.web.authenticated
    def post(self):
        result = self.check_args()
        if not result[0]:
            return self.handleerror("service.html", "New Service", result[1])
        service = dict()
        service['name'] = self.get_argument("pagetype")
        service['servicecomment'] = self.get_argument("servicecomment")
        service['userid'] = self.get_secure_cookie('userid')
        service['channelid'] = self._get_channelid()
        service['watermark'] = (self.get_argument("watermark", "") == "on") and 1 or 0
        service['markpath'] = self.get_argument("markpath", "")
        service['savesubdir'] = self.get_argument("savesubdir", "")
        if not service['savesubdir']:
            uploadtype = self.get_argument("uploadtype", "")
            if uploadtype == 'music':
                service['savesubdir'] = 'audio'
            elif uploadtype == "attach":
                service['savesubdir'] = 'attachment'
                
        service['handleexif'] = (self.get_argument("handleexif", "") == "on") and 1 or 0
        service['checkcomment'] = (self.get_argument("checkcomment", "") == "on") and 1 or 0
        service['autorotate'] = (self.get_argument("autorotate", "") == "on") and 1 or 0
        service['outputtypeid'] = self._get_outputtypeid()
        service['tickettypeid'] = self._get_tickettypeid()
        service['domain'] = self.get_argument('domain', '')
	
        m = md5.new()
        m.update(service['name'] + ServiceHandler.ticket_suffix_salt) 
        service['ticketsuffix'] = "ugc.renren.com_" + m.hexdigest()[:20]
        self._add_service(service)
        operator = self.get_secure_cookie('name')
        UpdateTestEnvironment(self.db, operator)
        self._sendemail(service)
        self.redirect('/')
        
    def _sendemail(self, service):
        mail_title = "Upload service created: %s" % service['name']
        operator = self.get_secure_cookie('name')
        msg = "%s create new upload service: %s\n" % (operator, service['name'])
        msg += "applicant: %s\n" % operator
        item = self.db.get('SELECT * FROM users WHERE id = %s', self.get_secure_cookie('userid'))
        owner_email = item.email
        msg += "department: %s\n" % item.department
        msg += "\n"
        msg += "pagetype: %s\n" % service['name']
        msg += "servicecomment: %s\n" % service['servicecomment']
        ticket_type = self.get_argument("ticketselect", "none")
        msg += "ticket_type: %s\n" % ticket_type
        msg += "ticket_suffix: %s\n" % service['ticketsuffix']
        output_type = self.get_argument('outputselect', 'json')
        msg += "output_type: %s\n" % output_type
        item = self.db.get("SELECT * FROM channels WHERE id = %s", service['channelid'])
        msg += "handle_image: %s\n" % item.handleimages
        msg += '\n'
        msg += "test server: 10.7.18.21 upload.renren.com" 
        
        print(msg)
        toaddrs = FIX_TOADDRS + (owner_email,)
        SendEmail(FROMADDR, PASSWORD, toaddrs, msg, mail_title)
        
        
    def _get_channelid(self):
        uploadtype = self.get_argument("uploadtype", "")
        if uploadtype == 'music':
            item = self.db.get('SELECT id FROM channels WHERE name = %s', 'music')
            return item.id
        elif uploadtype == "attach":
            item = self.db.get('SELECT id FROM channels WHERE name = %s', 'attachment')
            return item.id
            
        channel = dict()
        channel['handleimages'] = self._get_handleimages()
        channel['minwidth'] = self.get_argument("minwidth", "1")
        channel['minheight'] = self.get_argument("minheight", "1")
        channel['maxfilesize'] = self.get_argument("maxfilesize", "15")
        channel['exts'] = self._get_exts()
             
        savegroupid = 0 # 0: common server, 1: head server

        item = self.db.get("SELECT * FROM channels WHERE savegroup_id = %s " 
        "and handleimages = %s and minwidth = %s and minheight = %s " 
        "and maxfilesize = %s and exts = %s LIMIT 1", savegroupid, channel['handleimages'],
        channel['minwidth'] , channel['minheight'] , channel['maxfilesize'] , channel['exts'])
        if item:
            return item.id
        
        namebase = self.get_argument("pagetype") + "cl"
        name = namebase
        postfix = 1
        while True:
            item = self.db.get("SELECT * FROM channels WHERE name = %s", name) 
            if not item:
                break
            name = namebase + "_" + str(postfix)
            postfix += 1
            
        channel['name'] = name
        channel['savegroupid'] = savegroupid
        self._add_channel(channel)
        
        # what's the right way to get the current inserted item?
        item = self.db.get("SELECT * FROM channels WHERE savegroup_id = %s "
        "and handleimages = %s and minwidth = %s and minheight = %s "
        "and maxfilesize = %s and exts = %s LIMIT 1", savegroupid, channel['handleimages'],
        channel['minwidth'] , channel['minheight'] , channel['maxfilesize'] , channel['exts'])
        return item.id
    
    def _get_exts(self):
        exts = ''
        if self.get_argument("uploadtype", "") == "attach":
             exts = "*"
             return exts
         
        ext_list = []
        if self.get_argument("uploadtype", "") == "image":
            ext_list = ['jpg', 'bmp', 'png', 'gif', 'jpeg']
        elif self.get_argument("uploadtype", "") == "music":
            ext_list = ['mp3', 'ogg', 'avi']
        for ext in ext_list:
            if self.get_argument(ext, "") != "on":
                continue
            if exts:
                exts += ","
            exts += ext
        return exts
            
    def _get_handleimages(self): 
        if self.get_argument("uploadtype", "") != "image": 
            return ""
        num_image_size = 6 
        handleimages = ""
        for i in range(num_image_size):
            width = int(self.get_argument("imgwidth" + str(i), '0'))
            height = int(self.get_argument("imgheight" + str(i), '0'))
            if (width == 0 and height == 0) :
                continue
            prefix = self.get_argument("imgprefix" + str(i))
            quality = int(self.get_argument("imgquality" + str(i), '88'))
            crop = (self.get_argument("imgcrop" + str(i), "") == "on") and 1 or 0
            keeppng = (self.get_argument("imgkeeppng" + str(i), "") == "on") and 1 or 0
            keepgif = (self.get_argument("imgkeepgif" + str(i), "") == "on") and 1 or 0
            sharpen = (self.get_argument("imgsharpen" + str(i), "") == "on") and 1 or 0
            handle = "%d,%d,%s,%d,%d,%d,%d,30,%d" % (width, height, prefix, crop,
                                                     quality, keeppng, keepgif, sharpen)
            print(handle)
            if handleimages:
                handleimages += '|'
            handleimages += handle
        return handleimages
    
    def _get_tickettypeid(self):
        tickettype = self.get_argument("ticketselect", "none")
        item = self.db.get("SELECT * FROM tickets WHERE type = %s LIMIT 1", tickettype)
        return item.id
    def _get_outputtypeid(self):
        outputtype = self.get_argument('outputselect', 'json')
        item = self.db.get('SELECT * FROM outputtypes WHERE type = %s LIMIT 1 ', outputtype)
        return item.id
        
    def check_args(self):
        name = self.get_argument("pagetype", '')
        if not name:
            return (False, "pagetype can not be empty")
        item = self.db.get("SELECT * FROM services WHERE name = %s", name) 
        if item:
            return (False, "pagetype has already existed")
        uploadtype = self.get_argument("uploadtype", "")
        imgwidth = self.get_argument("imgwidth0", "")
        if uploadtype == "image" and not imgwidth:
          return (False, "Add at least one image size")
        return (True, "")
    
class ConfigFileGenerator(object):
    def __init__(self, db, operator):
        self.db = db
        self.operator = operator
        
    def gen_config_files(self, template, service):
         id_channel = self._gen_template_file(template)
         self._gen_service_file(service, id_channel)
         pass
     
    def _gen_template_file(self, filename):
        f = open(filename, 'w')
        self._write_header(f)
        id_channel = dict()
        savegroups = self._gen_save_groups()
        query = "SELECT * FROM channels"
        items = self.db.iter(query)
        for item in items:
            id_channel[item.id] = item.name
            f.write("[%s]\n" % item.name)
            f.write("savegroups=%s\n" % savegroups[item.savegroup_id])
            f.write("handleimages=%s\n" % item.handleimages)
            f.write("minwidth=%d\n" % item.minwidth)
            f.write("minheight=%d\n" % item.minheight)
            f.write("maxfilesize=%d\n" % item.maxfilesize)
            if item.exts:
                f.write("exts=%s\n" % item.exts)
            f.write("\n")
        f.close()
        return id_channel
    
    def _gen_service_file(self, filename, id_channel):
        id_outputtype = self._gen_output_types()
        id_tickettype = self._gen_ticket_types()
        f = open(filename, 'w')
        self._write_header(f)
        query = "SELECT * FROM services"
        items = self.db.iter(query)
        for item in items: 
            f.write("[%s]\n" % item.name)
            f.write("#@userid:%d\n" % item.user_id)
            f.write("#@servicecomment:%s\n" % item.servicecomment)
            f.write("channel=%s\n" % id_channel[item.channel_id])
            f.write("watermark=%s\n" % (item.watermark > 0 and "true" or "false"))
            f.write("markpath=%s\n" % item.markpath)
            f.write("savesubdir=%s\n" % item.savesubdir)
            f.write("handleexif=%s\n" % (item.handleexif > 0 and "true" or "false"))
            f.write("checkcomment=%s\n" % (item.checkcomment > 0 and "true" or "false"))
            f.write("outputtype=%s\n" % id_outputtype[item.outputtype_id])
            f.write("tickettype=%s\n" % id_tickettype[item.tickettype_id])
            f.write("ticketsuffix=%s\n" % item.ticketsuffix)
            f.write("autorotate=%s\n" % (item.autorotate > 0 and "true" or "false"))
            if item.domain:
                f.write("domain=%s\n" % item.domain)
            f.write("\n")
            
        f.close()
            
    def _gen_save_groups(self):
        savegroups = dict()
        query = 'SELECT * FROM savegroups'
        items = self.db.iter(query)
        for item in items:
            savegroups[item.id] = item.value
        return savegroups 
    def _gen_output_types(self):
        id_outputtype = dict()
        query = 'SELECT * FROM outputtypes'
        items = self.db.iter(query)
        for item in items:
            id_outputtype[item.id] = item.type
            
        return id_outputtype
    
    def _gen_ticket_types(self):
        id_tickettype = dict()
        query = 'SELECT * FROM tickets'
        items = self.db.iter(query)
        for item in items:
            id_tickettype[item.id] = item.type
        return id_tickettype 
    
    def _write_header(self, f):
        f.write("#Generated by: %s\n" % self.operator)
        f.write("#date: %s\n" % str(datetime.datetime.now()))
        f.write("\n\n")

class GenConfsHandler(BaseHandler):
    @tornado.web.authenticated
    def post(self):
        template = "template.conf"
        service = "service.conf"
        operator = self.get_secure_cookie('name')
        conf_generator = ConfigFileGenerator(self.db, operator)
        conf_generator.gen_config_files(template, service)
        tar = tarfile.open("confs.tar.gz", "w:gz")
        tar.add(template)
        tar.add(service)
        tar.close()
        tar = open('confs.tar.gz', 'rb')
        self.set_header('Content-Type', 'application/x-gzip')
        self.set_header('Content-Disposition', 'attachment; filename="confs.tar.gz"')
        self.write(tar.read())
        tar.close()
        
class ViewHandler(BaseHandler):
    @tornado.web.authenticated
    def post(self):
        serviceid = self.get_argument('serviceid')
        item = self.db.get('SELECT * FROM services WHERE id = %s limit 1', serviceid)
        userid = item.user_id
        channelid = item.channel_id
        outputtypeid = item.outputtype_id
        info = dict()
        info['pagetype'] = item.name
        info['servicecomment'] = item.servicecomment
        info['ticketsuffix'] = item.ticketsuffix
        item = self.db.get("SELECT * FROM tickets WHERE id = %s limit 1", item.tickettype_id)
        info['tickettype'] = item.type
        item = self.db.get("SELECT * FROM users WHERE id = %s limit 1", userid)
        info['owner'] = item.name
        info['ownerdepartment'] = item.department
        info['owneremail'] = item.email
        item = self.db.get("SELECT * FROM channels WHERE id = %s limit 1", channelid)
        info['handleimages'] = item.handleimages
        info['exts'] = item.exts
        item = self.db.get("SELECT * FROM outputtypes WHERE id = %s limit 1", outputtypeid)
        info['outputtype'] = item.type
        self.render("viewservice.html", title="View service", info=info)
        
class DeleteHandler(BaseHandler):
    @tornado.web.authenticated
    def post(self):
        serviceid = self.get_argument('serviceid')
        item = self.db.get("SELECT * FROM services WHERE id = %s LIMIT 1", serviceid)
        if not item:
            self.redirect('/')
            return
        service_name = item.name
        channelid = item.channel_id 
        userid = item.user_id
        item = self.db.get("SELECT * FROM users WHERE id = %s", userid)
        owner_name = item.name
        owner_email = item.email
        operator = self.get_secure_cookie('name')
        mail_title = "Upload service deleted:  %s" % service_name
        msg = ("%s deleted upload service: {name: %s, owner: %s}\n " % (operator,
                                                                        service_name, owner_name))
        self.db.execute('DELETE FROM services WHERE id = %s', serviceid)
        # delete channel if no service references it
        item = self.db.get("SELECT id from services where channel_id = %s LIMIT 1", channelid)
        if not item:
            self.db.execute('DELETE FROM channels where id = %s', channelid)
            
        operator = self.get_secure_cookie('name')
        UpdateTestEnvironment(self.db, operator)
        
        toaddrs = FIX_TOADDRS + (owner_email,)
        SendEmail(FROMADDR, PASSWORD, toaddrs, msg, mail_title)
        self.redirect('/')
        
        
class ImportHandler(BaseHandler):
    def __init__(self, application, request, **kwargsself):
        BaseHandler.__init__(self, application, request, **kwargsself)
        self.savegroupscache = dict()
        self.channelcache = dict()
        self.outputtypecache = dict()
        self.tickettypecache = dict()
        
    @tornado.web.authenticated
    def post(self):
        files = self.request.files
        if not files.has_key('service') or not files.has_key('template'):
            return
        self._clearservices()
        self._handletemplates(files['template'][0]['body'])
        self._handleservices(files['service'][0]['body'])
        self.redirect('/')
    @tornado.web.authenticated
    def get(self):
        self.redirect('/')
        
    def _handletemplates(self, content):
        self.db.execute('DELETE FROM channels')
        lines = content.split('\n')
        i = 0
        title_pattern = '\[(\\w+)\]'
        title_re = re.compile(title_pattern)
        keys = ['savegroups', 'handleimages', 'minwidth', 'minheight', 'maxfilesize', 'exts']
        while i < len(lines):
            line = lines[i].strip()
            i += 1
            m = title_re.match(line)
            if not m:
                continue
            #title has been found
            temp = dict()
            temp['name'] = m.group(1)
            temp['handleimages'] = ' '
            temp['minwidth'] = 1
            temp['minheight'] = 1
            temp['maxfilesize'] = 15
            temp['exts'] = ''
            while i < len(lines):
                line = lines[i].strip()
                i += 1
                if i == len(lines) or title_re.match(line):
                    # file end reached or a new template is started
                    # finish this template
                    i -= 1
                    if temp.has_key('name') and temp.has_key('savegroups'): 
                        self._inserttemplate(temp)
                    else:
                        print('except')
                        raise tornado.web.HTTPError(404)
                    break
                
                if not line or line.startswith('#'):
                    continue
                for key in keys:
                    if line.startswith(key):
                        temp[key] = line.split('=')[1]
                        break
                    
    def _handleservices(self, content):
        lines = content.split('\n')
        i = 0
        title_pattern = '\[(\\w+)\]'
        title_re = re.compile(title_pattern)
        keys = ['userid', 'servicecomment', 'channel', 'watermark', 'markpath', \
                'savesubdir', 'handleexif', 'checkcomment', \
                'outputtype', 'tickettype', 'ticketsuffix', 'autorotate', 'domain']
        while i < len(lines):
            line = lines[i].strip()
            i += 1
            m = title_re.match(line)
            if not m:
                continue
            #title has been found
            service = dict()
            service['name'] = m.group(1)
            service['userid'] = self.get_secure_cookie('userid')
            service['servicecomment'] = ''
            service['watermark'] = 'false'
            service['markpath'] = ''
            service['savesubdir'] = ''
            service['handleexif'] = 'true'
            service['checkcomment'] = 'false'
            service['autorotate'] = 'true'
            service['domain'] = ''
            while i < len(lines):
                line = lines[i].strip()
                i += 1
                if i == len(lines) or title_re.match(line):
                    # file end reached or a new service is started
                    # finish this one 
                    i -= 1
                    if (service.has_key('name') and service.has_key('channel') and 
                        service.has_key('outputtype') and service.has_key('tickettype')):
                        self._insert_service(service)
                    else:
                        print(service)
                        raise tornado.web.HTTPError(404)
                        
                    break
                
                if not line:
                    continue
                if line.startswith('#@'):
                    print(line)
                    if line.startswith('userid', len('#@')):
                        print('userid')
                        service['userid'] = int(line.split(':')[1])
                    elif line.startswith('servicecomment', len('#@')):
                        pos = line.find(':')
                        service['servicecomment'] = line[pos + 1:].lstrip()
                    continue
                if line.startswith('#'):
                    continue
                for key in keys:
                    if line.startswith(key):
                        service[key] = line.split('=')[1]
                        break
                    
    def _clearservices(self):
        self.db.execute('DELETE FROM services')
    def _inserttemplate(self, temp):
        savegroup_id = self.savegroupscache.get(temp['savegroups'])
        if not savegroup_id:
            item = self.db.get('SELECT * FROM savegroups WHERE value = %s', temp['savegroups'])
            if item:
                # add to cache
                self.savegroupscache[temp['savegroups']] = item.id
                savegroup_id = item.id
            else:
                self.db.execute('insert into savegroups(value) values(%s)', temp['savegroups'])
                item = self.db.get('SELECT * FROM savegroups WHERE value = %s', temp['savegroups'])
                self.savegroupscache[temp['savegroups']] = item.id
                savegroup_id = item.id
                
        temp['savegroupid'] = savegroup_id
        self._add_channel(temp)
        
    def _insert_service(self, service):
        channel_id = self.channelcache.get(service['channel'])
        if not channel_id:
            item = self.db.get('SELECT * FROM channels WHERE name = %s', service['channel'])
            if item:
                # add to cache
                self.channelcache[service['channel']] = item.id
                channel_id = item.id
            else:
                print(service['channel'])
                raise tornado.web.HTTPError(404)
        outputtype_id = self.outputtypecache.get(service['outputtype'])
        if not outputtype_id:
            item = self.db.get('SELECT * FROM outputtypes WHERE type = %s', service['outputtype'])
            if item:
                # add to cache
                self.outputtypecache[service['outputtype']] = item.id
                outputtype_id = item.id
            else:
                print("except: %s" % service['outputtype'])
                raise tornado.web.HTTPError(404)
        tickettype_id = self.tickettypecache.get(service['tickettype'])
        if not tickettype_id:
            item = self.db.get('SELECT * FROM tickets WHERE type = %s', service['tickettype'])
            if item:
                # add to cache
                self.tickettypecache[service['tickettype']] = item.id
                tickettype_id = item.id
            else:
                print(service['tickettype'])
                print('except 643')
                raise tornado.web.HTTPError(404)
            
        service['channelid'] = channel_id
        service['outputtypeid'] = outputtype_id
        service['tickettypeid'] = tickettype_id
        service['watermark'] = int(service['watermark'] == 'true')
        service['handleexif'] = int(service['handleexif'] == 'true')
        service['checkcomment'] = int(service['checkcomment'] == 'true')
        service['autorotate'] = int(service['autorotate'] == 'true')
        self._add_service(service)
    
def main():
    tornado.options.parse_command_line()
    http_server = tornado.httpserver.HTTPServer(Application())
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.instance().start()

def deamonlize():
    pid = os.fork()
    if pid != 0:
      sys.exit(-1)
    #os.umask(0)
    #null = os.open('/dev/null', os.O_RDWR)
    #os.dup2(0, null)
    #os.dup2(1, null)
    #os.dup2(2, null)
  

if __name__ == "__main__":
    deamonlize()
    main()

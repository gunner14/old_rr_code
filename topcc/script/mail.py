# -*- coding: utf-8 -*-  
  
import email  
import mimetypes  
from email.MIMEMultipart import MIMEMultipart  
from email.MIMEText import MIMEText  
from email.MIMEImage import MIMEImage  
from email.header import Header
import smtplib  
import time
  
def sendmail(authInfo, fromAdd, toAdd, subject, plainText, htmlText):  
  
        strFrom = fromAdd  
        strTo = ', '.join(toAdd)  
  
        server = authInfo.get('server')  
        user = authInfo.get('user')  
        passwd = authInfo.get('password')  
  
        if not (server and user and passwd) :  
                print 'incomplete login info, exit now'  
                return  
  
        # 设定root信息  
        msgRoot = MIMEMultipart('related')
        msgRoot['Subject'] = Header(subject,'utf-8')
        msgRoot['From'] = strFrom  
        msgRoot['To'] = strTo 
        #msgRoot['Cc'] = strCc
        msgRoot.preamble = 'This is a multi-part message in MIME format.'  
        #msgRoot['Reply-To'] = strFrom  
        msgRoot['Date'] = time.ctime(time.time())  
  
        # Encapsulate the plain and HTML versions of the message body in an  
        # 'alternative' part, so message agents can decide which they want to display.  
        msgAlternative = MIMEMultipart('alternative')  
        msgRoot.attach(msgAlternative)  
  
        #设定纯文本信息  
        if plainText:
          msgText = MIMEText(plainText, 'plain', 'utf-8')  
          msgAlternative.attach(msgText)  
  
        #设定HTML信息  
        if htmlText:
          msgText = MIMEText(htmlText, 'html', 'utf-8')  
          msgAlternative.attach(msgText)  
  
        #设定附件
        #filename = r'/app/opt/oracle/report.zip'  
        #fp = open(filename,'rb')  
        #ctype,encoding = mimetypes.guess_type(filename)  
        #if ctype is None or encoding is not None:  
        #        ctype = 'application/octet-stream'  
        #maintype,subtype = ctype.split('/',1)  
        #m = MIMEBase(maintype,subtype)  
        #m.set_payload(fp.read())  
        #fp.close()  
        #Encoders.encode_base64(m)  
        #m.add_header('Content-disposition','attachment',filename='report.zip')    
        #msgRoot.attach(m)                                                       

       #设定内置图片信息  
#       fp = open('test.jpg', 'rb')  
#       msgImage = MIMEImage(fp.read())  
#       fp.close()  
#       msgImage.add_header('Content-ID', '<image1>')  
#       msgRoot.attach(msgImage)  
  
       #发送邮件  
        smtp = smtplib.SMTP()  
       #设定调试级别，依情况而定  
        #smtp.set_debuglevel(1)  
        smtp.connect(server)  
        smtp.login(user, passwd)  
        smtp.sendmail(strFrom, toAdd, msgRoot.as_string())  
#        smtp.sendmail(strFrom, strTo, msgRoot.as_string())  
        smtp.quit()
        return
  
if __name__ == '__main__' :  
        authInfo = {}  
        authInfo['server'] = 'smtp.renren.com'  
        authInfo['user'] = 'photo@renren.com'  
        authInfo['password'] = 'photo_sparta_1207'
        fromAdd = 'photo@renren.com'  
        toAdd = ['lantao.liu@renren-inc.com','qinghui.liu@renren-inc.com']  
        subject = 'my title,中文'
        plainText = '这里是普通文本'
        htmlText = '<B>HTML文本</B>'
        sendmail(authInfo, fromAdd, toAdd, subject, plainText, htmlText)  

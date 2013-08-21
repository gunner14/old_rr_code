import smtplib,email,email.MIMEMultipart,email.MIMEBase,email.MIMEText
import time,datetime,string
from email import Utils

PATH = "/data/yinxin/scripts/monitor_guide_access/"
dstr1 = time.strftime("%y-%m-%d [00 - 15]",time.localtime())

str_th_style1 = "style=\"height: 10px; line-height: 10px;\"";
str_td_style1 = "style=\"height: 10px; line-height: 10px;\"";
str_th_style2 = "style=\"height: 10px; line-height: 10px;text-align: left;\"";
str_span_style1 = "style=\"padding:5px; border:1px solid #F2F2F2;background:none repeat scroll 0 0 #EE9A00;font-size:14px;color:white;\""
str_style_row = ["style=\"background:#FFEC8B\"","style=\"background:#FFF0F5\""];
str_unlimit = "∞"

mes = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
mes += "\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
mes += "<html>\n"
mes += "<head>\n"
#mes += "<link href=\"http://s.xnimg.cn/csspro/base/layout.css\" rel=\"stylesheet\" type=\"text/css\" media=\"all\" />\n"
#mes += "<link href=\"http://s.xnimg.cn/csspro/apps/point.css\" rel=\"stylesheet\" type=\"text/css\" media=\"all\" />\n"
mes += "<style>\n"
mes += "body { font-family:Tahoma,Verdana,STHeiTi,simsun,sans-serif; font-size:12px; }"
mes += "table {border-collapse:collapse;align:center}"
mes += "td {font-size:11px;border:1px solid #DDDDDD;}"
mes += "th {font-size:10px;color:blue;border:1px solid #DDDDDD;background:none repeat scroll 0 0 #F2F2F2;}"
mes += "</style>\n"
mes += "</head>\n"
mes += "<body id=\"home\" class=\"home\">"		
mes += "Index >>" 
mes += "<a href=\"#avg\">Average Report [00 - 15]</a>"
mes += "|<a href=\"#a35\">Cost Top Report of 35</a>"
mes += "|<a href=\"#a36\">Cost Top Report of 36</a>"
mes += "<div align=\"center\" style=\"width:100%;\">"
mes += "<div align=\"center\" style=\"padding:10px;\"><span align=\"center\""+str_span_style1+">Guide Performance Report - "+dstr1+"<a name=\"avg\" href=\"#TOP\">Back</a></span></div>"
mes += "<table align=\"center\">"
mes += "<th>St(Cnt)</th><th>Req</th><th>Average</th><th>Count</th><th>[0,100]</th><th>(100,300]</th><th>(300,1000]</th><th>(1000,"+str_unlimit+")</th></tr>\n"
myfile = open(PATH+"stat.ret.txt.all.0015.merged")
id = 0
for line in myfile:
    words = line.split('|')
    if len(words) > 10:
        continue
    for i in range(1,8):
        if words[i] == "":
            words[i] = "0"
    id += 1
    avgtime = string.atof(words[2])*1000 / string.atof(words[3])
    mes += "<tr "+str_style_row[(id%2)]+"><th>"+str(id)+"</th><td>"+words[1][0:39]+"</td><td>"+str("%.2f"%avgtime)+"</td><td>"+words[3]+"</td><td>"+words[4]+"</td><td>"+words[5]+"</td><td>"+words[6]+"</td><td>"+words[7]+"</td></tr>\n"
mes += "</table>\n"

mes += "<div align=\"center\" style=\"padding:10px;\"><span align=\"center\""+str_span_style1+">Guide Performance Report - Cost Top of 10.3.17.35 <a name=\"a35\" href=\"#TOP\">Back</a></span></div>"
mes += "<table align=\"center\">"
mes += "<th>St(Cnt)</th><th>Req</th><th>Average</th><th>Count</th><th>[0,100]</th><th>(100,300]</th><th>(300,1000]</th><th>(1000,"+str_unlimit+")</th></tr>\n"
myfile = open(PATH+"stat.ret.txt.35.all.0015.merged")
id = 0
for line in myfile:
    words = line.split('|')
    if len(words) > 10:
        continue
    if string.atoi(words[3]) <= 10:
        continue
    for i in range(1,8):
        if words[i] == "":
            words[i] = "0"
    id += 1
    avgtime = string.atof(words[2])*1000 / string.atof(words[3])
    mes += "<tr "+str_style_row[(id%2)]+"><th>"+str(id)+"</th><td>"+words[1][0:39]+"</td><td>"+str("%.2f"%avgtime)+"</td><td>"+words[3]+"</td><td>"+words[4]+"</td><td>"+words[5]+"</td><td>"+words[6]+"</td><td>"+words[7]+"</td></tr>\n"
mes += "</table>\n"

mes += "<div align=\"center\" style=\"padding:10px;\"><span align=\"center\""+str_span_style1+">Guide Performance Report - Cost Top of 10.3.17.36<a name=\"a36\" href=\"#TOP\">Back</a></span></div>"
mes += "<table align=\"center\">"
mes += "<th>St(Cnt)</th><th>Req</th><th>Average</th><th>Count</th><th>[0,100]</th><th>(100,300]</th><th>(300,1000]</th><th>(1000,"+str_unlimit+")</th></tr>\n"
myfile = open(PATH+"stat.ret.txt.36.all.0015.merged")
id = 0
for line in myfile:
    words = line.split('|')
    if len(words) > 10:
        continue
    if string.atoi(words[3]) <= 10:
        continue
    for i in range(1,8):
        if words[i] == "":
            words[i] = "0"
    id += 1
    avgtime = string.atof(words[2])*1000 / string.atof(words[3])
    mes += "<tr "+str_style_row[(id%2)]+"><th>"+str(id)+"</th><td>"+words[1][0:39]+"</td><td>"+str("%.2f"%avgtime)+"</td><td>"+words[3]+"</td><td>"+words[4]+"</td><td>"+words[5]+"</td><td>"+words[6]+"</td><td>"+words[7]+"</td></tr>\n"
mes += "</table>\n"

mes += "</div>\n"
mes += "</body></html>\n"
#msg = email.MIMEText.MIMEText(mes, _subtype="html", _charset="utf-8")
msg = email.MIMEText.MIMEText(mes,'html','utf-8')
msg['To'] = 'xin.yin@opi-corp.com,wei.xiang@opi-corp.com'
msg['From'] = 'xin.yin@opi-corp.com'
msg['Cc'] = 'zhenya.hong@opi-corp.com'
msg['Date'] = Utils.formatdate(localtime = 1)
msg['Subject'] ='Guide Performance Report '+dstr1

s = smtplib.SMTP("mail.opi-corp.com")
tomail = ['xin.yin@opi-corp.com']
#tomail = ['xin.yin@opi-corp.com','wei.cheng@opi-corp.com']
#tomail = ['xin.yin@opi-corp.com','wei.xiang@opi-corp.com','zhenya.hong@opi-corp.com']
#s.sendmail("xin.yin@opi-corp.com", tomail, msg.as_string())
s.close()
#print msg.as_string()

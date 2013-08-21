# -*- coding: utf-8 -*-  

import re
from operator import itemgetter
from sendmail import sendmail
import datetime



#10.3.18.59| -| 20/Feb/2012:04:02:02 +0800| POST /upload.fcgi?pagetype=addphotoflash&hostid=254163343&tick=ba8b7fdbdcafb447d6281f724865510f&uploadid=fileItem8834043_6&block_index=0&block_count=1 HTTP/1.0| 254163343| 200| 1254| http://s.xnimg.cn/n/core/modules/flashUploader/pic_uploader_v6.swf?ver=$revxxx$| Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30; TheWorld)| 218.29.202.49| 0.126| 0.121| 53391
#10.7.18.79| -| 20/Feb/2012:04:02:19 +0800| POST /upload.fcgi?pagetype=addphotoflash&hostid=236543090&tick=1aaf0234b219d0f125bbaed3176544f3&uploadid=fileItem69320153_5&block_count=1&block_index=0 HTTP/1.0| 236543090| 200| 2895| http://s.xnimg.cn/n/core/modules/flashUploader/pic_uploader_v6.swf?ver=$revxxx$| Mozilla/5.0 (Windows NT 5.1; rv:10.0.1) Gecko/20100101 Firefox/10.0.1| 144.32.71.105| 0.372| 0.369| 99880
#10.3.18.59| -| 20/Feb/2012:04:02:20 +0800| POST /uploadservice.fcgi?pagetype=addPhotoAPI&op=finish HTTP/1.0| -| 200| 445| -| Jakarta Commons-HttpClient/3.1| 123.125.42.25| 0.441| 0.426| 730780

css = r'''<style type=\"text/css\">
  table{border-left: 1px solid #C1DAD7;border-top: 1px solid #C1DAD7; 
    padding: 0; margin: 0; font: normal 12px auto "Trebuchet MS", 
    Verdana, Arial, Helvetica, sans-serif;}  
  th{font-size:13px; color: #4f6b72; border-right: 
    1px solid #C1DAD7; border-bottom: 1px solid #C1DAD7; 
    letter-spacing: 2px; text-transform: uppercase; text-align: left; 
    padding: 6px 6px 6px 12px; background: #CAE8EA  no-repeat;}
  td{border-right: 1px solid #C1DAD7; border-bottom: 1px solid #C1DAD7; 
     background: #fff; font-size:12px; padding: 6px 6px 6px 12px; color: #4f6b72; }
   </style>'''


v1 = re.compile(r'''POST /.+\.fcgi\?pagetype=(\w+).+\| ([0-9\.]+)\| ([0-9\.]+)\| \d+$''', re.U|re.S)

# all count, all requesttime , all repons time, <50ms count, 50ms count, 100ms count, ... 500ms count,> 500ms$
x = {}
y = []

#heads = ["tab1","tab2","tab3"]
#ls = [(1,2,3),(4,5,6),(9,8,7)]
def table(heads, ls):
  sstr = r'''<table cellpadding='0' cellspacing='0'><tr>'''
  for i in heads:
    sstr += "<th>"
    sstr += i
    sstr += "</th>"
  sstr += "</tr>"
  for i in ls:
    sstr += "<tr>"
    for j in i:
      sstr += "<td>"
      sstr += str(j)
      sstr += "</td>"
    sstr += "</tr>"
  return sstr

if __name__ == "__main__":
  print "begin scan log..."
  f = open("access.all.log")
  for line in f:
    #ss = line.decode("u8")
    grp = v1.search(line)
    if grp:
      if grp.group(0).find('op=finish') > 0:
        continue
      pagetype = grp.group(1)
      reqtime = float(grp.group(2))*1000
      reponstime = float(grp.group(3))*1000
      if not pagetype in x:
        x[pagetype] = [pagetype, 0,0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0]
      p = x[pagetype]
      p[1] += 1
      p[2] += reqtime
      p[3] += reponstime
      if reponstime < 50:
        p[4] += 1
      elif reponstime > 1050:
        p[23] += 1
      else:
        #print reponstime
        p[int((reponstime - 50)/50) + 5] += 1
      #print "pagetype:", pagetype, " repons:", reponstime, p
  f.close()
  print " end scan,", len(x)
  for i,j in x.items():
    y.append(j)
  #for i in y:
  #  print i
  print " start sort,", len(y)
  z = sorted(y, reverse=True, key = itemgetter(1))
  for i in z:
    i[2] = str(int(i[2] / i[1])) + "ms"
    i[3] = str(int(i[3] / i[1])) + "ms"
    for j in xrange(22):
      #print i[j+4], i[1], int(i[j+4] * 100  / i[1])
      i[j+4] = str(int(i[j+4] * 100 / i[1] )) + "%"
    
  now = datetime.datetime.now() #当前时间
  now -= datetime.timedelta(hours=24)
  # sort by req count
  authInfo = {}
  authInfo['server'] = 'smtp.renren.com'
  authInfo['user'] = 'photo@renren.com'
  authInfo['password'] = 'photo_sparta_1207'
  fromAdd = 'photo@renren.com'
  toAdd = ['qinghui.liu@renren-inc.com']  
  subject = 'C++上传处理时间统计 on ' + now.strftime("%Y-%m-%d") 
  plainText = ''
  #['yujian', 127, 672, 659, 0, 4, 14, 9, 20, 12, 5, 10, 4, 3, 2, 5, 3, 1, 3, 2, 2, 2, 2, 22, 2, 0]
  htmlText = css + table(("上传类型", "次数", "请求时间", "处理时间", "&lt;50ms", "50-100ms", "100-150ms", "150-200ms", "200-250ms", "250-300ms", "300-350ms", "350-400ms", "400-450ms", "450-500ms", "500-550ms", "550-600ms","600-650ms", "650-700ms","700-750ms", "750-800ms","800-850ms", "850-900ms","900-950ms", "950-1000ms","1000-1050ms","&gt;1050ms"), z)
  sendmail(authInfo, fromAdd, toAdd, subject, plainText, htmlText)


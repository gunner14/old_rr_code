import tornado.database
import sys
reload(sys)
sys.setdefaultencoding('UTF-8')
def ParseService():
    host="10.7.18.21:3306"
    database="web" 
    password=""
    user="root"
    db=tornado.database.Connection(
    host=host, database=database,
    user=user,password=password)

    a=open('service.conf','r+')
    channel=""
    watermark=""
    markpath=""
    savesubdir=""
    autorotate=""
    handleexif=""
    checkcomment=""
    outputtype=""
    domain=""
    tickettype=""
    ticketsuffix=""
    notation=""
    last_notation=""
    last=""
    flag=0 #ungly
    for line in a:

        if line.find('#')!=-1:
            index=line.find('#')
            notation+=line[index+1:-1]
            if flag==0:
                last_notation=notation
            continue

        if line[0]=='[':
            index=line.find(']')
            pagetype=line[1:index]
            pagetype=pagetype.strip()
    #        print pagetype
            if last!="":
                tmp=db.get("SELECT * FROM service WHERE pagetype= %s",last)
                if not tmp:
                    db.execute("INSERT INTO service (pagetype,channel,watermark,markpath,savesubdir,autorotate,handleexif,checkcomment,\
                    outputtype,tickettype,ticketsuffix,notation,domain)VALUES(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",last,channel,watermark,markpath,\
                    savesubdir,autorotate,handleexif,checkcomment,outputtype,tickettype,ticketsuffix,last_notation,domain)
      
                    watermark=""
                    markpath=""
                    savesubdir=""
                    autorotate=""
                    handleexif=""
                    checkcomment=""
                    outputtype=""
                    tickettype=""
                    ticketsuffix=""
                    domain=""
                    if flag==0:
                      last_notation=""
                      flag=1
                    else:  
                      last_notation=notation
                    notation=""
            last=pagetype
            continue

       
        if line.find("channel")!=-1:
            index=line.find('=')
            channel=line[index+1:-1]
            continue  

        if line.find("watermark")!=-1:
            index=line.find('=')
            watermark=line[index+1:-1]
            continue  
        
        if line.find("autorotate")!=-1:
            index=line.find('=')
            autorotate=line[index+1:-1]  
            continue  

        if line.find("markpath")!=-1:
            index=line.find('=')
            markpath=line[index+1:-1]
            continue  

        if line.find("savesubdir")!=-1:
            index=line.find('=')
            savesubdir=line[index+1:-1]  
            continue
        
        if line.find("handleexif")!=-1:
            index=line.find('=')
            handleexif=line[index+1:-1]  
            continue

        if line.find("checkcomment")!=-1:
            index=line.find('=')
            checkcomment=line[index+1:-1]  
            continue

        if line.find("outputtype")!=-1:
            index=line.find('=')
            outputtype=line[index+1:-1]
            continue

        if line.find("domain")!=-1:
            index=line.find('=')
            domain=line[index+1:-1]
            continue


        if line.find("tickettype")!=-1:
            index=line.find('=')
            tickettype=line[index+1:-1]  
            continue

        if line.find("ticketsuffix")!=-1:
            index=line.find('=')
            ticketsuffix=line[index+1:-1]  
            continue

    tmp=db.get("SELECT * FROM service WHERE pagetype= %s",last)
    if not tmp:
        db.execute("INSERT INTO service (pagetype,channel,watermark,markpath,savesubdir,autorotate,handleexif,checkcomment,\
        outputtype,tickettype,ticketsuffix,notation,domain)VALUES(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",last,channel,watermark,markpath,\
        savesubdir,autorotate,handleexif,checkcomment,outputtype,tickettype,ticketsuffix,last_notation,domain)


def ParseTemplate():
   
    host="10.7.18.21:3306"
    database="web" 
    password=""
    user="root"
    db=tornado.database.Connection(
    host=host, database=database,
    user=user,password=password)
    a=open('template.conf','r+')

#init value ,if none then be zero,should trans none when generate a .conf
    minwidth=""
    minheight=""
    maxfilesize=""
    last=""
    notation=""
    last_notation=""
    flag=0
    for line in a:
        if line[0]=='[':
            index=line.find(']')
            channel=line[1:index]
            channel=channel.strip()
            if last!="":
                tmp=db.get("SELECT * FROM channelinfo WHERE channel= %s",last)
                if not tmp:
                    db.execute("INSERT INTO channelinfo (channel,minwidth,minheight,maxfilesize,notation)VALUES(%s,%s,%s,%s,%s)",\
                    last,minwidth,minheight,maxfilesize,last_notation)    
                    minwidth=""
                    minheight=""
                    maxfilesize=""
                    if flag==0:
                       last_notation=""
                       flag=1
                    else:
                       last_notation=notation   
                    notation="" 
           
            last=channel
        if line.find("savegroups")!=-1:
            index=line.find('=')
            save=line[index+1:-1]
            savegrp=save.split('|')
#            print savegrp
            for item in savegrp:
                item=item.strip()
                tmp=db.get("SELECT * FROM savegroups WHERE channel= %s AND savegroup= %s",channel,item)
                if not tmp:
                    db.execute("INSERT INTO savegroups (savegroup,channel)VALUES(%s,%s)",item,channel)
       
        if line.find("handleimages")!=-1:
            index=line.find('=')
            images=line[index+1:-1]
            imggrp=images.split('|')
        #    print imggrp
            for item in imggrp:
                item=item.strip()
                handle=item.split(',')
       #         print handle
                tmp=db.get("SELECT * FROM handleimages WHERE width = %s AND height = %s AND nameprefix = %s AND crop = %s AND quality = %s \
                                 AND  samepngtype = %s AND samegiftype = %s AND nginx_cache=%s AND sharpen = %s AND channel= %s",int(handle[0]),int(handle[1]),\
                                 handle[2],int(handle[3]),int(handle[4]),int(handle[5]),int(handle[6]),int(handle[7]),int(handle[8]),channel)
                if not tmp:
                  db.execute("INSERT INTO handleimages (width,height,nameprefix,crop,quality,samepngtype,samegiftype,nginx_cache,sharpen,channel) \
                  VALUES(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",int(handle[0]),int(handle[1]),handle[2],int(handle[3]),int(handle[4]),int(handle[5]),\
                  int(handle[6]),int(handle[7]),int(handle[8]),channel)

        if line.find("exts")!=-1:
            index=line.find('=')
            exts=line[index+1:-1]
            extgrp=exts.split(',')
         #   print extgrp
            for item in extgrp:
                item=item.strip()
                tmp=db.get("SELECT * FROM exts WHERE ext= %s AND channel=%s",item,channel)
                if not tmp:
                    db.execute("INSERT INTO exts (ext,channel)VALUES(%s,%s)",item,channel)  
       
        if line.find('#')!=-1:
            index=line.find('#')
            notation+=line[index+1:-1]
            if flag==0:
               last_notation=notation

        if line.find("minwidth")!=-1:
            index=line.find('=')
            minwidth=line[index+1:-1]  

        if line.find("minheight")!=-1:
            index=line.find('=')
            minheight=line[index+1:-1]

        if line.find("maxfilesize")!=-1:
            index=line.find('=')
            maxfilesize=line[index+1:-1]
    
    
    tmp=db.get("SELECT * FROM channelinfo WHERE channel= %s",last)
    if not tmp:
        db.execute("INSERT INTO channelinfo (channel,minwidth,minheight,maxfilesize,notation)VALUES(%s,%s,%s,%s,%s)",\
        last,minwidth,minheight,maxfilesize,last_notation)  
     
 
    a.close()



class ImageFormatItem(object):
    def __init__(self,width=0,height=0,crop=0,quality=0,samepngtype=0,samegiftype=0,sharpen=1):
       self.width=width
       self.height=height
       self.crop=crop
       self.quality=quality
       self.samepngtype=samepngtype
       self.samegiftype=samegiftype
       self.sharpen=sharpen


def SaveServiceConfDB(**dict):
    host="10.7.18.21:3306"
    database="web" 
    password=""
    user="root"
    db=tornado.database.Connection(
    host=host, database=database,
    user=user,password=password)
    
    if(dict.has_key('notation')):
      notation=dict['notation']
    else:
      print "miss notation"
      return 

    if(dict.has_key('pagetype')):
      pagetype=dict['pagetype']
    else:
      print "miss pagetype"
      return  

    if(dict.has_key('channel')):
      channel=dict['channel']
    else:
      print "miss channel"
      return  

    if(dict.has_key('watermark')):
      watermark=dict['watermark']
    else:
      print "miss watermark"
      return  

    if(dict.has_key('markpath')):
      markpath=dict['markpath']
    else:
      print "miss markpath"
      return   

    if(dict.has_key('savesubdir')):
      savesubdir=dict['savesubdir']
    else:
      print "miss savesudir"
      return   

    if(dict.has_key('autorotate')):
      autorotate=dict['autorotate']
    else:
      print "miss autorotate"
      return   


    if(dict.has_key('handleexif')):
      handleexif=dict['handleexif']
    else:
      print "miss handleexif"
      return 

    if(dict.has_key('checkcomment')):
      checkcomment=dict['checkcomment']
    else:
      print "miss checkcomment"
      return  

    if(dict.has_key('outputtype')):
      outputtype=dict['outputtype']
    else:
      print "miss outputtype"
      return  

    if(dict.has_key('domain')):
      domain=dict['domain']
    else:
      print "miss domain"
      return  


    if(dict.has_key('tickettype')):
      tickettype=dict['tickettype']
    else:
      print "miss tickettype"
      return  

    if(dict.has_key('ticketsuffix')):
      ticketsuffix=dict['ticketsuffix']
    else:
      print "miss ticketsuffix"
      return

 

    tmp=db.get("SELECT * FROM service WHERE pagetype= %s",pagetype)
    if not tmp:
       print "do db"
       db.execute("INSERT INTO service (pagetype,channel,watermark,markpath,savesubdir,autorotate,handleexif,checkcomment,\
               outputtype,tickettype,ticketsuffix,notation,domain)VALUES(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",pagetype,channel,watermark,markpath,\
               savesubdir,autorotate,handleexif,checkcomment,outputtype,tickettype,ticketsuffix,notation,domain)



def GenServiceConf():
    host="10.7.18.21:3306"
    database="web" 
    password=""
    user="root"
    db=tornado.database.Connection(
    host=host, database=database,
    user=user,password=password)

    conf=open('service_test.conf','w+')
    tmp=db.query("SELECT * FROM service")
    for item in tmp:
        #conf.write("#test   \n")
        if item['notation']!="":
          notation= item['notation']
         # notation=unicode(item['notation'],'utf-8')
         # b=notation.encode('utf-8')
          conf.write("#"+notation+"\n")
        conf.write("["+item['pagetype']+"]\n")
        conf.write("channel="+item['channel']+"\n")
        if item['watermark']!="":
            conf.write("watermark="+item['watermark']+"\n")
        conf.write("markpath="+item['markpath']+"\n")
        conf.write("savesubdir="+item['savesubdir']+"\n")
        if item['autorotate']!="":
            conf.write("autorotate="+item['autorotate']+"\n")
        if item['handleexif']!="":
            conf.write("handleexif="+item['handleexif']+"\n")
        if item['checkcomment']!="":    
            conf.write("checkcomment="+item['checkcomment']+"\n")
        conf.write("outputtype="+item['outputtype']+"\n")
        if item['domain']!="":
            conf.write("domain="+item['domain']+"\n")
        conf.write("tickettype="+item['tickettype']+"\n")
        conf.write("ticketsuffix="+item['ticketsuffix']+"\n\n")

    conf.close()    
        
def GenTemplateConf():
    host="10.7.18.21:3306"
    database="web" 
    password=""
    user="root"
    db=tornado.database.Connection(
    host=host, database=database,
    user=user,password=password)

    conf=open('template_test.conf','w+')
    tmp=db.query("SELECT * FROM channelinfo")

    for item in tmp:
        notation=item['notation']
        channel=item['channel']
        minwidth=item['minwidth']
        minheight=item['minheight']
        maxfilesize=item['maxfilesize']
        handleimages=db.query("SELECT * FROM handleimages WHERE channel=%s",channel) 
        handleimages_str="handleimages=" 
        for node in handleimages:
            handleimages_str+=str(node['width'])+","+str(node['height'])+","+str(node['nameprefix'])+","+str(node['crop'])\
            +","+str(node['quality'])+","+str(node['samepngtype'])+","+str(node['samegiftype'])+","+str(node['nginx_cache'])\
            +","+str(node['sharpen'])+"|"
        
        index=handleimages_str.rfind('|')
        if index!=-1:
           handleimages_str=handleimages_str[0:index]
        else:
           handleimages_str=""  
     #   print handleimages_str
        
        savegroups=db.query("SELECT * FROM savegroups WHERE channel=%s",channel)
        savegroups_str="savegroups="
        for node in savegroups:
            savegroups_str+=str(node['savegroup'])+"|"
        index=savegroups_str.rfind('|')
        if index!=-1:
            savegroups_str=savegroups_str[0:index]
     #   print savegroups_str           
  
  
        exts=db.query("SELECT * FROM exts WHERE channel=%s",channel)
        exts_str="exts="
        for node in exts:
            exts_str+=str(node['ext'])+"|"
        index=exts_str.rfind('|')
        if index!=-1:
            exts_str=exts_str[0:index]
        else:
            exts_str=""    
     #   print exts_str   
     
             
        if notation!="":
          conf.write("#"+notation+"\n")
        conf.write("["+channel+"]\n")
        conf.write(savegroups_str+"\n")
        if handleimages_str.strip()!="":
           conf.write(handleimages_str+"\n")
        if minwidth!="":
           conf.write("minwidth="+str(minwidth)+"\n")
        if minheight!="":
           conf.write("minheight="+str(minheight)+"\n")
        if maxfilesize!="":
           conf.write("maxfilesize="+str(maxfilesize)+"\n")
        if exts_str.strip()!="":
           conf.write(exts_str+"\n")
        conf.write("\n")   

    conf.close()
 


def GetChannelFromFormat(Format):
    host="10.7.18.21:3306"
    database="web" 
    password=""
    user="root"
    db=tornado.database.Connection(
    host=host, database=database,
    user=user,password=password)
    
    if len(Format)==0:
      return set() 
#    channels=db.get("SELECT * FROM handleimages WHERE width=%s AND height=%s AND crop=%s",Format[0].width,Format[0].height,Format[0].crop)
 
#    channels=set()
    inters=None
    for item in Format:
        channels=set()
        print "####################",item.width,item.height,item.crop
        tmp=db.query("SELECT channel FROM handleimages WHERE width=%s AND height=%s AND crop=%s AND quality=%s AND samepngtype=%s \
        AND samegiftype=%s AND sharpen=%s",item.width,item.height,item.crop,item.quality,item.samepngtype,item.samegiftype,item.sharpen)
        for c in tmp:
            channels.add(c['channel'])             
        print channels 
        if inters==None:
            inters=channels
        else:
#            print channels,"********",inters  
            inters=inters.intersection(channels)
    return inters


def MakeOtherDefaultChannel(Name,Exts,Minwidth,Minheight,Maxfilesize):
    host="10.7.18.21:3306"
    database="web" 
    password=""
    user="root"
    db=tornado.database.Connection(
    host=host, database=database,
    user=user,password=password)
 
    savegroup=['fmn056,/bigpool/fmn056','fmn057,/bigpool/fmn057','fmn058,/bigpool/fmn058','fmn059,/bigpool/fmn059','fmn060,/bigpool/fmn060',\
               'fmn061,/bigpool/fmn061','fmn062,/bigpool/fmn062','fmn063,/bigpool/fmn063','fmn064,/bigpool/fmn064','fmn065,/bigpool/fmn065',\
               'fmn066,/bigpool/fmn066,gif' ]
    
    for item in savegroup:
       db.execute("INSERT INTO savegroups (savegroup,channel)VALUES(%s,%s)",item,Name)

    for item in Exts:
       db.execute("INSERT INTO exts (ext,channel)VALUES(%s,%s)",item,Name)   


    db.execute("INSERT INTO channelinfo (channel,minwidth,minheight,maxfilesize)VALUES(%s,%s,%s,%s)",\
        Name,int(Minwidth),int(Minheight),int(Maxfilesize))

     
     

def MakeImageDefaultChannel(Format,Name,Exts,Minwidth,Minheight,Maxfilesize):
    host="10.7.18.21:3306"
    database="web" 
    password=""
    user="root"
    db=tornado.database.Connection(
    host=host, database=database,
    user=user,password=password)
    
    tmp=db.query("SELECT * FROM channelinfo WHERE channel=%s",Name)
    if tmp:
      return False

    savegroup=['fmn056,/bigpool/fmn056','fmn057,/bigpool/fmn057','fmn058,/bigpool/fmn058','fmn059,/bigpool/fmn059','fmn060,/bigpool/fmn060',\
               'fmn061,/bigpool/fmn061','fmn062,/bigpool/fmn062','fmn063,/bigpool/fmn063','fmn064,/bigpool/fmn064','fmn065,/bigpool/fmn065',\
               'fmn066,/bigpool/fmn066,gif' ]
    
    exts=['jpg','jpeg','png','gif','bmp']
    
    t_nameprefix=['xlarge','large','main','head','tiny']
    t_nginx_cache=[10,30,60,1440,720]
    t_quality=[90,90,85,80,80]
    print "start sort"
    Format.sort(Compare)
    n=len(Format)
    for i in range(n):
       nginx_cache=t_nginx_cache[i]
       nameprefix=t_nameprefix[i]
       width=list(Format)[i].width
       height=list(Format)[i].height
       crop=list(Format)[i].crop
       quality=list(Format)[i].quality
       samepngtype=list(Format)[i].samepngtype
       samegiftype=list(Format)[i].samegiftype
       sharpen=list(Format)[i].sharpen
       print "start "+str(i)
       db.execute("INSERT INTO handleimages (width,height,nameprefix,crop,quality,samepngtype,samegiftype,nginx_cache,sharpen,channel) \
                  VALUES(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)",width,height,nameprefix,crop,quality,samepngtype,\
                  samegiftype,nginx_cache,sharpen,Name)
    

    print "db save group"
    for item in savegroup:
       db.execute("INSERT INTO savegroups (savegroup,channel)VALUES(%s,%s)",item,Name)

    print Name,Exts,"$$$$$$$$$$$$$$$$$$$$$"
    for item in Exts:
       db.execute("INSERT INTO exts (ext,channel)VALUES(%s,%s)",item,Name)   

    print Minwidth,Minheight,Maxfilesize 
    print db.execute("INSERT INTO channelinfo (channel,minwidth,minheight,maxfilesize)VALUES(%s,%s,%s,%s)",\
        Name,int(Minwidth),int(Minheight),int(Maxfilesize))
    return True 


def ClearDataBase():
    host="10.7.18.21:3306"
    database="web" 
    password=""
    user="root"
    db=tornado.database.Connection(
    host=host, database=database,
    user=user,password=password)
   
    
    db.execute("DELETE FROM service")
    db.execute("DELETE FROM channelinfo")
    db.execute("DELETE FROM handleimages")
    db.execute("DELETE FROM savegroups")
    db.execute("DELETE FROM exts")
  
#    for item in Format:
                  
def Compare(E1,E2):
    if E1.width>=E1.height:
        max1=E1.width
    else:
        max1=E1.height
        
    
    if E2.width>=E2.height:
        max2=E2.width
    else:
        max2=E2.height
    
    if(max1>max2):
      return -1 
    if(max1==max2):
      return 0
    if(max1<max2):
      return 1                    


#ParseService()

a={'pagetype':'zhuyifei','channel':'normal','watermark':'false','markpath':''\
  ,'savesubdir':'','handleexif':'true','checkcomment':'false','outputtype':'json'\
  ,'tickettype':'fix','ticketsuffix':'hello'}

#SaveServiceConfDB(**a)  
#GenServiceConf()


Format=[ImageFormatItem(660,1800,0,85,0,0,0),ImageFormatItem(170,170,1,88,0,0,0),ImageFormatItem(140,140,1,80,0,0,0)]

#print GetChannelFromFormat(Format)
#MakeImageDefaultChannel(Format,"zhuyifei",['bmp','jpg'],1,1,15)
#a=list(Format)
#a.sort(Compare)
#for node in a:
#   print node.width,node.height


#ClearDataBase()
#ParseTemplate()

#ParseService()

#GenTemplateConf()
#GenServiceConf()


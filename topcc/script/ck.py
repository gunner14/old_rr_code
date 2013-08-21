#!/usr/local/bin/python
#coding: utf-8
import os,sys
import getopt
import subprocess
import shlex
import datetime
import send

svnpath = ""
day = ""

def getname(ss):
  path,file = os.path.split(ss)
  print path, file
  s = path.split('/')
  if s and s[0].find('helloworld',2) != 0 and s[0].find('gtest',2) != 0:
    print ">> ",s,"|",s[1:]
    if len(s) == 1:
      return s[0],file
    else:
      freepath = os.path.join(*s[1:])
      return s[0], os.path.join(freepath,file)
  else:
    return ("","")

def runcmd(cmd):
  print "$]",cmd
  p = subprocess.Popen(shlex.shlex(cmd), stdout=subprocess.PIPE, stderr = subprocess.PIPE)
  ret = p.wait()
  return (ret, p.stdout.read(), p.stderr.read())

def cpplint_it(fs):
  ss = ""
  for f in fs:
    stdout = os.popen("python ../../cpplint.py "+ f + " 2>&1").read()
    #ss += f
    #ss += '-----\n'
    ss += stdout
    ss += '\n'
  return ss

def gtest_leak():
  stdout= os.popen("valgrind --tool=memcheck --leak-check=yes ./start_test 2>&1").read()
  return stdout

def gcov_it(fs):
  ss = ""
  for f in fs:
    print "#]","gcov "+ f + " | grep -A 1 " + f
    stdout = os.popen("gcov "+ f + " | grep -A 1 " + f).read()
    print "<", stdout
    ss += stdout
  return ss

def build_target():
  if os.path.exists("Makefile"):
    f = open("Makefile")
    ss = f.read()
    f.close()
    if ss.find("-O2") > 0 or ss.find("-O3") > 0 or ss.find("-O") > 0:
      return (88,"don't change Makefile about O2/O3 option!","")
    if ss.find("-g") < 0:
      return (88,"don't change Makefile about -g option!","")
    rets = runcmd("make")
    return rets
  else:
    return (88,"no find Makefile, C/C++ program?","")
  
def report_log(u,build_out,style_out,leak_out,gcov_out):
  ss=("email: "+ u + "@renren-inc.com \nsvn: http://svn.d.xiaonei.com/newbee/"+u)
  ss+=("\n=====build=====\n")
  ss+=(build_out);
  ss+=("\n=====google code style check=====\n")
  ss+=(style_out);
  ss+=("\n=====gtest and memory leak check=====\n")
  ss+=(leak_out);
  ss+=("\n=====code coverage=====\n")
  ss+=(gcov_out);
  ss+=("=======================\n")
  f = open(u+'.'+day+'.txt', 'w')
  if not f:
    return 
  f.write(ss)
  f.close()
  return ss

def is_cfile(f):
  if f.endswith('.c') or f.endswith('.cc') or f.endswith('.cpp') or f.endswith('.h') or f.endswith('.hpp'):
    return True
  else:
    return False 
def usage():
  print "svn google style check tool, so \n  python ck.py svnpath\n"
  sys.exit()

if __name__ == "__main__":
  #// TODO:test
  os.system('rm -rf newbee/qinghui.liu newbee/sanfeng.zhang')

  now = datetime.datetime.now()
  day = now.strftime("%Y-%m-%d")
  username = {}
  opts,args = getopt.getopt(sys.argv[1:],
                            "h",
                            ["svnpath="])
  if not opts:
    usage()
  for op, value in opts:
    if op == "-h":
      usage()
    elif op == '--svnpath':
      svnpath = value
    else:
      usage()
  print "now ... svn up " + svnpath
  os.chdir(svnpath)
  f = os.popen("svn up")
  ss = f.readlines();
  if len(ss) == 1:
    if ss[0].startswith("At revision "):
      print "no update"
    else:
      print "error:",ss[0]
    sys.exit()
#A    deply/testing/gtest_mac.mm
#U    topcc/face_cluster/action_cluster.cc
#Updated to revision 78433. 
  if ss[len(ss)-1].find("revision ") > 0:
    for s in ss:
      ps = s.split()
      print ps
      if ps[0] != 'C' and ps[0] != 'E' and ps[0] != 'D'and os.path.isfile(ps[1]) and is_cfile(ps[1]):
        un,f = getname(ps[1])
        if un:
          if not username.has_key(un):
            username[un] = []
          username[un].append(f)
    print "dict ",username
    ss = ""
    for u,fs in username.items():
      style_out = ""
      leak_out = ""
      gcov_out = ""
      os.chdir(u)
      (ret,build_out,build_err) = build_target()
      if ret != 0:
        print "compile error!"
        os.chdir("..")
        ss += report_log(u, build_out+build_err, style_out, leak_out, gcov_out)
      else:
        print "compile ok!"
        style_out = cpplint_it(fs)
        leak_out = gtest_leak()
        gcov_out = gcov_it(fs)
        os.chdir("..")
        ss += report_log(u, build_out, style_out, leak_out, gcov_out)
    # send mail
    if len(username):
      #ss = "some newbee commit code,please check it!\n\n"
      authInfo = {}  
      authInfo['server'] = 'smtp.renren.com'  
      authInfo['user'] = 'photo@renren.com'  
      authInfo['password'] = 'photo_sparta_1207'
      fromAdd = 'photo@renren.com'  
      #toAdd = ['qinghui.liu@renren-inc.com']  
      toAdd = ['renren.techcommittee@renren-inc.com']  
      subject = '['+ day + ']有以下'+str(len(username)) + '位C++同学提交代码,请求svn权限,请审核'
      plainText = ss  
      htmlText = None# '<B>HTML文本</B>'  
      send.sendEmail(authInfo, fromAdd, toAdd, subject, plainText, htmlText)  

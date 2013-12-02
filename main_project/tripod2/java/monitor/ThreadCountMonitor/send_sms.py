#!/usr/bin/python

'''
python version for sending sms 
Author: liyong@renren-inc.com
'''

import getopt
import sys
import urllib
import httplib


def Usage():
  print 'Usage: python send_sms.py [required_options] message_content' 
  print '  -c, --contact=<number>      phone number'


def Main():
  try:
    opts, args = getopt.getopt(sys.argv[1:], 'hc:', ['help', '--contact=']);
  except getopt.GetoptError, err:
    print str(err)
    Usage()
    sys.exit(2)

  phone_number = None
  for o, a in opts:
    if o in ('-h', '--help'):
      Usage();
      sys.exit(0)
    elif o in ('-c', '--contact'):
      phone_number = a

  if not phone_number:
    Usage() 
    sys.exit(0)

  message_content = ' '.join(args)
 
  if not message_content:
    Usage()
    sys.exit(0)
  url_str = 'http://sms.notify.d.xiaonei.com:2000/receiver?number=%s&message=%s' % (phone_number, urllib.quote(message_content))
  try:
    res = urllib.urlopen(url_str);
    print res.read()
    res.close()
  except IOERROR, e:
    print e
  

if __name__ == '__main__':
  Main()

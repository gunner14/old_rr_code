#!/usr/local/bin/python
# -*- coding: utf8 -*-

import socket
import sys
from xoa2_admin import Xoa2Admin

zk_addr = "10.11.21.185:2181,10.11.21.186:2181,10.3.18.186:2181/xoa2"
#zk_addr = "10.3.22.60:2181,10.3.22.59:2181,10.3.22.63:2181,10.3.22.62:2181,10.3.22.61:2181/xoa2"
admin = Xoa2Admin(zk_addr)
service_ids = admin.ListService()
for id in service_ids:
  print '----------------------------------------'
  admin.ListService(id)

import pymongo
import datetime
import sys
import re

def Str2Datetime(timestr):
    units = timestr.split('-')
    if len(units) == 4:
        return datetime.datetime(int(units[0]), int(units[1]), int(units[2]),int(units[3]))
    else:
        raise Exception,'TimeFormatError'
    
class DBApi:
    def __init__(self,host,port,database):
        self.conn = pymongo.Connection(host, port) 
        self.db= self.conn[database]

    def GetCaches(self):
        biz_col = self.db['entry']
        items = []
        for doc in biz_col.find({'entry': re.compile('cc.*')}).sort('entry'):
            item = doc['entry'] 
            items.append(item)
        return items 
    
    def GetBizes(self):
        biz_col = self.db['entry']
        items = []
        for doc in biz_col.find({'entry': re.compile(r'^[^c][^c].*')}).sort('entry'):
            item = doc['entry'] 
            items.append(item)
        return items 
 
    def ReadData(self, entry, ops, begintimestr, endtimestr):
        col = self.db[entry]
        begintime= Str2Datetime(begintimestr)
        endtime= Str2Datetime(endtimestr)
        if begintime > endtime:
            return []
        items = []
        for doc in col.find({'time': {'$gte': begintime,'$lte': endtime}},{'_id':0}).sort('time'):
            item = {}
            item['time'] = doc['time'] 
            for op in ops:
                try:
                    item[op] = doc[op]           
                except KeyError:
                    item[op] = 0
            items.append(item)
        return items 
        
    def WriteData(self, entry, op, timestr, number):
        biz_col = self.db['entry']
        biz_doc = biz_col.find_one({ 'entry': entry })
        if not biz_doc:
            biz_col.insert({'entry': entry })
        data_col = self.db[entry]
        data_doc = data_col.find_one({'time': Str2Datetime(timestr) })
        if not data_doc:
            data_col.insert({'time': Str2Datetime(timestr), op:number })
        else:
            data_doc[op]= number
            data_col.save(data_doc)

conndb =  DBApi('stat0.xce.d.xiaonei.com',27017,'tripod_report')

if __name__ == '__main__':
  argc=len(sys.argv)
  if argc < 5:
    print "Usage: mongodb_api.py BusinessEntry OperationCategory TimeHour Count"
  else:
    business_entry = sys.argv[1]
    operation_category = sys.argv[2]
    time = sys.argv[3]
    count = sys.argv[4]
    print "WriteData(%s %s %s %s)" %(business_entry,operation_category,time,count)
    conndb.WriteData(business_entry,operation_category,time,count)
    

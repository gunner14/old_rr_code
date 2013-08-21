#include "vldb_server.h"
#include "bson/db/json.h"
#include "net/bufferarray.h"

using namespace net;

const int kCannotOpenDb =  6001;
const int kCannotAddDataDb = 6002;
const int kCannotGetPageData = 6003;
const int kCannotGetItemData = 6004;
const int kNoExistItemData = 6005;
const int kCannotGetPageSize = 6006;
const int k = 6007;

//START_FUNC(VldbSession)
  bool _VldbSession_init_call_map() { \
    std::map<std::string, VldbSession::CallFunc> &mm = VldbSession::_call_map_;  \
    std::map<std::string, VldbSession::CallFunc3> &m3 = VldbSession::_call3_map_; \
    std::map<std::string, VldbSession::CallFunc4> &m4 = VldbSession::_call4_map_; \
    //FUNCITEM("/storage/open", VldbSession::OnOpen)
    //FUNCITEM("/storage/close", VldbSession::OnClose)
    //FUNCITEM("/storage/get-size", VldbSession::OnGetSize)
    //FUNCITEM("/storage/add", VldbSession::OnAdd)
    //FUNCITEM("/storage/get-page", VldbSession::OnGetPage)
    //FUNCITEM("/storage/get", VldbSession::OnGet)
    //FUNCITEM("/storage/get-page-size", VldbSession::OnGetPageSize)
    //FUNCITEM("/storage/remove-page", VldbSession::OnRemovePage)
    //FUNCITEM("/storage/remove-item", VldbSession::OnRemoveItem)

    mm.insert(std::make_pair("/storage/open", &VldbSession::OnOpen));
    mm.insert(std::make_pair("/storage/close", &VldbSession::OnClose));
    mm.insert(std::make_pair("/storage/get-size", &VldbSession::OnGetSize));
    m4.insert(std::make_pair("/storage/add", &VldbSession::OnAdd));
    mm.insert(std::make_pair("/storage/get-page", &VldbSession::OnGetPage));
    mm.insert(std::make_pair("/storage/get", &VldbSession::OnGet));
    mm.insert(std::make_pair("/storage/get-page-size", &VldbSession::OnGetPageSize));
    mm.insert(std::make_pair("/storage/remove-page", &VldbSession::OnRemovePage));
    mm.insert(std::make_pair("/storage/remove-item", &VldbSession::OnRemoveItem));


//END_FUNC(VldbSession)
    return true;
  } 
  std::map<std::string, VldbSession::CallFunc> VldbSession::_call_map_; 
  std::map<std::string, VldbSession::CallFunc3> VldbSession::_call3_map_; 
  std::map<std::string, VldbSession::CallFunc4> VldbSession::_call4_map_; 
  bool VldbSession::_call_once_init_ret_ = _VldbSession_init_call_map();
  void VldbSession::OnRequest(SpdyConnector &context,
                       spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock &block,
                       const std::string &path, const std::map<std::string,std::string> &param,
                       const char* data, int length) { 
    std::map<std::string, CallFunc>::iterator it = _call_map_.find(path);
    if (it != _call_map_.end()) {
      (this->*(it->second)) (id, param);
      return;
    }

    // 原型函数
    std::map<std::string, CallFunc4>::iterator it4 = _call4_map_.find(path);
    if (it4 != _call4_map_.end()) {
      (this->*(it4->second)) (id, param, data, length); 
      return;
    }

    // 解析json函数
    std::map<std::string, CallFunc3>::iterator it3 = _call3_map_.find(path);
    if (it3 != _call3_map_.end()) {
      if (data) {
        // parse "Accept: text/plain,application/serializable,application/protoc-buff,application/json"
        bson::bo bs = mongo::fromjson(data, &length);
        (this->*(it3->second)) (id, param, bs); 
      } else {
        // 错误的客户端调用,返回错误呢,还是不管呢
      }
    }
    // 错误的客户端调用,返回错误呢,还是不管呢
  }

std::string GetStr(const std::map<std::string, std::string> &param, const std::string &key) {
  std::map<std::string, std::string>::const_iterator dbit = param.find(key);
  if(dbit == param.end())
    return "";
  return dbit->second;
}

int GetInt(const std::map<std::string, std::string> &param, const std::string &key) {
  std::map<std::string, std::string>::const_iterator dbit = param.find(key);
  if(dbit == param.end())
    return -1;
  if (dbit->second.empty() || dbit->second[0] < '0' || dbit->second[0] > '9')
    return -1;
  int dbid = atoi(dbit->second.c_str());
  return dbid;
}

long GetLong(const std::map<std::string, std::string> &param, const std::string &key) {
  std::map<std::string, std::string>::const_iterator dbit = param.find(key);
  if(dbit == param.end())
    return -1;
  if (dbit->second.empty() || dbit->second[0] < '0' || dbit->second[0] > '9')
    return -1;
  long dbid = atol(dbit->second.c_str());
  return dbid;
}

arch::Index* VldbSession::VerifyDbOpened(int id, StorageInfo **si) {
  arch::Index *ptr = NULL;
  std::map<int, StorageInfo*>::iterator it = storage_map_.find(id);
  if (it == storage_map_.end()) {
    // TODO: 是否需要询问configserver是否同意打开
    StorageInfo *sit = new StorageInfo();
    int res = sit->df.Open(id, "/mnt/mfs", true);
    if (res < 0) {
      delete sit;
      return NULL;
    }
    sit->last = time(NULL);
    sit->idx = sit->df.index();
    if (si)
      *si = sit;
    storage_map_.insert(std::make_pair(id, sit));
    return sit->idx;
  } else {
    return it->second->idx;
  }

}

void VldbSession::OnOpen(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param) {
  int dbid = GetInt(param, "dbid");
  arch::Index *idx = VerifyDbOpened(dbid);
  if(!idx) 
    Put510Error(id, kCannotOpenDb, "don\'t open vldb");
  else
    Put200Ok(id, -1);
}

void VldbSession::Put200Ok(spdy::SpdyStreamId id, long ret) {
  char str[128]="";
  if (ret != -1)
    snprintf(str, sizeof(str) - 1, "{ return: %l }", ret);
  PutBson(id, "200 OK", str);
}

void VldbSession::Put510Error(spdy::SpdyStreamId id, int errorcode, const std::string &msg) {
  char str[1024];
  snprintf(str, sizeof(str) -1, "{ errorCode: %d, message: %s }", errorcode, msg.c_str());
  //mongo::bo x = bob().append("errorCode", str).append("message", msg).obj();
  //std::string s = x.toString();
  PutBson(id, "510 Error", str);
}

void VldbSession::OnClose(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param) {
  std::map<int, StorageInfo*>::iterator it = storage_map_.find(id);
  if (it != storage_map_.end()) {
    it->second->df.Close();
    delete it->second;
    storage_map_.erase(it);
  }
  Put200Ok(id, -1);
}

void VldbSession::OnGetSize(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param) {
  int dbid = GetInt(param, "dbid");
  StorageInfo *si;
  arch::Index *idx = VerifyDbOpened(dbid, &si);
  if(!idx) {
    Put510Error(id, kCannotOpenDb, "don\'t open vldb");
  } else {
    int64 sz = si->df.size();
    Put200Ok(id, sz);
  }
}

void VldbSession::OnAdd(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param, const char* data, int length) {
  int dbid = GetInt(param, "dbid");
  arch::PageId pid = GetInt(param, "pageid");
  arch::ItemId iid = GetLong(param, "itemid");
  StorageInfo *si;
  arch::Index *idx = VerifyDbOpened(dbid, &si);
  if(!idx) {
    Put510Error(id, kCannotOpenDb, "don\'t open vldb");
  } else {
    int res = idx->Add(pid, iid, std::string(data, length));
    if (res == -1)
      Put510Error(id, kCannotAddDataDb, "don\'t add data on vldb");
    else
      Put200Ok(id, -1);
  }
}

void VldbSession::OnGetPage(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param) {

  int dbid = GetInt(param, "dbid");
  arch::PageId pid = GetInt(param, "pageid");
  arch::ItemId index = GetLong(param, "index");
  StorageInfo *si;
  arch::Index *idx = VerifyDbOpened(dbid, &si);
  if(!idx) {
    Put510Error(id, kCannotOpenDb, "don\'t open vldb");
  } else {
    std::vector<std::pair<arch::ItemId, std::string> > values;
    int count = idx->GetPage(pid, index, &values);
    if (count == -1)
      Put510Error(id, kCannotGetPageData, "don\'t get page data on vldb");
    else {
      // like { number:23, items:[ {21,{value1}},{22,{value2}},{27,{value3}} ] }
      char str[128];
      arch::BufferArray ba(count * 512);
      int sz = snprintf(str, sizeof(str) -1, "{ number:%d, items:[", count);
      ba.Append(str, sz);
      for (std::vector<std::pair<arch::ItemId, std::string> >::iterator it = values.begin(); it != values.end(); it++) {
        sz = snprintf(str, sizeof(str) -1, "{%l,", (int64)it->first);
        ba.Append(str, sz);
        ba.Append(it->second.c_str(), it->second.size());
        ba.Append("},", 3); // 最后的逗号,有影响吗
      }
      ba.Append("] }", 3);
      PutBson(id, "200 OK", std::string(ba.ptr(), ba.size()));
    }
  }
}

void VldbSession::OnGet(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param) {

  int dbid = GetInt(param, "dbid");
  arch::PageId pid = GetInt(param, "pageid");
  arch::ItemId iid = GetLong(param, "itemid");
  StorageInfo *si;
  arch::Index *idx = VerifyDbOpened(dbid, &si);
  if(!idx) {
      Put510Error(id, kCannotGetPageData, "don\'t get page data on vldb");
  } else {
    std::string value;
    int flag = idx->Get(pid, iid, &value);
    if (flag == -1)
      Put510Error(id, kCannotGetItemData, "don\'t get item data on vldb");
    else if (flag == 256)
      Put510Error(id, kNoExistItemData, "no exist the item on vldb");
    else {
      PutBson(id, "200 OK", value);
    }
  }
}

void VldbSession::OnGetPageSize(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param) {

  int dbid = GetInt(param, "dbid");
  arch::PageId pid = GetInt(param, "pageid");
  StorageInfo *si;
  arch::Index *idx = VerifyDbOpened(dbid, &si);
  if(!idx) {
    Put510Error(id, kCannotOpenDb, "don\'t open vldb");
  } else {
    //arch::PageTable *pt;
    //int count = idx->GetPageSize(pid, &pt);
    int count = idx->GetPageSize(pid);
    if (count == -1) {
      Put510Error(id, kCannotGetPageSize, "don\'t get page size on vldb");
    } else {
      Put200Ok(id, count);
    }
  }
}

void VldbSession::OnRemovePage(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param) {

  int dbid = GetInt(param, "dbid");
  arch::PageId pid = GetInt(param, "pageid");
  StorageInfo *si;
  arch::Index *idx = VerifyDbOpened(dbid, &si);
  if(!idx) {
    Put510Error(id, kCannotOpenDb, "don\'t open vldb");
  } else {
    idx->RemovePage(pid);
    Put200Ok(id, -1);
  }
}

void VldbSession::OnRemoveItem(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param) {

  int dbid = GetInt(param, "dbid");
  arch::ItemId iid = GetInt(param, "itemid");
  StorageInfo *si;
  arch::Index *idx = VerifyDbOpened(dbid, &si);
  if(!idx) {
    Put510Error(id, kCannotOpenDb, "don\'t open vldb");
  } else {
    idx->RemoveItem(iid);
    Put200Ok(id, -1);
  }
}

int main() {
  
  printf("start run vldb server\n");
  SpdyServer server;
  VldbSession ss(server.io_service()); // only used for clone
  server.Set(&ss);
  server.Listen(std::make_pair("0.0.0.0", "3001"));
  server.Run();
  return 0;
}

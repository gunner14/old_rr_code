#include "dbc_client.h"
#include "net/spdy_client.h"

//using namespace entry;

using namespace xoa;
// test
void TestGetEntry(XoaClient &client) {
    char str[20];
    for(int i = 22000; i < 22001; ++i) {
      Method mod = Method::Get("xoa://page.xoa.renren.com/entry/{gid}");
      sprintf(str, "%d", i);
      mod.SetParam("gid", str);

      XoaResponse resp;
      bool b = client.Execute(mod, &resp, 1000);
      
    if (b) {
      entry::Entry y;
      entry::JsonToEntry(resp.content_, &y);
      std::cout << "response status:" << resp.status_  
                << " content type:" << resp.content_type_ << "\ncontent:" << resp.content_ 
                << "\ngid: " << y.id_ << " vector.size: " << y.fields_.size()<< std::endl;

    }
    else
      std::cout << "update error! status:" << resp.status_ << " error:" << resp.error_ << std::endl;
    }
}

void TestGetsEntry(XoaClient &client) {
    char str[20];
    Method mod = Method::Get("xoa://page.xoa.renren.com/entry");
    for(int i = 22000; i < 22005; ++i) {
      sprintf(str, "%d", i);
      mod.SetParam("gids", str);
    }
    std::cout << "gets:" << mod.url() << std::endl;
    XoaResponse resp;
    bool b = client.Execute(mod, &resp, 1000);

    if (b) {
      entry::Entry y;
      entry::JsonToEntry(resp.content_, &y);
      std::cout << "response status:" << resp.status_  
                << " content type:" << resp.content_type_ << "\ncontent:" << resp.content_ 
                << "\ngid: " << y.id_ << " vector.size: " << y.fields_.size()<< std::endl;

    }
    else
      std::cout << "update error! status:" << resp.status_ << " error:" << resp.error_ << std::endl;
}

void TestFindEntry(XoaClient &client) {
    char str[20];
    Method mod = Method::Get("xoa://page.xoa.renren.com/entry/find");
    mod.SetParam("query", "tag:4888 and author:888");
    mod.SetParam("limit", "2");

    XoaResponse resp;
    bool b = client.Execute(mod, &resp, 1000);

    if (b) {
      entry::FindGids f;
      //entry::JsonToEntry(resp.content_, &y);
      entry::JsonToFindGids(resp.content_, &f);
      std::cout << "response status:" << resp.status_  
                << " content type:" << resp.content_type_ << "\ncontent:" << resp.content_ 
                << "\ncursor: " << f.cursor_ << " vector.size: " << f.ids_.size()<< std::endl;

    }
    else
      std::cout << "update error! status:" << resp.status_ << " error:" << resp.error_ << std::endl;
}

void TestUpdateEntry(XoaClient &client) {

  char str[20];
  for(int i = 22000; i < 22001; ++i) {
    Method mod = Method::Put("xoa://page.xoa.renren.com/entry/{gid}");
    std::string json;
    sprintf(str, "%d", i);
    mod.SetParam("gid", str);
    entry::Field f1; f1.key_ = "author"; f1.value_ = "5888"; f1.index_ = true;
    entry::Field f2; f2.key_ = "tag"; f2.value_ = "4888"; f2.index_ = true;
    entry::Field f3; f3.key_ = "tag"; f3.value_ = "3888"; f3.index_ = true;
    std::vector<entry::Field> m;
    m.push_back(f1); m.push_back(f2); m.push_back(f3);

    entry::FieldsToJson(m, &json);
    mod.SetParam("fields", json);

    XoaResponse resp;
    bool b = client.Execute(mod, &resp, 1000);
    
    if (b)
      std::cout << "response status:" << resp.status_  
                << " content type:" << resp.content_type_ << "\ncontent:" << resp.content_ << std::endl;
    else
      std::cout << "update error! status:" << resp.status_ << " error:" << resp.error_ << std::endl;
  }
}

void TestSaveEntry(XoaClient &client) {
  char str[20];
  for(int i = 22000; i < 22005; ++i) {
    Method mod = Method::Post("xoa://page.xoa.renren.com/entry");
    std::string json;
    sprintf(str, "%d", i);
    mod.SetParam("gid", str);
    entry::Field f1; f1.key_ = "author"; f1.value_ = "888"; f1.index_ = true;
    entry::Field f2; f2.key_ = "tag"; f2.value_ = "4888"; f2.index_ = true;
    entry::Field f3; f3.key_ = "tag"; f3.value_ = "3888"; f3.index_ = true;
    std::vector<entry::Field> m;
    m.push_back(f1); m.push_back(f2); m.push_back(f3);

    entry::FieldsToJson(m, &json);
    std::cout << "test put data:" << i << " json:" << json << std::endl; // TODO:test

    mod.SetParam("fields", json);

    XoaResponse resp;
    bool b = client.Execute(mod, &resp, 1000);
    
    if (b)
      std::cout << "response status:" << resp.status_  
                << " content type:" << resp.content_type_ << "\ncontent:" << resp.content_ << std::endl;
    else
      std::cout << "update error! status:" << resp.status_ << " error:" << resp.error_ << std::endl;
  }
}

void TestDeleleEntry(XoaClient &client) {
  char str[20];
  for(int i = 22000; i < 22001; ++i) {
    Method mod = Method::Delete("xoa://page.xoa.renren.com/entry/{gid}");
    sprintf(str, "%d", i);
    mod.SetParam("gid", str);

    XoaResponse resp;
    bool b = client.Execute(mod, &resp, 1000);
    
    if (b)
      std::cout << "response status:" << resp.status_  
                << " content type:" << resp.content_type_ << "\ncontent:" << resp.content_ << std::endl;
    else
      std::cout << "update error! status:" << resp.status_ << " error:" << resp.error_ << std::endl;
  }
}

int main() {

  //arch::LogToDir("log", arch::LS_INFO, 20*3600);
  arch::LogMessage::LogToDebug(arch::LS_VERBOSE);
  std::cout << "start log" << std::endl;

  //ZookeeperBasedRegistry reg("10.3.22.60:2181,10.3.22.59:2181,10.3.22.63:2181,10.3.22.62:2181,10.3.22.61:2181", "/xoa");
  SimpleXoaRegistry reg;
  reg.Register("page.xoa.renren.com", "10.3.18.181", 8188);
  {
    XoaClient client(&reg);
    std::cout << "save..." << std::endl;
    TestSaveEntry(client);
    std::cout << "get..." << std::endl;
    TestGetEntry(client);
    /*std::cout << "update..." << std::endl;
    TestUpdateEntry(client);
    std::cout << "get..." << std::endl;
    TestGetEntry(client);
    std::cout << "delete..." << std::endl;
    TestDeleleEntry(client);*/
    std::cout << "gets..." << std::endl;
    TestGetsEntry(client);
    TestFindEntry(client);
  }
  std::cout << "end log" << std::endl;

  return 0;
}

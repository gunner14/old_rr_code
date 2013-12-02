#include "xoa2/client/zk_client.h"
#include "LogWrapper.h"

#include <iostream>
using namespace std;

using namespace xoa2::client;

class TestListener : public ZkEventListener {
 private:
  int id_;

 public:
  TestListener(int id) : id_(id) {
  }

  virtual void HandleNodeEvent(const char * path, const std::string & value) {
    cout << "TestListener" << id_ << " v=" << value << ",path=" << path << endl;
  }
  virtual void HandleChildEvent(const char * path, const std::vector<std::string> & children) {
    cout << "TestListener" << id_ << " child=" << children.size() << ",path=" << path << endl;
  }
};

typedef boost::shared_ptr<TestListener> TestListenerPtr;


int main(int argc, char argv) {
  MCE_INIT("./zk_test.log", "DEBUG");
  ZookeeperClient & client = ZookeeperClient::Instance();
  string value;
  vector<string> children;

  // client.Create("/com/renren/xoa/demo/cppclient_test", string("status=enabled"), true);
  // client.GetValue("/com/renren/xoa/demo/test/.service_group_0", &value);
  // client.GetChildren("/com/renren/xoa/demo/test/.service_group_0", &children);

  vector<TestListenerPtr> listeners;
  for (int i = 0; i < 10; ++i) {
    TestListenerPtr l(new TestListener(i));
    client.AddNodeListener("/com/renren/xoa/demo/test/.service_group_0", ZkEventListenerPtr(l));
    listeners.push_back(l); // weak_ptr单独引用 l 会导致释放，故增加 l 的 shared_ptr 引用
  }

  for (int i = 100; i < 110; ++i) {
    TestListenerPtr l(new TestListener(i));
    client.AddChildListener("/com/renren/xoa/demo/test/.service_group_0", ZkEventListenerPtr(l));
    listeners.push_back(l); // weak_ptr单独引用 l 会导致释放，故增加 l 的 shared_ptr 引用
  }
  
  cout << "children size : " << children.size() << endl;
  for(size_t i = 0; i < children.size(); ++i) {
    cout << "\tchild " << i << " : " << children[i] << endl;
  }
  sleep(100000000);
  return 0;
}


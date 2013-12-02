// Copyright 2012-09-10 shiqiang.ding@renren-inc.com

#include <iostream>
#include <string>
#include <vector>
#include "service_registry_accessor.h"
#include "gtest/gtest.h"
using namespace xcs;
class MyListener : public ServiceEventListener {
 public:
  MyListener(int listener_id, ServiceRegistryAccessorPtr accessor)
      : listener_id_(listener_id), ServiceEventListener(accessor) {
  }
  virtual void HandleChildChange(const std::string &version,
                                 const std::string &stat) {
    if (stat == "") {
      std::cout << "version层子节点变化:" << version << std::endl;
      // 考虑给增加的节点增加监听
      std::vector<std::string> children;
      accessor_->ListAndListen("", shared_from_this(), true, &children);
    } else {
      std::cout << "state层子节点变化:" << version << "/" << stat << std::endl;
      // 若是所关心的服务分区（stat），考虑给新增加的服务增加内容监听
      if (stat == "0") {
        accessor_->ContentListen("0", shared_from_this());
      }
    }
  }
  virtual void HandleContentChange(const std::string &version,
                                   const std::string &stat,
                                   const std::string &endpoint) {
    std::cout << "服务器内容变化:" << version << "/" << stat
              << "/" << stat << "/" << endpoint << std::endl;
  }
  virtual void HandleExpiredEvent() {
    std::cout << "session expired!" << std::endl;
    std::cout << "请重新初始化" << std::endl;
    // 考虑会话超时后一些服务需要重新初始化，若是不需要忽略。
  }

 private:
  int listener_id_;
};
typedef boost::shared_ptr<MyListener> MyListenerPtr;

TEST(SERVICEREGISTRYACCESSOR, MEMLEAK) {
  PropertiesConfiguration config("./service_registry_accessor_demo.conf");
  ServiceRegistryAccessorPtr client =
        ServiceRegistryAccessorPtr(new ServiceRegistryAccessor(config, 200));
  MyListenerPtr listener = MyListenerPtr(new MyListener(1, client));

  int rc = client->Connect("feed", "101");
  if (rc) {
    std::cout << "Connect() error: rc= " << rc << std::endl;
  }

  rc = client->AddListenExpired(listener);   // 增加会话超时后的初始化监听
  if (rc) {
    std::cout << "AddListenExpired() error: rc= " << rc << std::endl;
  }

  std::vector<std::string> children;
  // 递归给version层加监听路径为/feed/101
  rc = client->ListAndListen("", listener, true, &children);
  if (rc) {
    std::cout << "ListAndListen() error: rc= " << rc << std::endl;
  } else {
    std::cout << "子节点列表如下：" << std::endl;
    for (int i = 0; i < children.size(); i++) {
      std::cout << children[i] << std::endl;
    }
  }

  // 给0服务分区下的所有服务节点加监听
  rc = client->ContentListen("0", listener);
  if (rc) {
    std::cout << "ContentListen() error: rc= " << rc << std::endl;
  }


  // 获取1服务分区下的所有列表不增加监听
  MyListenerPtr listnull = MyListenerPtr((MyListener *)NULL);
  rc = client->ListAndListen("1", listnull, false, &children);
  if (rc) {
    std::cout << "ListAndListen() error: rc= " << rc << std::endl;
  } else {
    std::cout << "子节点列表如下：" << std::endl;
    for (int i = 0; i < children.size(); i++) {
      std::cout << children[i] << std::endl;
    }
  }
  // 使用zkCli.sh操作zk，查看监听是否起作用
//  sleep(100000);
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


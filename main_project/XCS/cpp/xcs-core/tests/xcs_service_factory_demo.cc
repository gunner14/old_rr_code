// Copyright 2012-09-12 shiqiang.ding@renren-inc.com

#include <iostream>
#include <string>
#include <map>

#include "xcs_service_factory.h"
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

TEST(XCSSERVICEFACTORY, MEMLEAK) {
  MCE_INIT("./factory.log", "DEBUG");
  XcsServiceFactory demo;

  // server角色进行操作
  PropertiesConfiguration config_server("./server.conf");
  ServiceRegistryPtr server = demo.CreateServiceRegistry(config_server, 200);
  xcs::ServiceNode my_node;
  my_node.name_ = "199.199.199.199:1923";
  my_node.content_ = "status=enabled";
  int rc = server->PublishService("feed", "100", "0", my_node);
  if (rc) {
    std::cout << "PublishService() error. rc= " << rc << std::endl;
  }

  // client角色进行操作
  PropertiesConfiguration config_client("./client.conf");
  ServiceRegistryAccessorPtr client = demo.CreateServiceRegistryAccessor(
                                      config_client, 200);
  MyListenerPtr listener = MyListenerPtr(new MyListener(1, client));
  rc = client->Connect("feed", "100");
  if (rc) {
    std::cout << "Connect() error. rc= " << rc << std::endl;
  }
  std::vector<std::string> children;
  rc = client->ListAndListen("", listener, true, &children);
  if (rc) {
    std::cout << "ListAndListen() error: rc= " << rc << std::endl;
  } else {
    std::cout << "子节点列表如下：" << std::endl;
    for (int i = 0; i < children.size(); i++) {
      std::cout << children[i] << std::endl;
    }
  }
  rc = client->ContentListen("0", listener);
  if (rc) {
    std::cout << "ContentListen() error: rc= " << rc << std::endl;
  }
  std::string content;
  rc = client->GetServiceStatus("0", "199.199.199.199:1923", &content);
  if (rc) {
    std::cout << "GetServiceStatus() error: rc= " << rc << std::endl;
  } else {
    std::cout << "服务199.199.199.199:1923的状态为：" << content << std::endl;
  }
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


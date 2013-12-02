// Copyright 2012-8-28 shiqiang.ding@renren-inc.com dinggangyilang@gmail.com
// This is demo which help service-publisher register a service.
// Note that you should create zookeeper node path(/online/feed/100/0).
// Author(shiqiang.ding:123456) owns all authrities.
// Author(server:123456) owns authrities which server-role owns.

#include <iostream>
#include "service_registry.h"
#include "gtest/gtest.h"

using std::cout;
using std::endl;
using std::string;

TEST(SERVICEREGISTRY, MEMLEAK) {
  MCE_INIT("./debug.log", "DEBUG");
  xcs::PropertiesConfiguration config("./service_registry_demo.conf");
  xcs::ServiceRegistryPtr demo = xcs::ServiceRegistryPtr(
      new xcs::ServiceRegistry(config, 200));

  xcs::ServiceNode my_node;
  my_node.name_ = "10.10.10.10:9149";
  my_node.content_ = "status=enabled";

  int rc = demo->PublishService("feed", "100", "0", my_node);
  if (rc) {
    cout << "RegistryService error! rc=" << rc << endl;
  }
  cout << "服务注册成功！" << endl;

  /* 以下操作可以利用zkCli.sh来观测所创建节点的状态(value值和acl值) */

  cout << "请断开网络，过2秒至5秒再连接网络，测试expired之后的重连" << endl;

//  sleep(10000);
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


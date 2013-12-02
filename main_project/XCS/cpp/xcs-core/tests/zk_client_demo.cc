// Copyright 2012-08-28 shiqiang.ding@renren-inc dinggangyilang@gmail.com

#include <iostream>

#include "zk_client.h"
#include "gtest/gtest.h"
#include "UtilCxx/src/LogWrapper.h"

using std::cout;
using std::endl;
using std::string;

class TestListener : public xcs::ZkEventListener {
 private:
  int id_;

 public:
  explicit TestListener(int id) : id_(id) {
  }

  virtual void HandleNodeEvent(const char * path, const std::string & value) {
    cout << "TestListener" << id_ << " v=" << value << ",path=" << path << endl;
  }
  virtual void HandleChildEvent(const char * path,
                                const std::vector<std::string> & children) {
    cout << "TestListener" << id_ << " child=" << children.size()
         << ",path=" << path << endl;
  }
  virtual void HandleExpiredEvent() {
    cout << "TestListener" << id_ << "connection expired!" << endl;
  }
};

typedef boost::shared_ptr<TestListener> TestListenerPtr;

void print_acl(const struct ACL_vector & acl) {
  cout << "acl.count = " << acl.count << endl;
  for (int i = 0; i < acl.count; i++) {
    printf("id = %s;perms = %d\n", acl.data[i].id.id, acl.data[i].perms);
  }
}

TEST(ZKCLIENT, MEMLEAK) {
  MCE_INIT("./zk_test.log", "DEBUG");
  xcs::PropertiesConfiguration config("zk_client_demo.conf");
  xcs::ZookeeperClient & client = xcs::ZookeeperClient::Instance(config, 200);
  string value;
  std::vector<string> children;

  xcs::DigestAuthProvider acl_provider;
  struct ACL_vector acl_out, acl_in;

  int rc;

  // 创建节点
  cout << "********创建节点***********" << endl;
  rc = client.Create("/test", "auth:shiqiang.ding", false);
  if (rc) {
    cout << "创建/test/demo1失败: rc=" << rc << endl;
  }

  rc = client.Create("/test/demo1", "this is demo1", false);
  client.Create("/test/demo2", "this is demo2", true);
  client.Create("/test/demo1/root", "this is root", true);
  cout << "创建节点成功" << endl;

  // ACL操作测试
  cout << "********ACL操作***********" << endl;

  rc = client.GetAcl("/test", &acl_out);
  if (rc) {
    cout << "获取/test的acl失败: rc=" << rc << endl;
  } else {
    cout << "/test的acl列表为:" << endl;
    print_acl(acl_out);
  }

  acl_provider.AddDigestAcl("shiqiang.ding", "123456", "rwcda");
  acl_provider.AddDigestAcl("server", "123456", "r");
  acl_provider.GenerateAclVector(&acl_in);
  client.SetAcl("/test", acl_in, true);  // 递归的增加权限

  acl_provider.DeleteAclVector(acl_in);

  client.GetAcl("/test/demo1/root", &acl_out);
  cout << "/test/demo1/root的acl列表为:" << endl;
  print_acl(acl_out);

  rc = client.GetValue("/test", &value);
  if (rc) {
    cout << "获取/test的value失败: rc=" << rc << endl;
  } else {
    cout << "获取/test的value成功: " << value << endl;
  }
  rc = client.Create("/test/hello", "hello world", true);
  if (rc) {
    cout << "创建/test/hello失败: rc=" << rc << endl;
  } else {
    cout << "创建/test/hello成功" << endl;
  }
  rc = client.Delete("/test", true);  // 递归删除
  if (rc) {
    cout << "删除/test失败: rc=" << rc << endl;
  } else {
    cout << "删除/test成功" << endl;
  }
  // 认证操作
  cout << "*********认证操作***********" << endl;
  client.AddAuthInfo("shiqiang.ding", "123456");
  rc = client.GetValue("/test", &value);
  if (rc) {
    cout << "获取/test的value失败: rc=" << rc << endl;
  } else {
    cout << "获取/test的value成功: " << value << endl;
  }

  rc = client.Update("/test", "修改函数测试");
  rc = client.GetValue("/test", &value);
  cout << "修改之后的value值为：" << value << endl;

  rc = client.Create("/test/hello", "hello world", true);
  if (rc) {
    cout << "创建/test/hello失败: rc=" << rc << endl;
  } else {
    cout << "创建/test/hello成功" << endl;
  }
  // 删除节点操作
  cout << "*********删除操作**********" << endl;
  rc = client.Delete("/test", true);
  if (rc) {
    cout << "删除/test失败: rc=" << rc << endl;
  } else {
    cout << "删除/test成功" << endl;
  }

/*监听操作*/
/*
  cout << "*********监听操作***********" << endl;
  // client.GetValue("/com/renren/xoa/demo/test/.service_group_0", &value);
  // client.GetChildren("/com/renren/xoa/demo/test/.service_group_0", &children);

  vector<TestListenerPtr> listeners;
  for (int i = 0; i < 10; ++i) {
    TestListenerPtr l(new TestListener(i));
    client.AddNodeListener("/test/demo1", ZkEventListenerPtr(l));
    listeners.push_back(l); // weak_ptr单独引用 l 会导致释放，故增加 l 的 shared_ptr 引用
  }

  for (int i = 100; i < 110; ++i) {
    TestListenerPtr l(new TestListener(i));
    client.AddChildListener("/test", ZkEventListenerPtr(l));
    listeners.push_back(l); // weak_ptr单独引用 l 会导致释放，故增加 l 的 shared_ptr 引用
  }
  
  cout << "children size : " << children.size() << endl;
  for(size_t i = 0; i < children.size(); ++i) {
    cout << "\tchild " << i << " : " << children[i] << endl;
  }
*/
//  sleep(100000000);
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


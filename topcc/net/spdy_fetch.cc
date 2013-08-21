#include "net/spdy_client.h"
#include <stdio.h>
#include <map>
#include <iostream>

using namespace xoa;

void fun(const char *buf, int) {
  std::cout << "fun" << std::endl;
}

int main() {

  SimpleXoaRegistry reg;
  reg.Register("blog.xoa.renren.com", "10.22.200.140", 8188);
  XoaClient client(&reg);

  Method mod = Method::Get("xoa://blog.xoa.renren.com/echo");
  mod.SetParam("aaa", "111");
  mod.SetParam("bbb", "222");
  mod.SetParam("aaa", "333");

  XoaResponse resp;
  bool b = client.Execute(mod, &resp, 1000);
  
  if (b)
    std::cout << "content type:" << resp.content_type_ << "\ncontent:" << resp.content_ << std::endl;
  else
    std::cout << "get error!" << std::endl;

  //client.AsyncRequest("GET", "xoa://blog.xoa.renren.com/echo", boost::bind(fun,_1, _2));
  //client.Run();

  // GET, url
  return 0;
}

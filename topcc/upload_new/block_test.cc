#include <stdio.h>
#include <iostream>
#include "blockmanage.h"

std::string pv(const std::vector<int> &losts) {
  char tmp[20];
  std::string str;
  for (std::vector<int>::const_iterator it = losts.begin(); it != losts.end(); it++) {
    sprintf(tmp, "%d,", *it); 
    str += tmp;
  }
  return str;
}

int main() {
  upload::g_blockmanager.Init("conf/blockmemcache.conf");
  std::string alldata;
  std::vector<int> losts;
  int res;
  losts.clear();
  res = upload::g_blockmanager.SetBlock("123", "00100", 1, 3, "1111111111", &alldata, &losts);
  std::cout << "1: " << res << " losts: " << pv(losts) << std::endl;
  losts.clear();
  res = upload::g_blockmanager.SetBlock("123", "00100", 2, 3, "2222222222", &alldata, &losts);
  std::cout << "2: " << res << " losts: " << pv(losts) << std::endl;
  losts.clear();
  res = upload::g_blockmanager.SetBlock("123", "00100", 0, 3, "0000000000", &alldata, &losts);
  std::cout << "0: " << res << " losts: " << pv(losts) << " str: " << alldata << std::endl;
  return 0;
}

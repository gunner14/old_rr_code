#ifndef INTSERIALIZE_H_
#define INTSERIALIZE_H_
#include <iostream>
#include <string>

namespace xce{
namespace journey{
namespace util{
using namespace std;

class IntSerialize{
  public:
    static string serialize(int key) {
      string result;
      result.resize(sizeof(int));
      memcpy((void*)(result.data()), &key, sizeof(int));
      return result;
    }
    static int unserialize(string strKey) {
      int result = 0;
      memcpy((void*)(&result), strKey.data(), strKey.length());
      return result;
    }
};

}}} //xce::journey::util;

#endif

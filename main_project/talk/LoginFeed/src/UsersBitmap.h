#ifndef _USERS_BITMAP_H
#define _USERS_BITMAP_H

#include <boost/shared_ptr.hpp>

namespace xce {
namespace xntalk {

#define BUFFER_SIZE 300*1024*1024
class UsersBitmap {
public:
  /*
  static UsersBitmap &Instance() {
    static UsersBitmap ub;
    return ub;
  }
  */
  UsersBitmap(): bit(NULL),count(0) {
  }
  ~UsersBitmap() {
    if (bit) {
      delete bit;
      bit = NULL;
    }
  }
  bool Initialize() {
    try {
      bit = new char[BUFFER_SIZE];
    } catch (...) {
      return false;
    }
    Clear();
    return true;
  }
  void Clear() {
    for (int i=0; i<BUFFER_SIZE; ++i)
      bit[i] = 0;
    count = 0;
  }
  void Set(int uid) {
    if (!bit)
      return;
    bit[uid/8] |= 1 << uid%8;
    count++;
  }
  bool Get(int uid) {
    if (!bit)
      return false;
    return bit[uid/8] & (1<<uid%8);
  }
  int Size() {
    return count;
  }
private:
  char *bit;
  int count;
};
typedef boost::shared_ptr<UsersBitmap> UsersBitmapPtr;
}
}

#endif

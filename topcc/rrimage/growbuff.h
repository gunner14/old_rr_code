#ifndef _RTIMAGE_GROWBUFF_H_
#define _RTIMAGE_GROWBUFF_H_

namespace rtimage {

// flower operator buffer

class GrowBuffer {
  const static int kHardLimit = 1024 * 1024 * 64;
  const static int kSoftLimit = 1024 * 1024 * 16;
 public:
  GrowBuffer(int sz = kSoftLimit)
             : buf_(new unsigned char[sz])
             , sz_(sz) {}

  unsigned char* ReAlloc(int sz) {
    if (sz > kHardLimit) {
      return 0;
    }
    // try to reclaim memory
    if (sz_ > kSoftLimit && sz <= kSoftLimit) {
      delete []buf_;
      sz_ = kSoftLimit;
      buf_ = new unsigned char [sz_];
      return buf_;
    }
    if (sz > sz_) {
      delete []buf_;
      sz_ = sz;
      buf_ = new unsigned char [sz_];
    }
    return buf_;
  }
  
  unsigned char *ptr() {
    return buf_;
  }

  int size() {
    return sz_;
  }

  ~GrowBuffer() {
    delete []buf_;
  }

 private:
  unsigned char *buf_;
  int sz_;
  GrowBuffer(const GrowBuffer &gb);
  GrowBuffer & operator = (const GrowBuffer &gb);
};

} //namespace

#endif

#ifndef __BUFFERARRAY_H_
#define __BUFFERARRAY_H_

namespace arch {

struct BufferArray {
 public:
  BufferArray(int sz = 128)
  : buf_ ((char*)malloc(sz))
  , len_ (sz)
  , pos_ (0)
  , advice_size_(sz) {
  }
  ~BufferArray() {
    free(buf_);
  }

  void Append(const char *buf, int len) {
    if (len > len_ - pos_) {
      if (len > len_)
        len_ += len;
      else
        len_ += len_;
      buf_ = (char*)realloc(buf_, len_); 
    }
    memcpy(buf_ + pos_, buf, len);
    pos_ += len;
  }

  char* detach_on_exit() {
    char* tmp = buf_;
    buf_ = NULL;
    pos_ = 0;
    len_ = 0;
    return tmp;
  }

  char* ptr(int *len = NULL) const {
    if (len)
      *len = pos_;
    return buf_;
  }
  int max() const {
    return len_;
  }
  int size() const {
    return pos_;
  }
  void Reset(bool realloc = true) {
    if (realloc) {
      if (len_ != advice_size_) {
        free(buf_);
        buf_ = (char*)malloc(advice_size_);
      }
      len_ = advice_size_;
    }
    pos_ = 0;
  }

 private:
  BufferArray(const BufferArray &br);
  BufferArray& operator =(const BufferArray &br);

  char *buf_;
  int len_;
  int pos_;
  int advice_size_;
};


}
#endif

#ifndef _VLDB_UTIL_H_
#define _VLDB_UTIL_H_

namespace arch {
/* unitest
  unsigned char buf[8];
  arch::Pack bp(buf,8);
  bp.Append(0x12,8);
  bp.Append(0x345678,24);
  bp.Append(0x9a,8);
 
  long val;
  arch::UnPack bp1(buf,8);
  bp1.Acquire(8, &val);
  bp1.Acquire(24, &val);
  bp1.Acquire(8, &val);

  bp.Reset();
  bp.Append(0x1,4);
  bp.Append(0x234567,24);
  bp.Append(0x89,8);
  bp.Append(0xa,4);

  arch::UnPack bp2(buf,8);
  bp2.Acquire(4, &val);
  bp2.Acquire(24, &val);
  bp2.Acquire(8, &val);
  bp2.Acquire(4, &val);

  bp.Reset();
  bp.Append(0x1,4);
  bp.Append(0x3,2);
  bp.Append(0x1,2);

  arch::UnPack bp3(buf,8);
  bp3.Acquire(4, &val);
  bp3.Acquire(2, &val);
  bp3.Acquire(2, &val);
*/


struct PackBase {
  PackBase(unsigned char *buf, int size)
    : buf_(buf)
    , size_(size)
    , pos_(0)
    , bit_(0) {}

  bool Skip(int bit) {
    bit_ += bit;
    if (bit_ >= 8) {
      pos_ += (bit_ / 8);
      bit_ %= 8;
    }
    return true;  
  }
  unsigned char *buff(int *byte, int* bit) {
    if (byte)
      *byte = pos_;
    if (bit)
      *bit = bit_;
    return buf_;
  }
protected:
  unsigned char *buf_;
  int size_;
  int pos_;
  int bit_;
};

class Pack : public PackBase{
  public:
    Pack(unsigned char *buf, int size)
      : PackBase(buf, size) {
        memset(buf, 0, size);
    }

    bool Append(long val, int bit) {
      return Put_(val, bit, &pos_, &bit_);
    }

    bool Append(char *str, int size) {
      if (bit_ != 0)
        return false;
      bool b =  memcpy(buf_ + pos_, str, size) != NULL;
      pos_ += size;
      return b;
    }
    inline bool Append64(unsigned long val) {
      if (bit_ != 0)
        return false;
      Append_(val, 64, &pos_);
      return true;
    }
    inline bool Append32(unsigned int val) {
      if (bit_ != 0)
        return false;
      Append_(val, 32, &pos_);
      return true;
    }
    inline bool Append24(unsigned int val) {
      if (bit_ != 0)
        return false;
      Append_(val, 24, &pos_);
      return true;
    }
    inline bool Append16(unsigned short val) {
      if (bit_ != 0)
        return false;
      Append_(val, 16, &pos_);
      return true;
    }
    inline bool Append8(unsigned char val) {
      if (bit_ != 0)
        return false;
      Append_(val, 8, &pos_);
      return true;
    }

    inline bool Put(int startbyte, int startbit, long val, int bit) { // no move current point
      int rpos = startbyte;
      int rbit = startbit;
      return Put_(val, bit, &rpos, &rbit);
    }

    void Reset() {
      memset(buf_,0,size_);
      pos_ = 0;
      bit_ = 0;
    }
  private:
    // bit must % 8 == 0
    template <typename T>
    inline void Append_(T val, int bit, int *rpos) {
      bit-=8;
      for(; bit >= 0; bit-=8,(*rpos)++) {
        buf_[*rpos] = val >> bit;
      }
    }

    bool Put_(long val, int bit, int *rpos, int *rbit) {
      if (!rpos || !rbit)
        return false;

      int free = (8 - *rbit) % 8;
      long newval = val;
      if (*rbit != 0 ) { // no align
        if (bit <= free) {
          buf_[*rpos] &= (0xff << free) | (0xff >> (*rbit + bit));
          buf_[*rpos] |= (val <<  (free - bit));
          *rbit += bit;
          if (bit == free) {
            *rbit = 0;
            (*rpos)++;
          }
          return true;
        } else {
          buf_[*rpos] &= (0xff << free);
          buf_[*rpos] |= (val >> (bit - free));
          int mov_bit = sizeof(long)*8 - (bit - free);
          newval &= ( ((unsigned long)(-1)) >> mov_bit);
          (*rpos)++;
        }
      }

      *rbit = (bit - free) % 8;
      int freeb = (bit - free);
      if (freeb > 7) {// have complete byte
         for (; freeb > 7; freeb -= 8, (*rpos)++) {
          int k = newval & (0xff << (freeb - 8)); // clear other bit
          buf_[*rpos] = k >> (freeb - 8); // shift 
        }
      }
      
      if ( *rbit != 0) { // have free tail
        newval &= 0xff >> *rbit;
        newval <<= (8 - *rbit);
        buf_[*rpos] &= (0xff >> *rbit);
        buf_[*rpos] |= newval;
      }
      return true;
    }
  };

class UnPack : public PackBase {
 public:
   UnPack(unsigned char *buf, int size)
     : PackBase(buf, size) {
    }
  bool Get(int startbyte, int startbit, int bit, long *val) {
    if (!val)
      return false;
    int rpos = startbyte;
    int rbit = startbit;
    return Get_(bit, val, &rpos, &rbit);
  }

  bool Acquire(char *ptr, int size) {
    if (!ptr || !size | bit_)
      return false;
    return memcpy(ptr, buf_ + pos_, size) == 0;
  }

  bool Acquire(int bit, long *val) {
    if (!val)
      return false;
    return Get_(bit, val, &pos_, &bit_);
  }

  inline bool Acquire64(unsigned long *val) {
    if (bit_ != 0)
      return false;
    Acquire_(val, 64, &pos_);
    return true;
  }
  inline bool Acquire32(unsigned int *val) {
    if (bit_ != 0)
      return false;
    Acquire_(val, 32, &pos_);
    return true;
  }
  inline bool Acquire24(unsigned int *val) {
    if (bit_ != 0)
      return false;
    Acquire_(val, 24, &pos_);
    return true;
  }
  inline bool Acquire16(unsigned short *val) {
    if (bit_ != 0)
      return false;
    Acquire_(val, 16, &pos_);
    return true;
  }
  inline bool Acquire8(unsigned char *val) {
    if (bit_ != 0)
      return false;
    Acquire_(val, 8, &pos_);
    return true;
  }
 private:
  // bit must % 8 == 0
  template <typename T>
  inline void Acquire_(T *val, int bit, int *rpos) {
    bit-=8;
    *val = buf_[*rpos];
    (*rpos)++;
    for(; bit > 0; bit-=8,(*rpos)++) {
      *val <<= 8;
      *val |= buf_[*rpos];
    }
  }
  bool Get_(int bit, long *val, int *rpos, int *rbit) {
    if (!rpos || !rbit || !val)
      return false;
    // if bit > sizeof(long): return false
    const int longbit = sizeof(long)*8;
    long newval = 0;
    *val = 0;
    int free = (8 - *rbit) % 8;

    if (*rbit != 0 ) { // no align
      if (bit <= free) {
        *val = buf_[*rpos];
        *val &= (0xff >> (8 - free));
        *val >>= free - bit;
         *rbit += bit;
        if (bit == free) {
          *rbit = 0;
          (*rpos)++;
        }
        return true;
      } else {
        *val = buf_[*rpos] & (0xff >> *rbit); // clear other
        *val <<= (bit - free);
        (*rpos)++;
      }
    }

    *rbit = (bit - free) % 8;
    int freeb = (bit - free);
    if (freeb > 7) { // have complete byte
      newval |= buf_[*rpos];
      (*rpos)++;
      freeb -=8;
      for (;freeb > 7; freeb -= 8,(*rpos)++) {
        newval <<= 8;
        newval |= buf_[*rpos];
      }
    }

    if ( *rbit != 0) { // have free tail
      newval <<= *rbit;
      newval |= buf_[*rpos] >> (8 - *rbit);
    }
    *val |= newval;
    return true;
  }
};


}
#endif

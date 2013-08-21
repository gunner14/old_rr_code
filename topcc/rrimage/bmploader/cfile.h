#ifndef _CFILE_H_
#define _CFILE_H_
#include <string>
#include <fstream>

class CFile {
private:
  std::ifstream file_in_;

public:
  // Flag values
  enum OpenFlags {
    modeRead = (int) 0x00000,
    modeWrite = (int) 0x00001,
    modeReadWrite = (int) 0x00002,
    typeText = (int) 0x04000,
    typeBinary = (int) 0x08000,
  };
  enum SeekPosition {
    begin = 0x0, current = 0x1, end = 0x2
  };
  // Operations
  bool Open(const std::string& filename, unsigned int nOpenFlags) {
    file_in_.open(filename.c_str(), std::ios::in | std::ios::binary);
    return true;
  }
  unsigned long Seek(long lOff, unsigned int nFrom) {
    file_in_.seekg(lOff, (std::_Ios_Seekdir) nFrom);
    return 0;
  }
  unsigned int Read(void* lpBuf, unsigned int nCount) {
    file_in_.read((char*) lpBuf, nCount);
    return 0;
  }
  void Close() {
    file_in_.close();
  }
};
#endif

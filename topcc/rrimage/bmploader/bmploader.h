#ifndef _BMP_LOADER_H_
#define _BMP_LOADER_H_
#include <map>
#include <string>
typedef struct tagPNGData {
  int width;
  int height;
  unsigned char* ippdata;
  int step;
  int channels;
} PNGData;

class PNGManager {
private:
  std::map<std::string, PNGData*> mapPNG;
public:
  const PNGData* GetPNGData(const std::string& filename);
  bool Load(const std::string& path);
};

#endif


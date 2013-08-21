#include "bmploader.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <boost/algorithm/string/predicate.hpp>
#include "ippiImage.h"
#include <map>
#include <istream>
#include "../png.h"
#include "../image.h"

bool GetFileBuffer_Ex(const char *file, char **buf, int* len) {

  if (file == NULL)
    return false;
  std::ifstream ifile(file, std::ios::in | std::ios::binary | std::ios::ate);
  if (!ifile)
    return false;
  *len = ifile.tellg();
  ifile.seekg(0, std::ios::beg);
  *buf = new char[*len];
  if (*buf) {
    ifile.read(*buf, *len);
    ifile.close();
    return true;
  }
  return false;

}

const PNGData* PNGManager::GetPNGData(const std::string& filename) {
  std::map<std::string, PNGData*>::iterator it = mapPNG.find(filename);
  if (it != mapPNG.end()) {
    return it->second;
  }
  return NULL;
}
bool PNGManager::Load(const std::string& path) {
  DIR * dir;
  int count = 0;
  std::string currfile;
  struct dirent *s_dir;
  struct stat file_stat;
  if (path.empty())
    return false;

  dir = opendir(path.c_str());
  while ((s_dir = readdir(dir)) != NULL) {
    if ((strcmp(s_dir->d_name, ".") == 0) || (strcmp(s_dir->d_name, "..") == 0))
      continue;
    currfile = path + s_dir->d_name;
    stat(currfile.c_str(), &file_stat);
    if (S_ISDIR(file_stat.st_mode))
      continue;
    else {
      if (boost::algorithm::ends_with(currfile, ".png")) {
        char *filedata = NULL;
        int filesize = 0;
        int width = 0;
        int height = 0;
        unsigned char* ipp_data = NULL;
        bool b = GetFileBuffer_Ex(currfile.c_str(), &filedata, &filesize);
        rtimage::GrowBufs* grow_bufs = new rtimage::GrowBufs();
        rtimage::ImageHandler* decoder_ = new rtimage::PngHandler(grow_bufs);
        if (decoder_) {

          b = decoder_->Attach(filedata, filesize, &width, &height);
          b = decoder_->Decode(width, height);
          ipp_data = decoder_->GetDecodedPicData();

          PNGData *pngdata = new PNGData;
          if (pngdata) {
            pngdata->channels = 4;
            pngdata->width = width;
            pngdata->height = height;
            pngdata->step = width * pngdata->channels;
            pngdata->ippdata = new unsigned char[pngdata->step * height];
            if (pngdata->ippdata) {
              IppiSize m_srcSize = { pngdata->width, pngdata->height };
              memcpy(pngdata->ippdata, ipp_data, pngdata->step * height);

            }
            mapPNG.insert(std::make_pair(currfile, pngdata));

          }
          delete decoder_;
        }
        delete grow_bufs;

      }
    }
  }
  closedir(dir);
  return mapPNG.size() > 0;
}

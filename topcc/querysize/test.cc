#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <fstream>
#include <boost/filesystem.hpp>
#include "util.h"
#include "photo_head.h"
#include "imagehelper.h"

namespace fs = boost::filesystem;

bool CalcResize(int maxwidth, int maxheight, int w, int h, int *dstwidth, int *dstheight) {
  bool resize = false;
  if (!dstwidth || !dstheight)
    return false;

  *dstheight = h;
  *dstwidth = w;
  if (maxwidth < *dstwidth) {
    *dstwidth = maxwidth;
    *dstheight = h * maxwidth/w;
    resize = true;
  }
  if (maxheight < *dstheight) {
    *dstheight = maxheight;
    *dstwidth = w * maxheight/h;
    resize = true;
  }
  if (resize == true)
    return true;
  return false;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "plz input image file list." << std::endl;
  }
  
  for (int i = 1; i < argc; i++) {
    std::string html,htmlfn;
    char tmp[2000];
    sprintf(tmp, "%d.html", i);
    htmlfn = tmp;

    std::string data = LoadFile(argv[i]);
    int width, height, width0, height0;
    std::string magick;
    CImageHelper::GetSize(data , width, height, magick);
    if ((width == 0 || height == 0 ) && magick.empty()) {
      printf("this image error:%s\n", argv[i]);
      return -1;
    }
    bool resize = CalcResize(720, 720, width, height, &width0, &height0);
    Image image, imageout;
    bool  b = CImageHelper::ReadImage(data, image, 0, 0);
    if (!b) {
      printf("read image error:%s\n", argv[i]);
      return -1;
    }
    image.profile("*",Blob());

    sprintf(tmp, "<img src=\'%s\' /> name=%s|sz=%d <p/>\n", argv[i], argv[i], data.size()); html += tmp;

    b = CImageHelper::ResizeImage(image, width0, height0, imageout);
    for (int j = 70; j < 101; j++) {
      std::string buf;
      b = CImageHelper::ImageData(imageout, j, &buf, "JPEG");
      char* pos = rindex(argv[i], '.');
      std::string fn; 
      if (pos != NULL)
        fn.assign(argv[i], pos - argv[i]);
      sprintf(tmp, "_%d.jpg", j);
      fn += tmp;
      SaveFile(fn, buf);
      sprintf(tmp, "<img src=\'%s\' /> zhilian=%d|sz=%d <br/>\n", fn.c_str(), j, buf.size()); html += tmp;
    }
    SaveFile(htmlfn, html);
  }
  return 0;
}

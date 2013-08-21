#include "action_runimage.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/time.h>
#include "upload/syncdownload.h"
#include "growbuff.h"
#include "base3/logging.h"
#include "base3/ptime.h"
#include "base64.h"
#include "jpg.h"
#include "png.h"
#include "gif.h"
#include "ftstring.h"
#include "watermark_param.h"

namespace runimage {

bool DownImage(const std::string &url, std::string &data) {
  net::SyncDownload sync;
  std::string head;
  std::string str = "download ";
  str += url;
  PTIME(pt1, str, true, 5);
  return 200 == sync.Get(url, &data, &head);
}

bool SaveFileBuffer(const char*file, const char *buf, int len) {
  int f = open(file, O_RDWR | O_CREAT | O_LARGEFILE,
               S_IRUSR | S_IWUSR | S_IRGRP);
  if (f != -1) {
    pwrite(f, buf, len, 0);
    close(f);
    return true;
  }
  return false;
}

bool GetFileGrowBuffer(const char *file, rtimage::GrowBuffer *buf, int *len) {
  bool b = true;
  int f = open(file, O_RDONLY);
  if (f < 0)
    return false;
  if (buf) {
    if (!len) {
      b = false;
    } else {
      //*buf = new char [sz];
      struct stat st;
      fstat(f, &st);
      buf->ReAlloc(st.st_size);
      *len = st.st_size;
      ssize_t res = read(f, buf->ptr(), *len);
      if (res < 0) {
        b = false;
      }
    }
  }
  close(f);
  return b;
}

RunImageAction::RunImageAction()
    : growbufs_(new rtimage::GrowBufs()),
      filedata_(new rtimage::GrowBuffer()),
      jpg_(new rtimage::JpgHandler(growbufs_)),
      png_(new rtimage::PngHandler(growbufs_)),
      gif_(new rtimage::GifHandler()),
      test_(new int),
      watermark_param_(new rtimage::WatermarkParam()) {
}

RunImageAction::~RunImageAction() {
  delete growbufs_;
}

bool RunImageAction::Match(const std::string& url) const {
  return true;
}

void RunImageAction::Stop() {
}

inline char* itoa(char* strnum, int numb) {
  sprintf(strnum, "%d", numb);
  return strnum;
}

std::string& MakeStream(const std::string &type, const unsigned char *buf,
                        int size, std::string *str) {
  char strnum[20];
  itoa(strnum, size);
  *str = "Status: 200\r\n"
      "Content-Type: image/";
  str->append(type);
  str->append("\r\nContent-Length: ");
  str->append(strnum);
  str->append("\r\n\r\n");
  str->append((const char*) buf, size);
  //LOG(INFO) << "make jpg" << size << "data:" << *str;
  return *str;
}
std::string& Make404Stream(std::string &str) {
  str = "Status: 404\r\n\r\n";
  return str;
}

std::string& MakeJsonInfoStream(std::string &str, int w, int h, int sndw,
                                int sndh, int sz) {
  char strnum[20];
  str = "Status: 200\r\n"
      "Content-Type: text/html; charset=utf-8\r\n\r\n"
      "{\"orgwidth\":";
  itoa(strnum, w);
  str += strnum;
  str += ",\"orgheight\":";
  itoa(strnum, h);
  str += strnum;
  str += ",\"nowwidth\":";
  itoa(strnum, sndw);
  str += strnum;
  str += ",\"nowheight\":";
  itoa(strnum, sndh);
  str += strnum;
  str += "} ";
  return str;
}

enum ResizeMode {
  kResizeNull,
  kResizeStretch,  // 无条件拉伸,1
  kResizeZoom,  // 等比缩小,2
  kResizeZoomFix,  // 等比缩放裁剪,3
  kResizeWaterMark,  // 水印,等比缩小,4
  kResizeZoomCut,  // 等比缩小裁剪50*50,5
  kResizeBlankFix,  // 等比缩放留白,6
  kQueryInfo,  // 查询图片的尺寸,7
  kResizeTest = 99,  // 测试用类型
  kResizeMax = kResizeTest,
// 目前仅支持到此 目前仅支持到此
};

enum ImageType {
  kImageTypeNone,
  kImageTypeJpg,
  kImageTypeGif,
  kImageTypePng,
  kImageTypeInfo
};

// 返回其后的pos,错误返回0, val != 0
inline const char* CheckItem(const char *url, char c, int *val) {
  if (url[0] != c)
    return 0;

  if (url[1] == 0 || url[1] < '0' || url[1] > '9') {
    *val = 0;
    return url + 1;
  }
  char *endpos;
  *val = strtol(url + 1, &endpos, 10);
  if (*val == LONG_MIN || *val == LONG_MAX || *val == 0)
    return 0;
  return endpos;
}

// str1 and str2 no null, abcd ,abc 
inline bool BeginWith(const char *str1, const char *str2) {
  const char *pos2 = str2;
  if (*str2 == '\0')
    return false;
  for (const char *pos = str1; *pos != '\0'; ++pos) {
    if (*pos2 == *pos) {
      ++pos2;
      if (*pos2 == '\0')
        return true;
      continue;
    } else {
      return false;
    }
  }
  return false;
}

// str1 and str no null, abcd , bcd
inline bool EndWith(const char *str1, int sz1, const char *str2, int sz2) {
  const char *pos2 = str2 + sz2 - 1;
  if (*str2 == '\0')
    return false;
  for (const char *pos = str1 + sz1 + 1; pos != str1 - 1; --pos) {
    if (*pos2 == *pos) {
      --pos2;
      if (pos2 = str2 - 1)
        return true;
      continue;
    } else {
      return false;
    }
  }
  return false;
}

inline const char* strrchr(const char *str, int sz, char c) {
  for (const char* pos = str + sz - 1; pos != str - 1; --pos) {
    if (*pos == c)
      return pos;
  }
  return 0;
}

// /fmn42/20100912/1940/p/m4w200h150q85lt_xxx.png.jpg 
// jpg png gif
bool ParseImagePath(const std::string &url, char whiteparam[256],
                    char orgpath[4096], std::string& filename,
                    ImageType *orgext, ImageType *nowext) {
  int sz = url.size();
  if (sz < 16)  // /p/m4whqlt_a.jpg
    return false;
  *orgext = kImageTypeNone;
  const char* ptr = url.c_str();

  ptr += (sz - 4);
  if (BeginWith(ptr, ".jpg") == true)
    *nowext = kImageTypeJpg;
  else if (BeginWith(ptr, ".png") == true)
    *nowext = kImageTypePng;
  else if (BeginWith(ptr, ".gif") == true)
    *nowext = kImageTypeGif;
  else if (BeginWith(ptr, ".htm") == true)
    *nowext = kImageTypeInfo;
  else
    return false;

  if (sz > 16 + 4) {
    ptr -= 4;  // /p/m4whqlt_a.jpg.png
    if (BeginWith(ptr, ".jpg") == true)
      *orgext = kImageTypeJpg;
    else if (BeginWith(ptr, ".png") == true)
      *orgext = kImageTypePng;
    else if (BeginWith(ptr, ".gif") == true)
      *orgext = kImageTypeGif;
    if (*orgext == *nowext)
      return false;
    if (*orgext == kImageTypeNone)
      ptr += 4;
  }

  int pathsz = (ptr - url.c_str()) + 4;
  const char *t = strrchr(url.c_str(), pathsz, '/');
  if (!t)
    return false;
  int filesz_noext = ptr - t - 1;  // 1 -> drop "/"
  const char *t1 = strchr(t + 1, '_');  // 1 -> drop "/"
  if (!t1)
    return false;
  if (t1 - t - 1 > 255)
    return false;
  memcpy(whiteparam, t + 1, t1 - t - 1);  // 1 -> drop "/"
  whiteparam[t1 - t - 1] = '\0';

  int path1size = t - url.c_str() - 2 + 1;  // 2 -> /p ,1 -> append "/"
  int path2size = ptr + 4 - t1 - 1;  // 4 -> ".jpg", 1 -> drop "_"
  memcpy(orgpath, url.c_str(), path1size);
  orgpath[path1size] = '\0';
  //memcpy(filename, t1 + 1, path2size);
  //filename[path2size] = '\0';
  filename.insert(0, t1 + 1, path2size);
  return true;
}

bool CalcResize(int maxwidth, int maxheight, int w, int h, int *dstwidth,
                int *dstheight) {
  bool resize = false;
  if (!dstwidth || !dstheight)
    return false;

  *dstheight = h;
  *dstwidth = w;

  if (maxwidth == 0)
    maxwidth = 0x7fffffff;
  if (maxheight == 0)
    maxheight = 0x7fffffff;
  if (maxwidth < *dstwidth) {
    *dstwidth = maxwidth;
    *dstheight = h * maxwidth / w;
    resize = true;
  }
  if (maxheight < *dstheight) {
    *dstheight = maxheight;
    *dstwidth = w * maxheight / h;
    resize = true;
  }
  if (resize == true)
    return true;
  return false;
}

void CalcCrop(int maxwidth, int maxheight, int w, int h, int *dstwidth,
              int *dstheight, int *xoff, int *yoff) {
  if (!dstwidth || !dstheight || !xoff || !yoff)
    return;
  *dstwidth = w;
  *dstheight = h;
  *xoff = 0;
  *yoff = 0;

  if (maxwidth == 0)
    maxwidth = 0x7fffffff;
  if (maxheight == 0)
    maxheight = 0x7fffffff;
  if ((w <= maxwidth) && (h <= maxheight))
    return;
  else if (w <= maxwidth) {  //crop height
    *yoff = (h - maxheight) / 2;
    *dstheight = maxheight;
  } else if (h <= maxheight) {  //crop width
    *xoff = (w - maxwidth) / 2;
    *dstwidth = maxwidth;
  } else {
    if ((double) h / w > (double) maxheight / maxwidth) {
      *dstheight = (maxheight * w) / maxwidth;
      *yoff = (h - *dstheight) / 2;
    } else {
      *dstwidth = (maxwidth * h) / maxheight;
      *xoff = (w - *dstwidth) / 2;
    }
  }
}

// l and t and w and h need no zero
// l, t,w,h为裁剪信息
// w1,h1,为其后的缩放信息
bool CalcSize(ResizeMode mode, int lw, int lh, int width, int height, int *l,
              int *t, int *w, int *h, int *w1, int *h1) {
  *l = 0;
  *t = 0;
  *w = width;
  *h = height;
  if (mode == kResizeStretch) {
    *w1 = lw;
    *h1 = lh;
    return true;
  } else if (mode == kResizeZoomFix) {
    CalcCrop(lw, lh, width, height, w, h, l, t);
    if (lw == width || lh == height) {
      *w1 = *w;
      *h1 = *h;
      return true;
    }
    bool resize = CalcResize(lw, lh, *w, *h, w1, h1);
    return true;
  } else if (mode == kResizeZoom) {
    bool resize = CalcResize(lw, lh, width, height, w1, h1);
    return true;
  } else if (mode == kResizeWaterMark) {
    bool resize = CalcResize(lw, lh, width, height, w1, h1);
    return true;
  } else if (mode == kResizeZoomCut) {
    bool resize = CalcResize(lw, lh, width, height, w1, h1);
    return true;
  } else if (mode == kResizeBlankFix) {
    bool resize = CalcResize(lw, lh, width, height, w1, h1);
    return true;
  } else if (mode == kQueryInfo) {
    bool resize = CalcResize(lw, lh, width, height, w1, h1);
  } else if (kResizeTest) {
    bool resize = CalcResize(lw, lh, width, height, w1, h1);
    return true;
  }
  return false;
}

// 所有参数不能为0,此为用户数据,谨慎解析,目前仅仅解析到t
bool ParseZoomParam(const std::string &main_param, ResizeMode *mode, int *width,
                    int *height, int *quality, int *left, int *top) {
  const char *pos = main_param.c_str();
  int sz = main_param.size();
  if (sz < 7)  // min param: m1whqlt
    return false;

  const char *endpos = pos + sz;

  pos = CheckItem(pos, 'm', (int*) mode);
  if (pos > endpos - 5 || pos == 0)
    return false;

  pos = CheckItem(pos, 'w', width);
  if (pos > endpos - 4 || pos == 0)
    return false;

  pos = CheckItem(pos, 'h', height);
  if (pos > endpos - 3 || pos == 0)
    return false;

  pos = CheckItem(pos, 'q', quality);
  if (pos > endpos - 2 || pos == 0)
    return false;

  pos = CheckItem(pos, 'l', left);
  if (pos > endpos - 1 || pos == 0)
    return false;

  pos = CheckItem(pos, 't', top);
  if (pos == endpos)
    return true;
  else
    return false;
}

double CalcDichotomy(double xf, double yf) {
  double last = 1;
  for (double i = 1.0; i > 0.1; i /= 2) {
    if (xf > i || yf > i) {
      return last;
    }
    last = i;
  }
  return last;
}

HttpStatusCode RunImageAction::Process(Request* request, Response* response) {
  std::string outhtml;
  char orgpath[4096];
  char whiteparam[256];
  ImageType nowext;
  ImageType orgext;
  int dstl, dstt, dstw, dsth, sndw, sndh;
  int size;
  unsigned char *buf = 0;
  bool orgjpg = false;
  int w, h;
  ResizeMode mode;
  int width, height, quality, left, top;
  std::string fullpath;
  std::string water_param;
  rtimage::ImageHandler *image = NULL;
  bool b;
//cjl
  std::string filename;
  int align_type;
  int hor_distance;
  int ver_distance;
  std::vector<rtimage::FreeTypeString> ftstrings;
  std::string logo_name;
  bool dichotomy = false;
  bool should_conv_format = false;
  bool should_resize = false;
  bool out_put_origin = true;
//end

  // 因为无参数区,可以从SCRIPT_NAME获值
  std::string url = request->header("SCRIPT_NAME");
  if (url.size() > sizeof(orgpath) - 1) {  // HEAD LARGE
    outhtml = Make404Stream(outhtml);
    goto END;
  }

  if (!ParseImagePath(url, whiteparam, orgpath, filename, &orgext, &nowext)) {  // NOT IMAGE
    outhtml = Make404Stream(outhtml);
    goto END;
  }

  if (!ParseZoomParam(whiteparam, &mode, &width, &height, &quality, &left,
                      &top)) {
    outhtml = Make404Stream(outhtml);
    goto END;
  }

  if (mode > kResizeMax) {
    outhtml = Make404Stream(outhtml);
    goto END;
  }

  if (mode == kResizeWaterMark) {
    if (orgext == kImageTypePng || orgext == kImageTypeGif
        || nowext == kImageTypePng || nowext == kImageTypeGif) {
      outhtml = Make404Stream(outhtml);
      goto END;
    }

    water_param = filename.substr(0, filename.size() - 4);
    b = watermark_param_->Init(decode(water_param));

    if (!b) {
      outhtml = Make404Stream(outhtml);
      goto END;
    }

    align_type = watermark_param_->GetAlignType();
    hor_distance = watermark_param_->GetHorDistance();
    ver_distance = watermark_param_->GetVerDistance();
    logo_name = watermark_param_->GetLogoFileName();
    filename = watermark_param_->GetFileName();
    b = watermark_param_->MakeFreeTypeString(ftstrings);
    if (-1 == align_type || -1 == hor_distance || -1 == ver_distance
        || logo_name.empty() || filename.empty() || !b) {
      outhtml = Make404Stream(outhtml);
      goto END;
    }

  }

  // 检测扩展名
  fullpath = request->host() + orgpath + filename;
  if (mode == kResizeWaterMark)
    fullpath += ".jpg";
  if (!DownImage(fullpath, filestring_)) {
    outhtml = Make404Stream(outhtml);
    goto END;
    //NOT_FOUND;
  }
  if (orgext == kImageTypeNone) {
    orgext = nowext;
  }

  if (orgext == kImageTypeJpg) {
    image = jpg_.get();
    orgjpg = true;
  } else if (orgext == kImageTypePng) {
    image = png_.get();
  } else if (orgext == kImageTypeGif) {
    // gif resize or convert to jpg
    if (nowext == kImageTypeGif) {
      MakeStream("gif", (const unsigned char*) filestring_.c_str(),
                 filestring_.size(), &outhtml);
      goto END;
    } else if (nowext == kImageTypeJpg) {
      image = gif_.get();
    } else {
      outhtml = Make404Stream(outhtml);
      goto END;
    }
  } else {
    // unsupported image type
    outhtml = Make404Stream(outhtml);
    goto END;
  }

  b = image->Attach((unsigned char*) filestring_.c_str(), filestring_.size(),
                    &w, &h);

  if (false
      == CalcSize(mode, width, height, w, h, &dstl, &dstt, &dstw, &dsth, &sndw,
                  &sndh)) {
    outhtml = Make404Stream(outhtml);
    goto END;
    //NOT_FOUND;
  }
  if (sndw == 0)
    sndw = 1;
  if (sndh == 0)
    sndh = 1;

  if (nowext == kImageTypeInfo) {
    outhtml = MakeJsonInfoStream(outhtml, w, h, sndw, sndh, filestring_.size());
    goto END;
  }

  if (orgjpg == true) {
    double xf = (double) sndw / dstw;
    double yf = (double) sndh / dsth;
    double lv = CalcDichotomy(xf, yf);
    if (lv < 0.6) {
      w *= lv;
      h *= lv;
      dstl *= lv;
      dstt *= lv;
      dstw *= lv;
      dsth *= lv;
      dichotomy = true;
      if (w == 0)
        w = 1;
      if (h == 0)
        h = 1;
      if (dstw == 0)
        dstw = 1;
      if (dsth == 0)
        dsth = 1;
    }
  }

  should_conv_format = orgext != nowext;
  should_resize = dstl != 0 || dstt != 0 || dstw != w || dsth != h || sndw != w
      || sndh != h || dichotomy == true || mode == kResizeTest;

  // out put origin image when no format converting, no resize, no water marker
  // and no sharpen
  out_put_origin = !should_conv_format && !should_resize
      && (mode != kResizeWaterMark) && (left == 0);

  if (out_put_origin) {
    std::string image_type = "jpg";
    if (orgext == kImageTypePng) {
      image_type = "png";
    }
    MakeStream(image_type, (const unsigned char*) filestring_.c_str(),
               filestring_.size(), &outhtml);
  } else {
    b = image->Decode(w, h);

    // resize
    if (mode == kResizeTest)
      b = jpg_->Shear(dstl, dstt, dstw, dsth, sndw, sndh);
    else if (!should_resize) {
      b = image->FakeResize();
    } else {
      b = image->Resize(dstl, dstt, dstw, dsth, sndw, sndh);
    }

    // water mark
    if (mode == kResizeWaterMark) {
      b = image->Watermark(sndw, sndh, ftstrings,
                           (rtimage::aligntype) align_type, hor_distance,
                           ver_distance, logo_name);
      if (!b) {
        //LOG
        MakeStream("jpg", (const unsigned char*) filestring_.c_str(),
                   filestring_.size(), &outhtml);
        goto END;
      }
    }
    // sharpen
    if (left > 0) {
      image->Sharpen(left);
    }
    // output
    if (nowext == kImageTypeJpg) {
      b = image->EncodeJpg(quality, "", &buf, &size);
      MakeStream("jpg", buf, size, &outhtml);
    } else if (nowext == kImageTypePng) {
      b = image->EncodePng(&buf, &size);
      MakeStream("png", buf, size, &outhtml);
    } else {
      // unsupported output type
      outhtml = Make404Stream(outhtml);
      goto END;
    }
  }

  END: response->WriteRaw(outhtml);
  return HC_OK;
}

}  // namespace

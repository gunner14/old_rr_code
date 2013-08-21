#include "imagehandler.h"
#include <math.h>
#include "imagehelper.h"
#include "util.h"
#include "cwf/stream.h"
#include "base3/logging.h"
#include "base3/ptime.h"

namespace photo {

void GetAutoRotateDegreeFromExif(
    const std::map<std::string, std::string>& exifs, double* degree,
    bool* flip) {
  const std::string& orient_key("Orientation");
  std::map<std::string, std::string>::const_iterator it = exifs.find(
      orient_key);
  if (it == exifs.end() || it->second.empty() || it->second[0] < '1'
      || it->second[0] > '8') {
    if (degree)
      *degree = 0;
    if (flip)
      *flip = false;
    return;
  }
  char orient = it->second[0];
  double d, f;
  // @see http://sylvana.net/jpegcrop/exif_orientation.html
  switch (orient) {
    case '1':
      d = 0;
      f = false;
      break;
    case '2':
      d = 180;
      f = true;
      break;
    case '3':
      d = 180;
      f = false;
      break;
    case '4':
      d = 0;
      f = true;
      break;
    case '5':
      d = 270;
      f = true;
      break;
    case '6':
      d = 90;
      f = false;
      break;
    case '7':
      d = 90;
      f = true;
      break;
    case '8':
      d = 270;
      f = false;
      break;
    default:
      d = 0;
      f = false;
      break;
  }
  if (degree)
    *degree = d;
  if (flip)
    *flip = f;
  return;
}

void SetOutImgURLPrefixFromWidthHeight(const std::string commonname,
                                       const std::string magick,
                                       upload::OutImageUrl& out_image) {
  if (out_image.width_ >= 2048 && out_image.height_ > 2048) {
    out_image.url_ = "original" + commonname + magick;
    out_image.nameprefix_ = "original";
  } else if (out_image.width_ >= 1024 && out_image.height_ > 1024) {
    out_image.url_ = "xlarge" + commonname + magick;
    out_image.nameprefix_ = "xlarge";
  } else if (out_image.width_ >= 720 && out_image.height_ >= 720) {
    out_image.url_ = "large" + commonname + magick;
    out_image.nameprefix_ = "large";
  } else if (out_image.width_ >= 200 && out_image.height_ >= 600) {
    out_image.url_ = "main" + commonname + magick;
    out_image.nameprefix_ = "main";
  } else if (out_image.width_ >= 100 && out_image.height_ >= 300) {
    out_image.url_ = "head" + commonname + magick;
    out_image.nameprefix_ = "head";
  } else if (out_image.width_ >= 50 && out_image.height_ >= 50) {
    out_image.url_ = "tiny" + commonname + magick;
    out_image.nameprefix_ = "tiny";
  } else {
    out_image.url_ = "main" + commonname + magick;
    out_image.nameprefix_ = "main";
  }
}

void SetOutImgURLPrefix(const std::string commonname, const std::string magick,
                        upload::OutImageUrl& out_image) {
  static const char* nameprefix_array[] = { "main", "tiny", "xlarge", "large",
      "head", "original" };
  int len = sizeof(nameprefix_array) / sizeof(char*);
  bool bFound = false;
  for (int i = 0; i < len; i++) {
    if (std::string::npos != out_image.url_.find(nameprefix_array[i])) {
      out_image.url_ = string(nameprefix_array[i]) + commonname + magick;
      out_image.nameprefix_ = nameprefix_array[i];
      return;
    }
  }
  if (!bFound) {
    SetOutImgURLPrefixFromWidthHeight(commonname, magick, out_image);
  }
}
std::string GetCommonName() {  // 产生的是一个原始图片的所有压缩文件名的的共同部分
  std::string filepart;
  std::string uniqueStr = GetUniqueStr();
  std::string randomStr = GetRandomStr(4);
  return "_" + randomStr + "_" + uniqueStr + ".";
}

// 生成url的路径的文件名部分,如: original_FCtt_3d8b000032331264.jpg
bool GenerateDbPath(const std::string &namepre1,  // ?p
    const HandleImageConf &hic, bool resize,  // 是否压缩,决定文件名的prefix部分
    const std::string &filepart, const std::string &ext,  // 原始扩展名, 大写如PNG
    std::string* dbpath) {
  if (!dbpath)
    return false;

  if (!namepre1.empty()) {
    *dbpath += namepre1;
    *dbpath += "_";
  }
  //for (std::vector<HandleImageConf>::const_iterator it = handleimages.begin(); it != handleimages.end(); ++it) {
  /*get name prefix*/
  if (resize)
    *dbpath += hic.nameprefix_ + filepart;
  else
    *dbpath += "original" + filepart;

  if (hic.samepngtype_ && ext == "PNG")
    *dbpath += "png";
  else if (hic.samegiftype_ && ext == "GIF")
    *dbpath += "gif";
  else
    *dbpath += "jpg";
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
static bool CalcHeadImageResize(int maxwidth, int maxheight,
                                int min_width_height, int origin_width,
                                int origin_height, int *resized_width,
                                int *resized_height) {
  if (!resized_width || !resized_height || origin_width == 0
      || origin_height == 0)
    return false;

  if (maxwidth == 0)
    maxwidth = min_width_height;
  if (maxheight == 0)
    maxheight = min_width_height;

  if (origin_width >= origin_height) {
    *resized_height = maxheight;
    double widthf = (double(origin_width) * (*resized_height)) / origin_height;
    *resized_width = (int) lround(widthf);
  } else {
    *resized_width = maxwidth;
    double heightf = (double(origin_height) * (*resized_width)) / origin_width;
    *resized_height = (int) lround(heightf);
  }
  return true;
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
    //*dstheight = (maxheight * w) / maxwidth;
    *dstheight = maxheight;
  } else if (h <= maxheight) {  //crop width
    *xoff = (w - maxwidth) / 2;
    //*dstwidth = (maxwidth * h) / maxheight;
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

void debugsave(Image image, const std::string &name) {
  std::string data;
  bool b = CImageHelper::ImageData(image, 90, &data, "JPEG");
  SaveFile(std::string("/data/") + name, data);
}

bool ImageHandler::VerityImage(const std::string &data, int minwidth,
                               int minheight, upload::OutFile *outf,
                               Magick::Image *image, int *width, int *height,
                               std::string *magick, bool bDecode) {
  if (!outf || !image || !width || !height || !magick)
    return false;

  CImageHelper::GetSize(data, *width, *height, *magick);
  bool isTooLarge = ((*width) * (*height)) > 104857600;  //10240^2
  if (isTooLarge) {
    outf->code_ = 539;
    outf->msg_ = "图片宽高乘积大于10240*10240";
    LOG(ERROR) << "verityimage,too large width:" << (*width) << ",height:"
        << (*height);
    return false;
  }

  if (((*width == 0 || *height == 0) && magick->empty())) {
    outf->code_ = 534;
    outf->msg_ = "图片处理错误,确认是否是图片文件";
    LOG(ERROR) << "verityimage,no get size,data:" << data[0] << data[1]
        << data[2] << data[3];
    return false;
  }

  if (*magick != "GIF" && (*width < minwidth || *height < minheight)) {
    outf->code_ = 537;
    char buf[256];
    sprintf(buf, "图片过小,至少为%dx%dpx", minwidth, minheight);
    outf->msg_ = buf;
    LOG(ERROR) << "verityimage, image is small: " << *width << "x" << *height;
    return false;
  }
  outf->width_ = *width;
  outf->height_ = *height;

  //bool resize = CalcResize(maxwidth, maxheight, width, height, &width0, &height0);

  if (bDecode && !CImageHelper::ReadImage(data, *image, 0, 0)) {
    outf->code_ = 534;
    outf->msg_ = "图片处理错误,确认正确图片格式";
    LOG(ERROR) << "verityimage,no read image, width:" << *width << " height:"
        << *height << " magick:" << *magick << " data:" << data[0] << data[1]
        << data[2] << data[3];
    return false;
  }
  return true;
}

bool ImageHandler::Handle(const ServiceConf &sc, double deg,
                          const std::string &comment,
                          const std::string &watermark, const std::string &data,
                          const Magick::Geometry& crop_geom,
                          upload::OutFile *outf) {
  if (!outf)
    return false;

  int width0, height0;
  std::string magick;
  Image image;
  bool nooptionimage = true;
  {
    PTIME(pt1, "verityimage", true, 5);
    if (!VerityImage(data, sc.minwidth_, sc.minheight_, outf, &image, &width0,
                     &height0, &magick))
      return false;
  }
  // comment
  if (sc.checkcomment_ == true) {
    if (!comment.empty()) {
      const string& strOldComment = image.comment();
      const string& strNewComment = std::string("xiaonei") + comment;
      if ((strOldComment == "")
          || (strOldComment.find("xiaonei") == string::npos)) {
        image.comment(strNewComment);
        nooptionimage = false;
      } else if (strOldComment != strNewComment) {
        outf->code_ = 532;
        outf->msg_ = "使用了其他用户的图像";
        LOG(ERROR) << "handleerror,image theft:" << strOldComment
            << " now userid:" << strNewComment;
        return false;
      }
    }
  }

  bool flip = false;
  bool exifs_extracted = false;
  if (sc.auto_rotate_ && !(deg > 1 || deg < -1)) {
    //CImageHelper::GetExif(image, outf->exifs_);
    exifs_extracted = true;
    GetAutoRotateDegreeFromExif(outf->exifs_, &deg, &flip);
  }
  // exif
  if (sc.handleexif_) {
    if (!exifs_extracted) {
      //CImageHelper::GetExif(image, outf->exifs_);
    }
    image.profile("*", Blob());  //去掉所有的profile信息,其中exif占得最多
    if (!outf->exifs_.empty())
      nooptionimage = false;
  }

  // CMYK -> RGB
  if (image.colorSpace() == Magick::CMYKColorspace) {
    CImageHelper::TransferCMYKImage(image);
    nooptionimage = false;
  }

  if (deg > 1 || deg < -1) {
    PTIME(pt1, "rotateimage", true, 5);
    CImageHelper::RotateImage(image, deg, &width0, &height0);
    nooptionimage = false;
  }
  if (flip) {
    PTIME(pt1, "flipimage", true, 5);
    CImageHelper::FlipImage(image);
    nooptionimage = false;
  }
  //压缩
  vector<string> vecDbPath, vecFilePath;  //保存路径(db和文件)
  vector<Image> vecImg;

  //获取路径
  std::vector<string> dbpaths;
  std::string commonname = GetCommonName();

  // 缩放并放入内存
  int w, h;
  int dstwidth;
  int dstheight;
  int maxquality = 0;
  std::string maxqualityjpgdata, maxqualitypngdata, maxqualitygifdata;
  for (std::vector<HandleImageConf>::const_iterator it =
      sc.handleimages_.begin(); it != sc.handleimages_.end(); it++) {
    if (!it->crop_) {
      if (it->quality_ > maxquality)
        maxquality = it->quality_;
    }
  }
  for (std::vector<HandleImageConf>::const_iterator it =
      sc.handleimages_.begin(); it != sc.handleimages_.end(); it++) {
    outf->images_.push_back(upload::OutImageUrl());
    upload::OutImageUrl &iu = outf->images_.back();
    iu.nameprefix_ = it->nameprefix_;
    if (it->samegiftype_ && magick == "GIF") {
      bool resize = CalcResize(it->width_, it->height_, width0, height0,
                               &dstwidth, &dstheight);
      iu.width_ = dstwidth;
      iu.height_ = dstheight;
      if (!resize)
        iu.data_ = data;
      else {
        PTIME(pt1, "ResizeGifData", true, 5);
        if (!CImageHelper::ResizeGifData(data, dstwidth, dstheight,
                                         &iu.data_)) {
          outf->code_ = 534;
          outf->msg_ = "图片处理错误, GIF无法缩放";
          LOG(ERROR) << "handleerror,resize gif orgw:" << width0 << " orgh:"
              << height0 << " dstw:" << dstwidth << " dsth:" << dstheight;
          return false;
        }
      }
      GenerateDbPath(sc.savenameprefix_, *it, resize, commonname, magick,
                     &iu.url_);
    } else {
      Image image1 = image;
      if (it->crop_) {  // crop
        int xoff = 0, yoff = 0;
        if (crop_geom.isValid()) {
          // Note: the upperleft corner is offset(0, 0)
          xoff = crop_geom.xOff();
          yoff = crop_geom.yOff();
          w = crop_geom.width();
          h = crop_geom.height();
        } else {
          CalcCrop(it->width_, it->height_, width0, height0, &w, &h, &xoff,
                 &yoff);
        }
        if (xoff || yoff || w < width0 || h < height0) {
          PTIME(pt1, "CropImage", true, 5);
          CImageHelper::CropImage(image1, xoff, yoff, w, h);
        }
      } else {
        w = width0;
        h = height0;
      }

      bool resize = CalcResize(it->width_, it->height_, w, h, &dstwidth,
                               &dstheight);
      iu.width_ = dstwidth;
      iu.height_ = dstheight;

      if (resize) {
        PTIME(pt1, "ResizeImage", true, 5);
        if (!CImageHelper::ResizeImage(image1, dstwidth, dstheight)) {
          outf->code_ = 534;
          outf->msg_ = "图片处理错误, 无法缩放";
          LOG(ERROR) << "handleerror,resize magick:" << magick << " orgw:"
              << width0 << " orgh:" << height0 << " dstw:" << dstwidth
              << " dsth:" << dstheight;
          return false;
        }
      }
      bool b = true;

      // watermark
      if (sc.watermark_ == true && dstwidth > 250 && dstheight > 68
          && (it->nameprefix_ == "large" || it->nameprefix_ == "xlarge")) {
        PTIME(pt1, "watermark image", true, 5);
        Image waterMark;
        std::string watermark_file("util/watermark/renren.png");
        if (!sc.markpath_.empty()) {
          watermark_file = sc.markpath_;
        }
        WaterMarkDataMap::const_iterator it = watermark_data_.find(watermark_file);
        if (it != watermark_data_.end()) {
          CImageHelper::ReadImage(it->second, waterMark, 300, 100);
        } else {
          const std::string data = LoadFile(watermark_file, 0);
          if (!data.empty()) {
            watermark_data_[watermark_file] = data;
          }
          CImageHelper::ReadImage(data, waterMark, 300, 100);
        }
        //waterMark.opacity(MaxRGB*0.2);
        image1.composite(waterMark, 10, image1.rows() - 41, AtopCompositeOp);
        // for page upload, this implement is trivial
        bool ispage = (strcmp(watermark_file.c_str(),
                              "util/watermark/Page.jpg") == 0);
        if (ispage) {
          image1.fontPointsize(13);
          image1.font("@Arial"); // use truetype font for efficiency
          image1.fillColor(Color(MaxRGB, MaxRGB, MaxRGB, MaxRGB / 2.5));

          const std::string& strUser = comment;
          image1.annotate("page.renren.com/" + strUser, "+13+8",
                          SouthWestGravity);
          image1.fillColor(
              Color(MaxRGB / 2, MaxRGB / 2, MaxRGB / 2, MaxRGB / 2));
          image1.annotate("page.renren.com/" + strUser, "+12+8",
                          SouthWestGravity);
        }

        nooptionimage = false;
        maxqualitypngdata.clear();
        maxqualityjpgdata.clear();
      }

      // sharpen
      if (it->sharpen_ == true) {
        CImageHelper::SharpenImage(image1);
        nooptionimage = false;
        maxqualitypngdata.clear();
        maxqualityjpgdata.clear();
      }

      GenerateDbPath(sc.savenameprefix_, *it, resize || it->crop_, commonname,
                     magick, &iu.url_);
      if (!resize && !it->crop_) {  // 使用原尺寸, 优化的地方
        if (it->samepngtype_ && magick == "PNG") {
          if (maxqualitypngdata.empty()) {
            PTIME(pt1, "ImageDataPNG", true, 5);
            if (nooptionimage == true && data.size() < 200 * 1024)  // 直接原图保存
              maxqualitypngdata = data;
            else
              b = CImageHelper::ImageData(image1, it->quality_,
                                          &maxqualitypngdata, "PNG");
          }
          iu.data_ = maxqualitypngdata;
        } else {
          if (maxqualityjpgdata.empty()) {
            PTIME(pt1, "ImageDataJPEG", true, 5);
            if (nooptionimage == true && data.size() < 200 * 1024
                && magick == "JPEG")  // 直接原图保存
              maxqualityjpgdata = data;
            else
              b = CImageHelper::ImageData(image1, it->quality_,
                                          &maxqualityjpgdata, "JPEG");
          }
          iu.data_ = maxqualityjpgdata;
        }
      } else {
        if (it->samepngtype_ && magick == "PNG") {
          PTIME(pt1, "ImageDataPNG", true, 5);
          b = CImageHelper::ImageData(image1, it->quality_, &iu.data_, "PNG");
        } else {
          PTIME(pt1, "ImageDataJPEG", true, 5);
          b = CImageHelper::ImageData(image1, it->quality_, &iu.data_, "JPEG");
        }
      }
      if (!b) {
        LOG(ERROR) << "handleerror,imagedata magick:" << magick << " orgw:"
            << width0 << " orgh:" << height0 << " dstw:" << dstwidth << " dsth:"
            << dstheight;
        break;
      }
    }
  }
  return true;
}
enum HeadImageType {
  kSmallHead = 0,
  kLargeHead = 1,
  kNumHeadImageType
};
struct HeadImageConf {
  // min(width, height) used in resize
  int resize_min_wh;
  // result width and height
  int dst_wh;
};
const static HeadImageConf gHeadConf[kNumHeadImageType] = { { 58, 50 }, { 450,
    400 } };
bool ImageHandler::HandleHeadImage(const ServiceConf &sc,
                                   const std::string &comment,
                                   const std::string &data, const int crop_x_in,
                                   const int crop_y_in, upload::OutFile *outf) {
  int origin_width = 0, origin_height = 0;
  std::string magick;
  Image image;

  PTIME(pt1, "verityimage", true, 5);
  if (!VerityImage(data, sc.minwidth_, sc.minheight_, outf, &image,
                   &origin_width, &origin_height, &magick))
    return false;

  // comment
  if (sc.checkcomment_ == true) {
    if (!comment.empty()) {
      const string& strOldComment = image.comment();
      const string& strNewComment = std::string("xiaonei") + comment;
      if ((strOldComment == "")
          || (strOldComment.find("xiaonei") == string::npos)) {
        image.comment(strNewComment);
      } else if (strOldComment != strNewComment) {
        outf->code_ = 532;
        outf->msg_ = "使用了其他用户的图像";
        LOG(ERROR) << "handleerror,image theft:" << strOldComment
            << " now userid:" << strNewComment;
        return false;
      }
    }
  }

  // exif
  if (sc.handleexif_) {
    //CImageHelper::GetExif(image, outf->exifs_);
    image.profile("*", Blob());  //去掉所有的profile信息,其中exif占得最多
  }

  // CMYK -> RGB
  if (image.colorSpace() == Magick::CMYKColorspace) {
    CImageHelper::TransferCMYKImage(image);
  }
  // skip rotate image
  //压缩
  vector<string> vecDbPath, vecFilePath;  //保存路径(db和文件)
  vector<Image> vecImg;

  //获取路径
  std::vector<string> dbpaths;
  std::string commonname = GetCommonName();

  // 缩放并放入内存
  int resized_width = 0, resized_height = 0;
  for (std::vector<HandleImageConf>::const_iterator it =
      sc.handleimages_.begin(); it != sc.handleimages_.end(); it++) {
    outf->images_.push_back(upload::OutImageUrl());
    upload::OutImageUrl &iu = outf->images_.back();
    iu.nameprefix_ = it->nameprefix_;

    // crop_x and crop_y are related to min(w, h) == 58
    // so new crop_x and crop_y should figured out for min(w, h) == 450
    int crop_x = crop_x_in, crop_y = crop_y_in;
    HeadImageConf head_conf;
    if (std::min(it->width_, it->height_)
        == gHeadConf[kSmallHead].resize_min_wh) {
      head_conf = gHeadConf[kSmallHead];
    } else if (std::min(it->width_, it->height_)
        == gHeadConf[kLargeHead].resize_min_wh) {
      head_conf = gHeadConf[kLargeHead];
      double ratio =
          (double(head_conf.resize_min_wh) - head_conf.dst_wh)
              / (gHeadConf[kSmallHead].resize_min_wh
                  - gHeadConf[kSmallHead].dst_wh);
      crop_x = lround(crop_x * ratio);
      crop_y = lround(crop_y * ratio);
      if (crop_x < 0 || crop_y < 0) {
        LOG(ERROR) << "handleerror,imagedata magick:" << magick << " crop_x:"
            << crop_x_in << " crop_y:" << crop_y_in << " crop_x_450:" << crop_x
            << " crop_y_450:" << crop_y;

        return false;
      }
    } else {
      LOG(ERROR) << "handleerror, unexpected head image dst size";
      return false;
    }

    // resize
    bool resize = CalcHeadImageResize(it->width_, it->height_,
                                      head_conf.resize_min_wh, origin_width,
                                      origin_height, &resized_width,
                                      &resized_height);
    if (!resize) {
      // if resize is not required, neither does crop
      iu.width_ = resized_width;
      iu.height_ = resized_height;
      iu.data_ = data;
    } else {
      if (it->samegiftype_ && magick == "GIF") {
        PTIME(pt1, "ResizeGifData", true, 5);
        if (!CImageHelper::ResizeGifData(data, resized_width, resized_height,
                                         &iu.data_)) {
          outf->code_ = 534;
          outf->msg_ = "图片处理错误, GIF无法缩放";
          LOG(ERROR) << "handleerror,resize gif orgw:" << origin_width
              << " orgh:" << origin_height << " dstw:" << resized_width
              << " dsth:" << resized_height;
          return false;
        }
        iu.width_ = resized_width;
        iu.height_ = resized_height;
        GenerateDbPath(sc.savenameprefix_, *it, resize, commonname, magick,
                       &iu.url_);
        // TODO: gif can not crop?
      } else {
        // other image type
        Image image_tmp(image);
        if (!CImageHelper::ResizeImage(image_tmp, resized_width,
                                       resized_height)) {
          outf->code_ = 534;
          outf->msg_ = "图片处理错误, 无法缩放";
          LOG(ERROR) << "handleerror,resize magick:" << magick << " orgw:"
              << origin_width << " orgh:" << origin_height << " dstw:"
              << resized_width << " dsth:" << resized_height;
          return false;
        }
        // crop image types except gif
        PTIME(pt1, "CropImage", true, 5);

        CImageHelper::CropImage(image_tmp, crop_x, crop_y, head_conf.dst_wh,
                                head_conf.dst_wh);
        // skip water mark
        // skip sharpen
        // image to data
        bool image2data = false;
        if (it->samepngtype_ && magick == "PNG") {
          PTIME(pt1, "ImageDataPNG", true, 5);
          image2data = CImageHelper::ImageData(image_tmp, it->quality_,
                                               &iu.data_, "PNG");
        } else {
          PTIME(pt1, "ImageDataJPEG", true, 5);
          image2data = CImageHelper::ImageData(image_tmp, it->quality_,
                                               &iu.data_, "JPEG");
        }
        if (!image2data) {
          LOG(ERROR) << "handleerror,imagedata magick:" << magick << " orgw:"
              << origin_width << " orgh:" << origin_height << " dstw:"
              << resized_width << " dsth:" << resized_height;
          break;
        }
        Magick::Geometry geom = image_tmp.size();
        iu.width_ = geom.width();
        iu.height_ = geom.height();
        GenerateDbPath(sc.savenameprefix_, *it, resize, commonname, magick,
                       &iu.url_);
      }
    }
  }
  return true;
}
bool ImageHandler::VerityFile(const ServiceConf* sc, upload::OutFile *outfile) {
  if (!outfile)
    return false;
  outfile->code_ = 0;

  // 大小校验
  if (outfile->filesize_ > sc->maxfilesize_) {
    outfile->code_ = 530;
    char str[256];
    sprintf(str, "图片文件尺寸过大,最大支持%.1fM", (double) sc->maxfilesize_ / 1024 / 1024);
    outfile->msg_ = str;
    LOG(ERROR) << "verityfile,size :" << outfile->filesize_;
    return false;
  }

  // 扩展名校验
  size_t pos = outfile->filename_.find_last_of('.');
  if (pos != std::string::npos) {
    std::string lowext = ToLower(outfile->filename_.substr(pos + 1));
    if (sc->exts_.find(lowext) == sc->exts_.end()) {
      pos = std::string::npos;
    }
  }
  if (pos == std::string::npos) {
    outfile->code_ = 531;
    outfile->msg_ = "无效的文件扩展名,必须为jpg,jpeg,gif,png,bmp之一";
    LOG(ERROR) << "verityfile,file name is error:" << outfile->filename_;
    return false;
  }
  // 如果是ie的话，给的是全路径，需要截取
  // LOG(INFO) << "<>" << req.Env();
  // 处理
  //LOG(INFO) << "file name is sucess:" << outfile->filename_;
  return true;
}

bool ImageHandler::Route(const ServiceConf *sc, double deg,
                         const std::string &data, upload::OutFile *outfile) {
  if (!VerityFile(sc, outfile))
    return false;
  Magick::Geometry crop_geom;
  crop_geom.isValid(false);
  return Handle(*sc, deg, "", "", data, crop_geom, outfile);
}

int ImageHandler::ProcessImage(const ServiceConf* sc,
                               const std::string &comment,
                               cwf::Request::DispositionArrayType &files,
                               const ENUM_PROCESS_TYPE type,
                               const Magick::Geometry& crop_geom,
                               upload::OutResponse *resp) {

  // 对每张图片进行处理
  int sucess = 0;
  size_t filecount = files.size();
  resp->files_.resize(filecount);
  for (size_t i = 0; i < filecount; i++) {
    upload::OutFile& outfile = resp->files_[i];
    map<string, string> mapExif;
    CImageHelper::GetExif(files[i].data, outfile.exifs_);
    CImageHelper::RemoveAppSegment(files[i].data);
// outfile.exifs_  = mapExif;
    outfile.filesize_ = files[i].data.size();
    outfile.filename_ = files[i].filename;
    outfile.width_ = 0;
    outfile.height_ = 0;
    if (!VerityFile(sc, &outfile))
      return false;
    if (type == PROCESS_TYPE_COMMON) {
      if (Handle(*sc, 0, comment, "", files[i].data, crop_geom, &outfile))
        sucess++;
    } else if (type == PROCESS_TYPE_COPY) {
      if (HandleCopyImage(*sc, comment, "", files[i].data, &outfile))
        sucess++;
      else {
        LOG(ERROR) << "ProcessImage type error";
        assert(0);
      }
    }
  }
  return sucess;
}

int ImageHandler::ProcessImage(const ServiceConf* sc,
                               const std::string &comment,
                               const cwf::Request::DispositionArrayType &files,
                               const int crop_x, const int crop_y,
                               upload::OutResponse *resp) {
  // 对每张图片进行处理
  int sucess = 0;
  size_t filecount = files.size();
  resp->files_.resize(filecount);
  for (size_t i = 0; i < filecount; i++) {
    upload::OutFile& outfile = resp->files_[i];

    map<string, string> mapExif;
//CImageHelper::GetExif(files[i].data,  mapExif);
//CImageHelper::RemoveAppSegment(files[i].data);
    outfile.filesize_ = files[i].data.size();
    outfile.filename_ = files[i].filename;
    outfile.width_ = 0;
    outfile.height_ = 0;
    if (!VerityFile(sc, &outfile))
      return false;
    if (HandleHeadImage(*sc, comment, files[i].data, crop_x, crop_y,
                        &outfile)) {
      ++sucess;
    } else {
      LOG(ERROR) << "ProcessImage type error";
    }
  }
  return sucess;
}

bool ImageHandler::HandleCopyImage(const ServiceConf &sc,
                                   const std::string &comment,
                                   const std::string &watermark,
                                   const std::string &data,
                                   upload::OutFile *outf) {
  if (!outf)
    return false;

  int width = 0, height = 0;
  std::string magick = "jpg";
#if 1 
  Image image;
  {
    PTIME(pt1, "verityimage", true, 5);
    if (!VerityImage(data, sc.minwidth_, sc.minheight_, outf, &image, &width,
                     &height, &magick, false))
      return false;

  }
  if (magick == "JPEG")
    magick = "jpg";
#endif
  outf->images_.push_back(upload::OutImageUrl());
  upload::OutImageUrl &iu = outf->images_.back();
  iu.data_ = data;
  iu.width_ = width;
  iu.height_ = height;
  iu.url_ = outf->filename_;
  std::string commonname = GetCommonName();
  SetOutImgURLPrefix(commonname, magick, iu);
  return true;
}

} // namespace


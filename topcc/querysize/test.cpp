
#include <stdio.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "imagehelper.h"
#include "runtimeconfig.h"
#include "util.h"
#include "base3/logging.h"
#include "base3/ptime.h"

struct FileResponse {
  int nCode;
  std::string strMsg;
  std::map<std::string, std::string> mapExif;
};

/*
struct FormDisposition {
  std::string name;
  std::string filename;
  std::string content_type;
  std::string data;
};
typedef FormDisposition TFormFile;
*/

bool GetFileBuffer(const char *file, char **buf, int* len) {
  bool b = true;
  int f = open(file, O_RDONLY);
  if (f < 0)
    return false;
  const int sz = 1024*1024*10;
  if (buf) {
    if (!len) {
      b = false;
    } else {
      *buf = new char [sz];
      ssize_t res = read(f, *buf, sz);
      if (res >= 0) {
        *len = res;
      } else {
        b = false;
      }
    }
  }
  close(f);
  return b;
}

void ChangeFileToGif(std::string *filename, std::string *dbname) {
  size_t index = filename->find(".jpg");
  if (index == filename->size() - 4) // 没必要判断std::string::npos
    filename->replace(index, 4, ".gif");
  index = dbname->find(".jpg");
  if(index == dbname->size() - 4)
    dbname->replace(index, 4, ".gif");
}
bool  GenerateSavePath(const TUploadConf *pUploadConf,const string&strSubfix,vector<string>& vecDbPath,vector<string>& vecFilePath)
{
  /*get save group conf*/
  const vector<TSaveGroupConf> & vecSaveGroupConf =  pUploadConf->vecSaveGroupConf;
  if(vecSaveGroupConf.empty())
    return false;
  int size = vecSaveGroupConf.size();
  int index = GetRandomNumber(0, size);
  const TSaveGroupConf& saveGroupConf = vecSaveGroupConf.at(index);

  /*get save image confs*/
  const vector<TSaveImageConf> & vecSaveImageConf = pUploadConf->vecSaveImageConf;
  if(vecSaveImageConf.empty())
    return false;

  /*get time path*/
  string timeStr = GetTimeStr();
  string uniqueStr = GetUniqueStr();
  string namePrefix1= pUploadConf->strSaveNamePrefix;  
  namePrefix1=(namePrefix1!="")?(namePrefix1+"_"):"";
  string strSubDir = pUploadConf->strSaveSubDir;
  strSubDir = (strSubDir!="")?(strSubDir+"/"):"";
  string saveDir = saveGroupConf.strSaveNodePath + "/" + strSubDir + timeStr;
  //Log(saveDir);
  Mkdir(saveDir);

  for(unsigned int i=0; i<vecSaveImageConf.size(); ++i)
  {
    /*get name prefix*/
    const TSaveImageConf& saveImageConf = vecSaveImageConf.at(i);
    string namePrefix2 = saveImageConf.strNamePrefix;
    namePrefix2 = (namePrefix2!="")?(namePrefix2+"_"):"";

    string randomStr = GetRandomStr(4);
    string fullPath = strSubDir + timeStr + "/" + namePrefix1 + namePrefix2 + randomStr + "_" + uniqueStr + "." + strSubfix;
    string savePath = saveGroupConf.strSaveNodePath + "/" + fullPath;
    string dbPath = saveGroupConf.strGroupName + "/" + fullPath;
    vecDbPath.push_back(dbPath);
    // TODO: 为了小站特殊处理
    if (i == vecSaveImageConf.size() -1)
      vecFilePath.push_back(savePath);
    else
      vecFilePath.push_back("");
  }
  return true;
}
bool Upload(const TUploadConf *pUploadConf,const TFormFile &formfile, FileResponse& fileResponse, std::vector<std::string> &vecDbPath) {

  const vector<TSaveImageConf>& vecSaveImageConf = pUploadConf->vecSaveImageConf;
  assert(!vecSaveImageConf.empty());
  const TSaveImageConf& saveImageConf = vecSaveImageConf.at(0);


  int nMaxSaveWidth = saveImageConf.nWidth;
  int nMaxSaveHeight = saveImageConf.nHeight;
  int nMinWidthLimit = pUploadConf->pairMinSize.first;
  int nMinHeightLimit = pUploadConf->pairMinSize.second;
  float fMinSizeRateLimit =pUploadConf->pairSizeRate.first;
  float fMaxSizeRateLimit =pUploadConf->pairSizeRate.second;

  int errorCode=0;
        
  string strHostId;
  std::vector<std::string> post_buf;
  int det_img_size = 0;
  

    //const TFormFile& formfile = files[i];
    Image image;
    try {
      int nWidth = 0;
      int nHeight = 0;
      string magick;
      //图片宽高验证
      {
         // drop for test PTIME(pt1, "get_image_size", true, 5);
         CImageHelper::GetSize(formfile.data , nWidth , nHeight, magick);
      }

      if(nWidth == 0 || nHeight == 0){
        fileResponse.nCode=4;
        errorCode=505;
        fileResponse.strMsg = "此文件不是图片";
        return false;
      }
      
      //暂时关闭图片验证问题,等错误提示完全后再增加
      string strSizeLog=ToString<int>(nWidth)+"x"+ToString<int>(nHeight);      
      if(nWidth<nMinWidthLimit || nHeight<nMinHeightLimit){
        fileResponse.nCode=5;
        errorCode=505;
        fileResponse.strMsg = "图片尺寸不能小于" + ToString<int>(nMinWidthLimit)+"x"+ToString<int>(nMinHeightLimit)+",当前为"+strSizeLog;
        return false;
      }

      //图片宽高比验证
      float fRate=nWidth*1.0/nHeight;
      if(fMinSizeRateLimit>0 && fRate<fMinSizeRateLimit){
        fileResponse.nCode=6;
        errorCode=505;
        fileResponse.strMsg = "图片宽高比不能小于" + ToString<float>(fMinSizeRateLimit) + ",当前为" + ToString<float>(fRate)+","+strSizeLog;
        return false;
      }
      if(fMaxSizeRateLimit>0 && fRate>fMaxSizeRateLimit){
        fileResponse.nCode=6;
        errorCode=505;
        fileResponse.strMsg = "图片宽高比不能大于" + ToString<float>(fMaxSizeRateLimit) + ",当前为" + ToString<float>(fRate)+","+strSizeLog; 
        return false;
      }

      //读图
      {
        // drop for test PTIME(pt1, "read_image", true, 1);
        if(!CImageHelper::ReadImage(formfile.data, image, nMaxSaveWidth,nMaxSaveHeight)){
          fileResponse.nCode=7;
          errorCode=505;
          fileResponse.strMsg="服务器读图出错";
          return false;
        }
      }

      //处理comment
      if(pUploadConf->strChannel=="head"){
        if(strHostId!=""){
          const string& strOldComment=image.comment();
          const string& strNewComment="xiaonei"+strHostId; 
          if((strOldComment=="") || (strOldComment.find("xiaonei")==string::npos)){
            image.comment(strNewComment);
          }
          else if(strOldComment!=strNewComment){
            Log("commets is %s, %s",strOldComment.c_str(), strNewComment.c_str());
            fileResponse.nCode=10;
            fileResponse.strMsg="盗用了其他用户的头像";
            return false;
          }
        }
      }

      //处理exif
      CImageHelper::GetExif(image, fileResponse.mapExif);

      image.profile("*",Blob());  //去掉所有的profile信息,其中exif占得最多

      //转换CMYK图片为RGB
      if(image.colorSpace() == Magick::CMYKColorspace) {
        // drop for test PTIME(pt1, "transferCMYKImage", true, 5);
        CImageHelper::TransferCMYKImage(image);
      }

      vector<string>vecFilePath;//保存路径(db和文件)
      if(magick == "GIF" &&((pUploadConf->strChannel == "xiaozhan_photo") 
                          ||(pUploadConf->strChannel == "xiaozhanFlashPhoto"))) {//小站传的gif文件,对生成的大图需要特殊处理
        GenerateSavePath(pUploadConf,"jpg",vecDbPath,vecFilePath);
        ChangeFileToGif(&vecDbPath[0], &vecFilePath[0]);

        bool bResize = true;//是否需要压缩
        if(nMaxSaveWidth == 0 && nMaxSaveHeight == 0)
          bResize = false;
        else if(nMaxSaveWidth == 0 && nHeight <= nMaxSaveHeight)
          bResize = false;
        else if(nMaxSaveHeight == 0 && nWidth <= nMaxSaveWidth)
          bResize = false;
        else if((nWidth <= nMaxSaveWidth) && (nHeight <= nMaxSaveHeight))
          bResize = false;


        //存有动画效果的大图
        {
        // drop for test PTIME(pt1, "save_gif", true, 5);
        if(!CImageHelper::SaveGIFImage(formfile.data, saveImageConf, vecFilePath[0], bResize)){
          fileResponse.nCode=7;
          errorCode=505;
          fileResponse.strMsg="保存gif大图出错";
          return false;
        }
        }

        vector<Image> vecGif;//除了第一张图的其他图不需要动画效果
        vector<TSaveImageConf> vecSaveGifConf(vecSaveImageConf.begin()+1, vecSaveImageConf.end());
        vector<string> vecGifFilePath(vecFilePath.begin()+1, vecFilePath.end());

        //压缩小图
        if(!CImageHelper::ResizeImage(image,vecSaveGifConf, vecGif)){
          fileResponse.nCode=7;
          errorCode=505;
          fileResponse.strMsg="缩gif小图出错";
          return false;
        }

        //保存小图
        if(!CImageHelper::SaveJPEGImage(vecGif,vecGifFilePath,vecSaveGifConf, NULL)){
          fileResponse.nCode=7;
          fileResponse.strMsg="保存gif小图出错";
          return false;
        }
      } else {
        //压缩
        vector<Image> vecImg;
        {
          // drop for test PTIME(pt, "resize_image", true, 1);
          if(!CImageHelper::ResizeImage(image,vecSaveImageConf, vecImg)){
            fileResponse.nCode=7;
            errorCode=505;
            fileResponse.strMsg="缩图出错";
            return false;
          }
        }


        bool warktable =  false;
        Image& largeImage = vecImg[0];
        bool imageSizeRight = largeImage.rows() >= 220 && largeImage.columns() >= 220;
        

        if (warktable && imageSizeRight && vecImg.size() > 3){
          Image waterMark;
          string waterFile = LoadFile("util/watermark/Page.jpg", 0); 
          CImageHelper::ReadImage(waterFile, waterMark, 300, 100);
             largeImage.opacity(30);
             largeImage.composite(waterMark, 10, largeImage.rows() - 41, AtopCompositeOp);
          largeImage.fontPointsize(13);
                                        largeImage.font("@Arial");
          largeImage.fillColor(Color(MaxRGB,MaxRGB,MaxRGB,MaxRGB/2.5));
      
          string strUser="watermark test";
          largeImage.annotate("page.renren.com/" + strUser,  "+13+8", SouthWestGravity);

          largeImage.fillColor(Color(MaxRGB/2,MaxRGB/2,MaxRGB/2,MaxRGB/2));
          largeImage.annotate("page.renren.com/" + strUser,  "+12+8", SouthWestGravity);
        }

        //获取路径
        if(pUploadConf->strChannel=="page_skin"){//单独提出来写是因为如果保存成jpeg格式的话，png等图会有丢失，这个在皮肤上是不能接受的
          const string& strFileName = "testpng.png";
          size_t index = strFileName.find_last_of(".");
          string strExt;
          if(index != string::npos)
            strExt = strFileName.substr(index+1);
          GenerateSavePath(pUploadConf,strExt,vecDbPath,vecFilePath);

          //保存
          if(!CImageHelper::SaveImage(vecImg,vecFilePath,vecSaveImageConf)){
            fileResponse.nCode=7;
            fileResponse.strMsg="保存图片出错";
            return false;
          }

        }else{
          GenerateSavePath(pUploadConf,"jpg",vecDbPath,vecFilePath);

          // TODO : log
          //std::string log;
          //for(std::vector<std::string>::iterator it = vecDbPath.begin(); it != vecDbPath.end(); ++it) {
          //  log += *it;
          //  log += " ";
          //}
          //LOG(INFO) << "generate jpg path: " << log;

          {
            // drop for test PTIME(pt, "save_image", true, 10);
            if(!CImageHelper::SaveJPEGImage(vecImg, vecFilePath, vecSaveImageConf, &post_buf)) {
              fileResponse.nCode=7;
              fileResponse.strMsg="保存图片出错";
              LOG(WARNING) << "<> save Image file error:" << vecFilePath[0];
              return false;
            }
          }
        }
      }

      //计算md5
      string picMd5 = StrMd5(formfile.data);

      Log("UploadMd5 %s %s", strHostId.c_str(), picMd5.c_str());
    }
    catch(Exception & e){
      errorCode=505;
      string strError=e.what();
       /*为了保存错误文件临时增加的代码
       time_t now;
       time(&now);
       struct tm *p = localtime(&now);
       char buf[100];
       snprintf(buf, sizeof(buf), "/data/img/%04d%02d%02d-%02d%02d%02d-%d.jpg", (p->tm_year + 1900), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, getpid());
       SaveFile(buf, formfile.strFileData); */
    }
  
  return true;
}

int main(int argc, char * argv[]) {
  
  char ch;
  g_runTimeConfig.Init();
  const TUploadConf *pUploadConf= g_runTimeConfig.GetUploadConf(argv[2]);
  char *buf;
  int len;
  if (argc < 3) {
    printf("plz input photo_file_path and channel_name\n");
    return 0;
  }
  bool b = GetFileBuffer(argv[1], &buf, &len);
  FileResponse response;
  TFormFile tf;
  tf.data = std::string(buf, len);
  tf.filename = "demo.jpg";
  tf.name = "photo1";
  vector<TFormFile> files;
  std::vector<std::string> vecDbPath;
  b = Upload(pUploadConf, tf, response, vecDbPath);
  if (b)
    printf("%s\n", vecDbPath.back().c_str());
  else
    printf("error:%s\n", response.strMsg.c_str());
  /*while ((ch = getopt(argc, argv, "i:o:a12345h")) != EOF) {
    switch (ch) {
      case 'C':
        cachesize = (u_int32_t)atoi(optarg);
      break;
  }*/
  return 0;
}

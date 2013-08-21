#include <curl/curl.h>
#include <assert.h>

#include "imageprocessor.h"
#include "memcache.h"
#include "imagehelper.h"
#include "downloader.h"
#include "runtimeconfig.h"
#include "util.h"
#include "base3/logging.h"
#include "base3/ptime.h"
#include "dbutil.h"

CImageProcessor::CImageProcessor()
{
	std::vector<net::Address> v;
	//v.push_back(std::make_pair("10.22.206.180", 80));
	// arch::LogToDir("/data/log", arch::INFO, 20*3600);
	v.push_back(std::make_pair("10.7.18.125", 80));
#ifndef kHeadVersion
	webpool_.Start(v);
#endif
}

bool CImageProcessor::Validate(const TRequest& request, TResponse& response)
{
	assert(request.pUploadConf);
	bool bIsAlbum = g_runTimeConfig.IsAncestorChannal(
			request.pUploadConf->strChannel, "album");
	if (bIsAlbum)
	{
		string strOp = request.Query("op");
		if ((strOp == "" || strOp == "upload") && request.files().empty())
		{
			response.nCode = 504;//为了兼容控件上传，错误代码必须是特定的值才行
			/*
			 * { 501, _T("用户未登录") },
			 * { 502, _T("用户未激活") },
			 * { 503, _T("相册参数不对") },
			 * { 504, _T("没有上传照片") }, // 没有上传照片
			 * { 505, _T("无效的照片格式") },
			 * { 506, _T("相册不存在") },
			 * { 507, _T("没有权限") },
			 * { 508, _T("达到用户空间上限，上传失败") },
			 * { 509, _T("不能盗用别人照片") }, // 盗用照片
			 * { 510, _T("照片无法处理") },
			 * { 511, _T("达到相册照片数上限") },
			 * 504, 505, 509可以继续，其他的话上传终止
			 * */
			response.strMsg = "没有上传文件";
			return false;
		}

		//相册批量上传必须需要albumid参数且大于0
		if (strOp == "upload" || strOp == "finish" || strOp == "clean")
		{
			string strAlbumId = request.Query("albumid");
			int nAlbumId = atoi(strAlbumId.c_str());
			if (nAlbumId <= 0)
			{
				response.nCode = 503;
				response.strMsg = "缺少相册id参数";
				return false;
			}
		}
	}
	else // 难道其他类型可以为空图片?  if(request.pUploadConf->strChannel == "head" || request.pUploadConf->strChannel == "page_skin"){//头像上传，只检查是不是没有上传文件
	if (request.files().empty())
	{
		response.nCode = 11;
		response.strMsg = "没有上传文件";
		return false;
	}
	return CBaseProcessor::Validate(request, response);
}

void ChangeFileToGif(std::string *filename, std::string *dbname)
{
	size_t index = filename->find(".jpg");
	if (index == filename->size() - 4) // 没必要判断std::string::npos
		filename->replace(index, 4, ".gif");
	index = dbname->find(".jpg");
	if (index == dbname->size() - 4)
		dbname->replace(index, 4, ".gif");
}

void GetFileFromDb(std::vector<TFileResponse> &vecFileResponse,
		std::string &strHostId, std::string &strAlbumId)
{
	std::vector<Photo> photos = DbUtil::getInstance().getPhotoListByAlbum(
			std::atoi(strHostId.c_str()), std::atol(strAlbumId.c_str()));
	std::string strJson = DbUtil::getInstance().Photos2Json(photos);
	TResponse r2;
	if (r2.FromJson(strJson))
	{
		vecFileResponse.insert(vecFileResponse.end(),
				r2.vecFileResponse.begin(), r2.vecFileResponse.end());
	}
	DbUtil::getInstance().deletePhotoInfo(std::atoi(strHostId.c_str()),
			std::atol(strAlbumId.c_str()));
}

bool CImageProcessor::Upload(TRequest& request, TResponse& response)
{
	//文件数为空不处理,相册
	if (request.files().empty())
		return true;

	if (request.Query("pagetype") == "addPhotoConfigured")
	{//如果是客户端配置缩略图宽度和高度的话
		LOG(INFO)<< "addPhotoConfigured";
		return UploadConfigured(request, response);
	}
	const TUploadConf *pUploadConf=request.pUploadConf;
	assert(pUploadConf);

	const vector<TSaveImageConf>& vecSaveImageConf = pUploadConf->vecSaveImageConf;
  assert(!vecSaveImageConf.empty());
  const TSaveImageConf& saveImageConf = vecSaveImageConf.at(0);
//remove const for funcition RemoveAppSegment
  vector<TFormFile>& files =request.files();
  vector<TFileResponse>& vecFileResponse = response.vecFileResponse;
  vecFileResponse.resize(files.size());

  int nMaxSaveWidth = saveImageConf.nWidth;
  int nMaxSaveHeight = saveImageConf.nHeight;
  int nMinWidthLimit = pUploadConf->pairMinSize.first;
  int nMinHeightLimit = pUploadConf->pairMinSize.second;
  float fMinSizeRateLimit =pUploadConf->pairSizeRate.first;
  float fMaxSizeRateLimit =pUploadConf->pairSizeRate.second;

  int nOkCount=0;
  int errorCode=0;

  string strHostId;
#ifndef kHeadVersion
  net::HtmlData hd;
#endif
  std::vector<std::string> post_buf;
  int det_img_size = 0;
#ifndef kHeadVersion
  hd.AppendHead("Cookie: "+request.Env("COOKIE"));
#endif
  /*if (request.Cookie("t").empty()) {
    std::string t = request.Query("t");
          if (!t.empty()) {
            size_t pos = t.find('-');
      if (pos != std::string::npos)
        t = t.substr(pos+1);
    }
  }*/

  for(size_t i=0; i<files.size(); i++)
  {

#ifndef kHeadVersion
    hd.ClearBody();
#endif
    post_buf.clear();

    TFileResponse& fileResponse=vecFileResponse[i];
    if(fileResponse.nCode !=1) {
      LOG(WARNING) << "No." << i << " image is error: " << fileResponse.nCode;
      continue;
    }

      //处理exif
   CImageHelper::GetExif(files[i].data, fileResponse.mapExif);
   CImageHelper::RemoveAppSegment(files[i].data);



    const TFormFile& formfile = files[i];
    Image image;
    try {
      int nWidth = 0;
      int nHeight = 0;
      string magick;
      //图片宽高验证
      {
         PTIME(pt1, "get_image_size", true, 5);
         CImageHelper::GetSize(formfile.data , nWidth , nHeight, magick);
      }
      LOG(INFO) << "No." << i << " [" << formfile.filename << "] length: " << formfile.data.size() << " width: " << nWidth << " height: " << nHeight;

      if(nWidth == 0 || nHeight == 0){
        fileResponse.nCode=4;
        errorCode=505;
        fileResponse.strMsg = "此文件不是图片";
        // TODO:test
        std::string path = "/data/" + request.Query("hostid") + "_" + request.Query("albumid") + "_" + formfile.filename;
        int f = open(path.c_str(), O_RDWR | O_CREAT | O_LARGEFILE, S_IRUSR|S_IWUSR|S_IRGRP);
        if (f != -1) {
          pwrite(f, formfile.data.c_str(), formfile.data.size(), 0);
          close(f);
        }
        continue;
      }

      //暂时关闭图片验证问题,等错误提示完全后再增加
      string strSizeLog=ToString<int>(nWidth)+"x"+ToString<int>(nHeight);
      if(nWidth<nMinWidthLimit || nHeight<nMinHeightLimit){
        fileResponse.nCode=5;
        errorCode=505;
        fileResponse.strMsg = "图片尺寸不能小于" + ToString<int>(nMinWidthLimit)+"x"+ToString<int>(nMinHeightLimit)+",当前为"+strSizeLog;
        continue;
      }

      //图片宽高比验证
      float fRate=nWidth*1.0/nHeight;
      if(fMinSizeRateLimit>0 && fRate<fMinSizeRateLimit){
        fileResponse.nCode=6;
        errorCode=505;
        fileResponse.strMsg = "图片宽高比不能小于" + ToString<float>(fMinSizeRateLimit) + ",当前为" + ToString<float>(fRate)+","+strSizeLog;
        continue;
      }
      if(fMaxSizeRateLimit>0 && fRate>fMaxSizeRateLimit){
        fileResponse.nCode=6;
        errorCode=505;
        fileResponse.strMsg = "图片宽高比不能大于" + ToString<float>(fMaxSizeRateLimit) + ",当前为" + ToString<float>(fRate)+","+strSizeLog;
        continue;
      }
      //读图
      {
        PTIME(pt1, "read_image", true, 1);
        if(!CImageHelper::ReadImage(formfile.data, image, nMaxSaveWidth,nMaxSaveHeight)){
          fileResponse.nCode=7;
          errorCode=505;
          fileResponse.strMsg="服务器读图出错";
          continue;
        }
      }

      image.profile("*",Blob());  //去掉所有的profile信息,其中exif占得最多
    
      strHostId = request.Query("hostid")!=""?request.Query("hostid"):request.Cookie("id");
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
            continue;
          }
        }
      }

    
      //转换CMYK图片为RGB
      if(image.colorSpace() == Magick::CMYKColorspace) {
        PTIME(pt1, "transferCMYKImage", true, 5);
        CImageHelper::TransferCMYKImage(image);
      }

      vector<string>vecDbPath,vecFilePath;//保存路径(db和文件)
      if(magick == "GIF" &&((pUploadConf->strChannel == "xiaozhan_photo")
                          ||(pUploadConf->strChannel == "xiaozhanFlashPhoto")
                          ||(pUploadConf->strChannel == "addPhotoAPI")
                          ||(pUploadConf->strChannel == "addOriginal"))) {//小站传的gif文件,对生成的大图需要特殊处理
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

        // TODO : log
        std::string log;
        for(std::vector<std::string>::iterator it = vecDbPath.begin(); it != vecDbPath.end(); ++it) {
          log += *it;
          log += " ";
        }
        LOG(INFO) << "generate gif path: " << log;

        //存有动画效果的大图
        {
        PTIME(pt1, "save_gif", true, 5);
        int width0 = nWidth;
        int height0 = nHeight;
        if(!CImageHelper::SaveGIFImage(formfile.data, saveImageConf, vecFilePath[0], bResize, &width0, &height0)){
          fileResponse.nCode=7;
          errorCode=505;
          fileResponse.strMsg="保存gif大图出错";
          continue;
        }
        fileResponse.mapOtherMsg["width0"] = ToString<int>(width0);
        fileResponse.mapOtherMsg["height0"] = ToString<int>(height0);
        }

        vector<Image> vecGif;//除了第一张图的其他图不需要动画效果
        vector<TSaveImageConf> vecSaveGifConf(vecSaveImageConf.begin()+1, vecSaveImageConf.end());
        vector<string> vecGifFilePath(vecFilePath.begin()+1, vecFilePath.end());

        //压缩小图
        if(!CImageHelper::ResizeImage(image,vecSaveGifConf, vecGif)){
          fileResponse.nCode=7;
          errorCode=505;
          fileResponse.strMsg="缩gif小图出错";
          continue;
        }
        for(size_t k=0; k<vecGif.size(); k++){
          std::string key="width"+ToString<int>(k+1);
          fileResponse.mapOtherMsg[key]=ToString<int>(vecGif[k].columns());
          key="height"+ToString<int>(k+1);
          fileResponse.mapOtherMsg[key]=ToString<int>(vecGif[k].rows());
        }

        //保存小图
        if(!CImageHelper::SaveJPEGImage(vecGif,vecGifFilePath,vecSaveGifConf, NULL)){
          fileResponse.nCode=7;
          fileResponse.strMsg="保存gif小图出错";
          continue;
        }
      } else {
        //压缩
        vector<Image> vecImg;
        {
          PTIME(pt, "resize_image", true, 1);
          if (pUploadConf->strChannel == "addOriginal") { // 此类型的不处理,类似附件
            vecImg.push_back(image);
          } else {
            if(!CImageHelper::ResizeImage(image,vecSaveImageConf, vecImg)){
              fileResponse.nCode=7;
              errorCode=505;
              fileResponse.strMsg="缩图出错";
              continue;
            }
          }
        }

       //if(pUploadConf->strChannel !="page_skin") { // 锐化
          //PTIME(pt, "charpen_image", true, 1);
          // TODO: 暂时去掉 CImageHelper::SharpenImage(vecSaveImageConf, vecImg, nWidth);
       //}

        bool warktable =  request.Query("watermark") == "1";
        Image& largeImage = vecImg[0];
        bool imageSizeRight = largeImage.rows() >= 220 && largeImage.columns() >= 220;

#ifndef kHeadVersion
        char tmp[128];
        sprintf(tmp, "width=%d", largeImage.columns());
        hd.AppendBody(tmp);
        sprintf(tmp, "height=%d", largeImage.rows());
        hd.AppendBody(tmp);
#endif


        if (warktable && imageSizeRight && vecImg.size() > 3){
          Image waterMark;
          static string waterFile = LoadFile("util/watermark/Page.jpg", 0);
          CImageHelper::ReadImage(waterFile, waterMark, 300, 100);
             largeImage.opacity(30);
             largeImage.composite(waterMark, 10, largeImage.rows() - 41, AtopCompositeOp);
          largeImage.fontPointsize(13);
                                        largeImage.font("@Arial");
          largeImage.fillColor(Color(MaxRGB,MaxRGB,MaxRGB,MaxRGB/2.5));

          string strUser=request.Query("alias")!=""?request.Query("alias"):request.Query("pid");
          largeImage.annotate("page.renren.com/" + strUser,  "+13+8", SouthWestGravity);

          largeImage.fillColor(Color(MaxRGB/2,MaxRGB/2,MaxRGB/2,MaxRGB/2));
          largeImage.annotate("page.renren.com/" + strUser,  "+12+8", SouthWestGravity);
        }

        //获取路径
        if(pUploadConf->strChannel=="page_skin"){//单独提出来写是因为如果保存成jpeg格式的话，png等图会有丢失，这个在皮肤上是不能接受的
          const string& strFileName = fileResponse.strFileName;
          size_t index = strFileName.find_last_of(".");
          string strExt;
          if(index != string::npos)
            strExt = strFileName.substr(index+1);
          GenerateSavePath(pUploadConf,strExt,vecDbPath,vecFilePath);

          //保存
          if(!CImageHelper::SaveImage(vecImg,vecFilePath,vecSaveImageConf)){
            fileResponse.nCode=7;
            fileResponse.strMsg="保存图片出错";
            continue;
          }

        }else{
          GenerateSavePath(pUploadConf,"jpg",vecDbPath,vecFilePath);
#ifndef kHeadVersion
          hd.AppendBody(std::string("largeUrl=")+vecDbPath[0]);
#endif

          // TODO : log
          std::string log;
          for(std::vector<std::string>::iterator it = vecDbPath.begin(); it != vecDbPath.end(); ++it) {
            log += *it;
            log += " ";
          }
          LOG(INFO) << "generate jpg path: " << log;

          {
            PTIME(pt, "save_image", true, 10);
            if(!CImageHelper::SaveJPEGImage(vecImg, vecFilePath, vecSaveImageConf, &post_buf)) {
              fileResponse.nCode=7;
              fileResponse.strMsg="保存图片出错";
              LOG(WARNING) << "<> save Image file error:" << vecFilePath[0];
              continue;
            }
          }
          {
            PTIME(pt, "save_cache", true, 1);
            std::vector<std::string>::iterator dbit = vecDbPath.begin();
            //static int _timeout[5] = { 3600, 7200, 86400*2, 86400, 0}; // TODO: 以后放到配置中
#ifdef kHeadVersion
            static int _timeout[5] = { 3600, 3600, 4800, 4800, 0}; // TODO: 以后放到配置中
            int i = 0;
            for (std::vector<std::string>::iterator it = post_buf.begin(); it != post_buf.end() && dbit != vecDbPath.end(); ++it, ++dbit) {
              g_nginxcache.Set(std::string("/") + *dbit, *it, _timeout[i++]);
            };
#else
#endif

          }
          //long uid = atol(strHostId.c_str());
          //if (uid%10>0 && uid%10<6) {  //
#ifndef kHeadVersion
          if (!post_buf.empty()) {
            hd.AssignData(post_buf.front().c_str(), post_buf.front().size());
            hd.AppendBody(std::string("hostId=")+strHostId);
            hd.AppendBody(std::string("albumId=")+request.Query("albumid"));
            webpool_.PostFile(hd);
          }
#endif
          //}
          for(size_t k=0; k<vecDbPath.size(); k++){
            std::string key="width"+ToString<int>(k);
            fileResponse.mapOtherMsg[key]=ToString<int>(vecImg[k].columns());
            key="height"+ToString<int>(k);
            fileResponse.mapOtherMsg[key]=ToString<int>(vecImg[k].rows());
          }
        }
      }
      //保存状态
      fileResponse.nCode=1;
      fileResponse.mapOtherMsg["width"]=ToString<int>(nWidth);
      fileResponse.mapOtherMsg["height"]=ToString<int>(nHeight);
      for(size_t k=0; k<vecDbPath.size(); k++){
        string key="url"+ToString<int>(k);
        fileResponse.mapOtherMsg[key]=vecDbPath[k];
      }
      nOkCount++;

      //计算md5
      string picMd5 = StrMd5(formfile.data);

      Log("UploadMd5 %s %s", strHostId.c_str(), picMd5.c_str());
      if(pUploadConf->strChannel == "gossip_pic"){
        fileResponse.mapOtherMsg["md5"] = picMd5;
      }
    }
    catch(Exception & e){
      fileResponse.nCode=4;
      errorCode=505;
      fileResponse.strMsg = "图片处理出错";
      string strError=e.what();
       /*为了保存错误文件临时增加的代码
                        time_t now;
                        time(&now);
                        struct tm *p = localtime(&now);
                        char buf[100];
                        snprintf(buf, sizeof(buf), "/data/img/%04d%02d%02d-%02d%02d%02d-%d.jpg", (p->tm_year + 1900), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, getpid());
                        SaveFile(buf, formfile.strFileData); */
      Log("HandleImageFail %s %s", fileResponse.strFileName.c_str(), strError.c_str());
    }

  }

  response.nErrorCount = response.nTotalCount - nOkCount;
  ///没有一个成功则说明所有的都失败
  if(nOkCount==0){
    response.nCode=errorCode;
    response.strMsg="所有图片处理都失败";
    LOG(WARNING) << "<> all " << response.nTotalCount << " image handle error";
    return false;
  } else {
    if (nOkCount != response.nTotalCount) {
      LOG(WARNING) << "<> have " << nOkCount << "/" << response.nTotalCount << " image handle ok";
     } else {
      LOG(INFO) << "<> all " << nOkCount << " image handle ok";
     }
  }
  return true;
}

bool CImageProcessor::AfterUpload(const TRequest& request, TResponse& response)
{
	bool bOk = true;

	const TUploadConf *pUploadConf = request.pUploadConf;
	assert(pUploadConf);
	string strHostId = request.Query("hostid") != "" ? request.Query("hostid")
			: request.Cookie("id");

	//相册特殊处理
	int nCacheSec = 12 * 3600; //cache过期时间为12小时
	bool bIsAlbum = g_runTimeConfig.IsAncestorChannal(pUploadConf->strChannel,
			"album");

	if ("event_album" == pUploadConf->strChannel)
	{
		LOG(INFO)<< "event_album";
		string strAlbumId=request.Query("albumid");

		const string& strOp=request.Query("op");
		string strPhotoCountKey="upload2photoinfos_"+strAlbumId+"_"+strHostId+"_count";
    string strPhotoKeyPrefix="upload2photoinfos_"+strAlbumId+"_"+strHostId+"_";
    string pagetype = request.Query("pagetype");
    if(strOp=="upload"){

      PTIME(pt1, "save_memcache", true, 5);
      string strCountValue=g_memcache.Get(strPhotoCountKey);
      int nCount=atoi(strCountValue.c_str());
      if(!g_memcache.Set(strPhotoCountKey, ToString<int>(nCount+1) , nCacheSec) )
        Log("AfterUploadUpload.MemcacheSetFail[%d] %s %s %s", g_memcache.Error(),strPhotoCountKey.c_str(), strHostId.c_str(), pagetype.c_str());

      string strPhotoKey = strPhotoKeyPrefix + ToString<int>(nCount);
      string strJson=response.ToJson();
      if(!g_memcache.Set(strPhotoKey, strJson, nCacheSec))
        Log("AfterUploadUpload.MemcacheSetFail[%d] %s %s %s", g_memcache.Error(),strPhotoKey.c_str(), strHostId.c_str(), pagetype.c_str());

      return true;
    }
    else if(strOp=="finish"){
      vector<TFileResponse>& vecFileResponse = response.vecFileResponse;
      vecFileResponse.clear();

      string strCountValue=g_memcache.Get(strPhotoCountKey);
      if(strCountValue == "")
        Log("AfterUploadFinish.MemcacheGetFail[%d] %s %s %s",g_memcache.Error(),strPhotoCountKey.c_str(), strHostId.c_str(), pagetype.c_str());
      size_t nCount=atoi(strCountValue.c_str());
      if(nCount>0){
        if(!g_memcache.Delete(strPhotoCountKey))  //清除countkey
          Log("AfterUploadFinish.MemcacheDeleteFail[%d] %s %s %s",g_memcache.Error(),strPhotoCountKey.c_str(), strHostId.c_str(), pagetype.c_str());

        for(size_t i=0; i<nCount;i++){
          string strPhotoKey = strPhotoKeyPrefix + ToString<int>(i);
          string strJson=g_memcache.Get(strPhotoKey);
          if(strJson == ""){
            Log("AfterUploadFinish.MemcacheGetFail[%d] %s %s %s",g_memcache.Error(), strPhotoKey.c_str(), strHostId.c_str(), pagetype.c_str());
            continue;
          }
          TResponse r2;
          if(r2.FromJson(strJson)){
            vecFileResponse.insert(vecFileResponse.end(),r2.vecFileResponse.begin(),r2.vecFileResponse.end());
          }

          if(!g_memcache.Delete(strPhotoKey))  //清除photo信息的缓存
            Log("AfterUploadFinish.MemcacheDeleteFail[%d] %s %s %s",g_memcache.Error(),strPhotoKey.c_str(), strHostId.c_str(), pagetype.c_str());
        }
        response.nTotalCount = vecFileResponse.size();
        for(size_t i=0; i<vecFileResponse.size(); i++){
          const TFileResponse& fileResponse=vecFileResponse[i];
                      if(fileResponse.nCode != 1)
                              response.nErrorCount++;
              }
      }

    }
    else if(strOp=="clean"){
      string strCountValue=g_memcache.Get(strPhotoCountKey);
      size_t nCount=atoi(strCountValue.c_str());
      if(nCount>0){
        if(!g_memcache.Delete(strPhotoCountKey))  //清除countkey
          Log("AfterUploadClean.MemcacheDeleteFail[%d] %s %s %s",g_memcache.Error(), strPhotoCountKey.c_str(), strHostId.c_str(), pagetype.c_str());
        for(size_t i=0; i<nCount; i++){
          string strPhotoKey = strPhotoKeyPrefix + ToString<int>(i);
          if(!g_memcache.Delete(strPhotoKey))  //清除photo信息的缓存
            Log("AfterUploadClean.MemcacheDeleteFail[%d] %s %s %s", g_memcache.Error(), strPhotoKey.c_str(), strHostId.c_str(), pagetype.c_str());
        }
      }
      return true;
    }
  }
		else if(bIsAlbum)
		{
			string strAlbumId=request.Query("albumid");

			const string& strOp=request.Query("op");
			string strPhotoCountKey="upload2photoinfos_"+strAlbumId+"_count";
			string strPhotoKeyPrefix="upload2photoinfos_"+strAlbumId+"_";
			string pagetype = request.Query("pagetype");
			if(strOp=="upload")
			{
				PTIME(pt1, "upload_memcache", true, 5);
				string strCountValue=g_memcache.Get(strPhotoCountKey);
				int nCount=atoi(strCountValue.c_str());
				if(!g_memcache.Set(strPhotoCountKey, ToString<int>(nCount+1) , nCacheSec) )
				Log("AfterUploadUpload.MemcacheSetFail[%d] %s %s %s", g_memcache.Error(),strPhotoCountKey.c_str(), strHostId.c_str(), pagetype.c_str());

				string strPhotoKey = strPhotoKeyPrefix + ToString<int>(nCount);
				string strJson=response.ToJson();
				if(!g_memcache.Set(strPhotoKey, strJson, nCacheSec))
				Log("AfterUploadUpload.MemcacheSetFail[%d] %s %s %s", g_memcache.Error(),strPhotoKey.c_str(), strHostId.c_str(), pagetype.c_str());

				std::vector<Photo> photos = DbUtil::getInstance().Json2Photos(strJson,std::atoi(strHostId.c_str()),std::atol(strAlbumId.c_str()));
				DbUtil::getInstance().savePhotoInfo(photos);

				return true;
			}
			else if(strOp=="finish")
			{
				PTIME(pt1, "finish_memcache", true, 5);
				vector<TFileResponse>& vecFileResponse = response.vecFileResponse;
				vecFileResponse.clear();

				string strCountValue=g_memcache.Get(strPhotoCountKey);
				if(strCountValue == "")
				{
					Log("AfterUploadFinish.MemcacheGetFail[%d] %s %s %s",g_memcache.Error(),strPhotoCountKey.c_str(), strHostId.c_str(), pagetype.c_str());
				}
				size_t nCount=atoi(strCountValue.c_str());
				if(nCount<=0)
				{
					GetFileFromDb(vecFileResponse,strHostId,strAlbumId);
				}
				else
				{
					if(!g_memcache.Delete(strPhotoCountKey)) //清除countkey
					Log("AfterUploadFinish.MemcacheDeleteFail[%d] %s %s %s",g_memcache.Error(),strPhotoCountKey.c_str(), strHostId.c_str(), pagetype.c_str());

					for(size_t i=0; i<nCount;i++)
					{
						string strPhotoKey = strPhotoKeyPrefix + ToString<int>(i);
						string strJson=g_memcache.Get(strPhotoKey);
						if(strJson == "")
						{
							Log("AfterUploadFinish.MemcacheGetFail[%d] %s %s %s",g_memcache.Error(), strPhotoKey.c_str(), strHostId.c_str(), pagetype.c_str());
							GetFileFromDb(vecFileResponse,strHostId,strAlbumId);
							break;
						}
						TResponse r2;
						if(r2.FromJson(strJson))
						{
							vecFileResponse.insert(vecFileResponse.end(),r2.vecFileResponse.begin(),r2.vecFileResponse.end());
						}

						if(!g_memcache.Delete(strPhotoKey))
						{ //清除photo信息的缓存
							Log("AfterUploadFinish.MemcacheDeleteFail[%d] %s %s %s",g_memcache.Error(),strPhotoKey.c_str(), strHostId.c_str(), pagetype.c_str());
						}
					}
					DbUtil::getInstance().deletePhotoInfo(std::atoi(strHostId.c_str()), std::atol(strAlbumId.c_str()));
				}
				response.nTotalCount = vecFileResponse.size();
				for(size_t i=0; i<vecFileResponse.size(); i++)
				{
					const TFileResponse& fileResponse=vecFileResponse[i];
					if(fileResponse.nCode != 1)
					response.nErrorCount++;
				}
				LOG(INFO) << "AfterFinish: " << nCount << " Org: " << response.nTotalCount;
			}
			else if(strOp=="clean")
			{
				string strCountValue=g_memcache.Get(strPhotoCountKey);
				size_t nCount=atoi(strCountValue.c_str());
				if(nCount>0)
				{
					if(!g_memcache.Delete(strPhotoCountKey)) //清除countkey
					Log("AfterUploadClean.MemcacheDeleteFail[%d] %s %s %s",g_memcache.Error(), strPhotoCountKey.c_str(), strHostId.c_str(), pagetype.c_str());
					for(size_t i=0; i<nCount; i++)
					{
						string strPhotoKey = strPhotoKeyPrefix + ToString<int>(i);
						if(!g_memcache.Delete(strPhotoKey)) //清除photo信息的缓存
						Log("AfterUploadClean.MemcacheDeleteFail[%d] %s %s %s", g_memcache.Error(), strPhotoKey.c_str(), strHostId.c_str(), pagetype.c_str());
					}
				}
				return true;
			}
		}

		return CBaseProcessor::AfterUpload(request,response);
	}

bool CImageProcessor::GenerateSavePath(const TUploadConf *pUploadConf,
		const string&strSubfix, vector<string>& vecDbPath,
		vector<string>& vecFilePath)
{
	/*get save group conf*/
	const vector<TSaveGroupConf> & vecSaveGroupConf =
			pUploadConf->vecSaveGroupConf;
	if (vecSaveGroupConf.empty())
		return false;
	int size = vecSaveGroupConf.size();
	int index = GetRandomNumber(0, size);
	const TSaveGroupConf& saveGroupConf = vecSaveGroupConf.at(index);

	/*get save image confs*/
	const vector<TSaveImageConf> & vecSaveImageConf =
			pUploadConf->vecSaveImageConf;
	if (vecSaveImageConf.empty())
		return false;

	/*get time path*/
	string timeStr = GetTimeStr();
	string uniqueStr = GetUniqueStr();
	string namePrefix1 = pUploadConf->strSaveNamePrefix;
	namePrefix1 = (namePrefix1 != "") ? (namePrefix1 + "_") : "";
	string strSubDir = pUploadConf->strSaveSubDir;
	strSubDir = (strSubDir != "") ? (strSubDir + "/") : "";
	string saveDir = saveGroupConf.strSaveNodePath + "/" + strSubDir + timeStr;
	//Log(saveDir);
	Mkdir(saveDir);

	for (unsigned int i = 0; i < vecSaveImageConf.size(); ++i)
	{
		/*get name prefix*/
		const TSaveImageConf& saveImageConf = vecSaveImageConf.at(i);
		string namePrefix2 = saveImageConf.strNamePrefix;
		namePrefix2 = (namePrefix2 != "") ? (namePrefix2 + "_") : "";

		string randomStr = GetRandomStr(4);
		string fullPath = strSubDir + timeStr + "/" + namePrefix1 + namePrefix2
				+ randomStr + "_" + uniqueStr + "." + strSubfix;
		string savePath = saveGroupConf.strSaveNodePath + "/" + fullPath;
		string dbPath = saveGroupConf.strGroupName + "/" + fullPath;
		vecDbPath.push_back(dbPath);
		vecFilePath.push_back(savePath);
	}
	return true;
}

bool CImageProcessor::UploadConfigured(const TRequest& request,
		TResponse& response)
{
	const TUploadConf *pUploadConf = request.pUploadConf;
	assert(pUploadConf);
	int requestWidth = atoi(request.Query("w").c_str());
	int requestHeight = atoi(request.Query("h").c_str());
	if ((0 == requestWidth) || (0 == requestHeight))
	{
		response.nCode = 0;
		response.strMsg = "错误的参数";
		return false;

	}

	//不使用pUploadConf中全局的vecSaveImageConf,而是根据用户输入生成一个
	TSaveImageConf saveImageConf;
	saveImageConf.nWidth = requestWidth;
	saveImageConf.nHeight = requestHeight;
	saveImageConf.bCrop = false;
	saveImageConf.quality = 100;
	saveImageConf.strNamePrefix = "";

	vector<TSaveImageConf> vecSaveImageConf;
	vecSaveImageConf.push_back(saveImageConf);

	const vector<TFormFile>& files = request.files();
	vector<TFileResponse>& vecFileResponse = response.vecFileResponse;
	vecFileResponse.resize(files.size());

	int nMaxSaveWidth = saveImageConf.nWidth;
	int nMaxSaveHeight = saveImageConf.nHeight;
	int nMinWidthLimit = pUploadConf->pairMinSize.first;
	int nMinHeightLimit = pUploadConf->pairMinSize.second;
	float fMinSizeRateLimit = pUploadConf->pairSizeRate.first;
	float fMaxSizeRateLimit = pUploadConf->pairSizeRate.second;

	int nOkCount = 0;
	int errorCode = 0;
	for (size_t i = 0; i < files.size(); i++)
	{
		const TFormFile& formfile = files[i];
		TFileResponse& fileResponse = vecFileResponse[i];
		if (fileResponse.nCode != 1)
			continue;
		Image image;
		try
		{
			int nWidth = 0;
			int nHeight = 0;
			string magick;
			//图片宽高验证
			CImageHelper::GetSize(formfile.data, nWidth, nHeight, magick);
			if (nWidth == 0 || nHeight == 0)
			{
				fileResponse.nCode = 4;
				errorCode = 505;
				fileResponse.strMsg = "此文件不是图片";
				continue;
			}
			//暂时关闭图片验证问题,等错误提示完全后再增加
			string strSizeLog = ToString<int> (nWidth) + "x" + ToString<int> (
					nHeight);
			if (nWidth < nMinWidthLimit || nHeight < nMinHeightLimit)
			{
				fileResponse.nCode = 5;
				errorCode = 505;
				fileResponse.strMsg = "图片尺寸不能小于" + ToString<int> (
						nMinWidthLimit) + "x" + ToString<int> (nMinHeightLimit)
						+ ",当前为" + strSizeLog;
				continue;
			}

			//图片宽高比验证
			float fRate = nWidth * 1.0 / nHeight;
			if (fMinSizeRateLimit > 0 && fRate < fMinSizeRateLimit)
			{
				fileResponse.nCode = 6;
				errorCode = 505;
				fileResponse.strMsg = "图片宽高比不能小于" + ToString<float> (
						fMinSizeRateLimit) + ",当前为" + ToString<float> (fRate)
						+ "," + strSizeLog;
				continue;
			}
			if (fMaxSizeRateLimit > 0 && fRate > fMaxSizeRateLimit)
			{
				fileResponse.nCode = 6;
				errorCode = 505;
				fileResponse.strMsg = "图片宽高比不能大于" + ToString<float> (
						fMaxSizeRateLimit) + ",当前为" + ToString<float> (fRate)
						+ "," + strSizeLog;
				continue;
			}

			//读图
			if (!CImageHelper::ReadImage(formfile.data, image, nMaxSaveWidth,
					nMaxSaveHeight))
			{
				fileResponse.nCode = 7;
				errorCode = 505;
				fileResponse.strMsg = "服务器读图出错";
				continue;
			}

			image.profile("*", Blob()); //去掉所有的profile信息,其中exif占得最多

			//转换CMYK图片为RGB
			if (image.colorSpace() == Magick::CMYKColorspace)
				CImageHelper::TransferCMYKImage(image);

			vector<string> vecDbPath, vecFilePath;//保存路径(db和文件)
			//压缩
			vector<Image> vecImg;
			if (!CImageHelper::ResizeImage(image, vecSaveImageConf, vecImg))
			{
				fileResponse.nCode = 7;
				errorCode = 505;
				fileResponse.strMsg = "缩图出错";
				continue;
			}

			//获取路径
			GenerateSavePath(pUploadConf, "jpg", vecDbPath, vecFilePath);
			//保存
			if (!CImageHelper::SaveJPEGImage(vecImg, vecFilePath,
					vecSaveImageConf, NULL))
			{
				fileResponse.nCode = 7;
				fileResponse.strMsg = "保存图片出错";
				continue;
			}
			//保存状态
			fileResponse.nCode = 1;
			fileResponse.mapOtherMsg["width"] = ToString<int> (nWidth);
			fileResponse.mapOtherMsg["height"] = ToString<int> (nHeight);
			for (size_t k = 0; k < vecDbPath.size(); k++)
			{
				string key = "url" + ToString<int> (k);
				fileResponse.mapOtherMsg[key] = vecDbPath[k];
			}
			nOkCount++;

		} catch (Exception & e)
		{
			fileResponse.nCode = 4;
			errorCode = 505;
			fileResponse.strMsg = "图片处理出错";
			string strError = e.what();
			Log("UploadFail %s %s", fileResponse.strFileName.c_str(),
					strError.c_str());
		}

	}

	response.nErrorCount = response.nTotalCount - nOkCount;
	///没有一个成功则说明所有的都失败
	if (nOkCount == 0)
	{
		response.nCode = errorCode;
		response.strMsg = "所有图片处理都失败";
		return false;
	}
	return true;
}

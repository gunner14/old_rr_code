#include "baseprocessor.h"
#include "memcache.h"
#include "runtimeconfig.h"

#include "downloader.h"
#include "util.h"

#include <assert.h>
#include "base3/logging.h"
#include "base3/ptime.h"

bool CBaseProcessor::Process(TRequest& request,TResponse& response)
{
  bool bOk=false;
  
  //验证请求是否有效
  bOk=Validate(request,response);
  
  //核心上传处理
  if(bOk)
    bOk=Upload(request,response);

  //上传后处理:保存db,失败了也要afterupload,以获取适当的forward页面
  bool bAfterbOk=AfterUpload(request,response);
  
  //response.nCode=(bOk && bAfterbOk)?1:0;
  return response.nCode==1;
}

bool CBaseProcessor::Validate(const TRequest& request,TResponse& response)
{
  const vector<TFormFile>& vecFormFile=request.files();

  if(!IsLogin(request,response)){
    response.nCode=501;
    response.strMsg="用户未登录";
    LOG(ERROR) << response.strMsg;
    return false;
  }

  string strChannel=request.Query("channel");
  const TUploadConf *pUploadConf=g_runTimeConfig.GetUploadConf(strChannel);
  if(!pUploadConf){
    response.nCode=0;
    response.strMsg="频道参数为空或错误:"+strChannel;
    LOG(ERROR) << response.strMsg;
    return false;
  }
  
  if(!vecFormFile.empty()){
    bool bOneOk=false;
    int errorCode = 0;
    response.vecFileResponse.resize(vecFormFile.size());
    for(size_t i=0; i<vecFormFile.size(); i++){
      const TFormFile& file=vecFormFile[i];
      TFileResponse& fileResponse=response.vecFileResponse[i];
      //设置desc
      //string strDesc = request.Query("desc");
      //if(strDesc != "")
        //fileResponse.mapOtherMsg["desc"]=strDesc;

      size_t nPos=file.filename.rfind(".");
      string strExt;
      if(nPos!=string::npos)
        strExt=ToLower(file.filename.substr(nPos+1));
      if(!pUploadConf->setExt.empty() && pUploadConf->setExt.find(strExt)==pUploadConf->setExt.end() && request.Query("pagetype")!="addPhotoActivex"){
        fileResponse.nCode=2;
        errorCode = 505;
        fileResponse.strMsg="文件后缀名错误,必须为"+pUploadConf->strExts+",当前为"+(strExt.empty()?"空":strExt);
        LOG(WARNING) << fileResponse.strMsg;
        continue;
      }
      
      float nCurSize = (float)file.data.size()/(1024*1024);
      int nLimitSize = pUploadConf->nFileSizeLimit/(1024*1024);
      if(nCurSize > nLimitSize){
        fileResponse.nCode=3;
        errorCode = 505;
        fileResponse.strMsg="文件大小不能超过"+ToString<int>(nLimitSize)+"M,当前为"+ToString<float>(nCurSize)+"M";
        LOG(WARNING) << fileResponse.strMsg;
        continue;
      }
      bOneOk=true;
    }
    if(!bOneOk){
      ///没有一个成功则说明所有的都失败
      if(vecFormFile.size() == 1)//
        response.nCode=errorCode;
      else
        response.nCode = 0;
      response.strMsg="所有文件验证失败,请确定文件大小及文件后缀名";
      LOG(ERROR) << response.strMsg;
      return false;
    }
  }
  return true;
}

bool CheckJiaren(const std::string &str, const std::string &hostid) {
   //static std::string KEY = "Xysh38@34#";
   std::string msg = "pagetype=jiaren&hostid=";
   msg += hostid;
   msg += "&key=Xysh38@34#";
   std::string keymd5 = StrMd5(msg);
   return str == keymd5;
}

bool CBaseProcessor::IsLogin(const TRequest& request,TResponse& response)
{
  const TUploadConf *pUploadConf=request.pUploadConf;
  assert(pUploadConf);

  string strSocietyGuester=request.Query("societyguester")!=""?request.Query("societyguester"):request.Cookie("societyguester");
  string strt=request.Query("t")!=""?request.Query("t"):request.Cookie("t");
  string strUserId=request.Query("hostid")!=""?request.Query("hostid"):request.Cookie("id");
  if(strSocietyGuester=="" && strUserId=="" && strt==""){
    Log("IsLoginFail societyguester, t和hostid都为空 %s",request.Query().c_str());
    return false;
  }
  else{
    if(pUploadConf->strChannel == "head"){//头像上传，只上传一张，先不做验证
      return true;
    }
    if("EYZAQpJr2DFjfxXH" == request.Query("encryption")){//彩信上传，用key验证
      return true;
    }
    if(request.Query("pagetype") == "add3GMap" && "Etjj_QpTDS23jX_" == request.Query("encry")){//3G地图，用key验证
      return true;
    }
    if(request.Query("pagetype") == "addShop" && "EYZ@QpJr23)(jfxX_" == request.Query("encry")){//商城，用key验证
      return true;
    }
    if(request.Query("pagetype") == "addHeadPageApi" && "EYZQpJr23)ll(jfxXj" == request.Query("encry")){//开放平台传page主页头像
      return true;
    }
    if (request.Query("pagetype") == "jiaren")
      return CheckJiaren(request.Query("sign"), strUserId);

    //if (request.Query("pagetype") == "add3GOriginal" || request.Query("pagetype") == "add3GAPP") // 3g app 暂时不验票
    //  return true;
    //读memcache判断是否登录
    if(strUserId!="" && g_memcache.Get("IsLogin_"+strUserId)=="1")
      return true;
    if(strt!="" && g_memcache.Get("IsLogin_"+strt)=="1")
      return true;
    
    //判断用户是否登录
    string loginUrl = pUploadConf->strIsLoginUrl;
    string strUrl=loginUrl + "?" + request.Query();
    LOG(INFO) << "IsLogin: " << strUrl;

    THttpArg httpArg;
    httpArg.vecReqHeader.push_back("Cookie: "+request.Env("COOKIE"));
    httpArg.vecReqHeader.push_back("Referer: "+request.Env("REFERER"));
    httpArg.vecReqHeader.push_back("User-Agent: "+request.Env("USER_AGENT"));
    string ip = request.Env("X_Forwarded_For");
    if(ip == "")
      ip = request.Env("REMOTE_ADDR");
    httpArg.vecReqHeader.push_back("X-Forwarded-For: "+ip);

    int nHttpCode;
    {
       PTIME(pt1, "valid_ticket", true, 10);
       nHttpCode=g_downloader.Download(strUrl,httpArg);
    }
    string strHostId=httpArg.strResBody;
    long nHostId = atol(strHostId.c_str());
    strHostId=ToString<long>(nHostId);
    if(nHttpCode==200 && nHostId>0l ){
      //半小时之内不用重新判断登录
      if(strHostId!="" && !g_memcache.Set("IsLogin_"+strHostId,"1",1800))
        Log("IsLogin.MemcacheSetFail IsLogin_%s %s", strHostId.c_str(), strHostId.c_str());
      if(strt!="" && !g_memcache.Set("IsLogin_"+strt,"1",1800))
        Log("IsLogin.MemcacheSetFail IsLogin_%s %s", strt.c_str(), strHostId.c_str());
      return true;
    }
    Log("IsLoginFail 服务器验证出错 societyguester=%s t=%s hostid=%s query=%s cookie=%s nHttpCode=%d header=%s body=%s",
        strSocietyGuester.c_str(), strt.c_str(), 
        strUserId.c_str(), request.Query().c_str(), 
        request.Cookie().c_str(), nHttpCode, 
        httpArg.strResHeader.c_str(), httpArg.strResBody.c_str());
    return false;
  }
}

bool CBaseProcessor::AfterUpload(const TRequest& request, TResponse& response) {
  const TUploadConf *pUploadConf=request.pUploadConf;
  assert(pUploadConf);
  
  string strAfterUrl=pUploadConf->strAfterUrl;

  if(!strAfterUrl.empty()){
    strAfterUrl += "?" + request.Query();
    ///1个文件且错误,则把此文件错误信息赋值给strMsg
    //if(response.vecFileResponse.size()==1 && response.vecFileResponse[0].nCode!=1)
    //  response.strMsg = response.vecFileResponse[0].strMsg;
    string strJson=response.ToJson();
    THttpArg httpArg;
    httpArg.vecReqHeader.push_back("Cookie: "+request.Env("COOKIE"));
    httpArg.vecReqHeader.push_back("Referer: "+request.Env("REFERER"));
    httpArg.vecReqHeader.push_back("User-Agent: "+request.Env("USER_AGENT"));
    string ip = request.Env("X_Forwarded_For");
    if(ip == "")
      ip = request.Env("REMOTE_ADDR");
    httpArg.vecReqHeader.push_back("X-Forwarded-For: "+ip);
    httpArg.strReqBody=strJson;

    // TODO: debug
    char filebuf[100];
    if (request.Query("pagetype") == "addEventPhotoFlash") {
      time_t now;
      time(&now);
      struct tm *p = localtime(&now);
      snprintf(filebuf, sizeof(filebuf), "/data/log/%04d%02d%02d-%02d%02d%02d-%d.log", (p->tm_year + 1900), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, getpid());
      SaveFile(filebuf, strJson);
    }
    int nHttpCode=0;
    { 
      PTIME(pt, "photo_java", true, 1);
      nHttpCode = g_downloader.Download(strAfterUrl,httpArg);  
    }
    // TODO: debug
    if (request.Query("pagetype") == "addEventPhotoFlash") {
      sprintf(filebuf, "%s.%d", filebuf, nHttpCode);
      SaveFile(filebuf, httpArg.strResBody);
    }

    bool bOk = (nHttpCode==200 || nHttpCode==301 || nHttpCode==302);
    if(!bOk && httpArg.strResHeader.empty()){
    //if(!bOk){
      response.nCode = 0;
      response.strMsg = "请求数据库服务器失败";
    }
    response.nAfterHttpCode = nHttpCode;
    response.strAfterResHeader = httpArg.strResHeader;
    response.strAfterResBody= httpArg.strResBody;
    response.bHaveSaveDb = true;
    DLOG(INFO) << "<> photo_url: " << strAfterUrl << " response code: " << nHttpCode << " body: " << httpArg.strResBody;
    return bOk;
  }
  return true;
}



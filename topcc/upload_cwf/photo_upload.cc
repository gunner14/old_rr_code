#include "photo_upload.h"
#include "base3/logging.h"

#include "runtimeconfig.h"
#include "processdispatcher.h"
#include "util.h"
#include "datatype.h"
#include "json/json.h"

namespace photo {

using namespace cwf;
bool PhotoAction::Match(const std::string& url) const {
  return true;// 目前就一个页面,直接返回了 std::string::npos != url.find("eat");
}

extern CProcessDispatcher * g_dispatcher;

void HandleActiveX(Request* request) {
  std::map<string,string>& mapQuery = request->query_;
  std::string strPageType= request->get("pagetype");
  // 据说为了兼容ActiveX控件
  ///activex控件的op参数(由flag转换而来)的特殊处理
  if(strPageType=="addPhotoActivex" || strPageType=="addPhotoX"){
    string& strOp = mapQuery["op"];
    if(strOp == "0" )
      strOp = "upload";
    else if(strOp == "1")
      strOp = "finish";
    else if(strOp =="-1")
      strOp ="clean";
  }
  
  //避免414 url too large
  if(strPageType == "addPhotoActivex" || strPageType == "addPhotoFlash" || strPageType == "addPhotoX")  
    mapQuery["ids"] ="";  
}

bool FillRequest(Request* request) {

  // 据说为了兼容
  std::map<string,string>& mapQuery = request->query_;
  std::string strPageType= request->get("pagetype");
  std::map<std::string,std::map<std::string,std::pair<std::string,std::string> > >::const_iterator itMap =g_runTimeConfig.m_mapPageType2QueryNormalize.find(strPageType);
  if(itMap != g_runTimeConfig.m_mapPageType2QueryNormalize.end()){
    for(map<string,pair<string,string> >::const_iterator itMap2 = itMap->second.begin(); itMap2!=itMap->second.end(); itMap2++){
      const std::string& strDstParam = itMap2->first;
      const std::string& strSrcParam = itMap2->second.first;
      const std::string& strDefaultValue= itMap2->second.second;
      //printf("<> %s,%s,%s\n", strDstParam.c_str(), strSrcParam.c_str(), strDefaultValue.c_str());

      if(!request->get(strDstParam.c_str()).empty()) {
       // printf("  exist dst: %s,value:%s, continue\n", strDstParam.c_str(), request->get(strDstParam.c_str()).c_str());
        continue;
      }
      std::string req_src = request->get(strSrcParam.c_str());
      if(!req_src.empty()) {
        mapQuery[strDstParam] = req_src;
        //printf("  exist src: %s, value:%s\n", strSrcParam.c_str(), req_src.c_str());
      } else {
        //printf("  no exist src: %s, default:%s\n", strSrcParam.c_str(), strDefaultValue.c_str());
        mapQuery[strDstParam] = strDefaultValue;
      }
    }
    //printf("<> channel:%s\n", request->get("channel").c_str());
    return true;
  }else{
    return false;
  }
}

HttpStatusCode PhotoAction::Process(Request* request, Response* response) {
  TRequest req;
  TResponse resp;
  resp.Init();
  std::string channel; 

  bool b = FillRequest(request);
  HandleActiveX(request);

  channel = request->get("channel");
  if (!b && channel == "") {
    LOG(WARNING) << "<> no exist this pagetype and channel:" << request->get("pagetype");
    resp.nCode = 0;
    resp.strMsg = "错误的频道类型参数";
    return HC_OK;
  }


  const TUploadConf* pUploadConf = g_runTimeConfig.GetUploadConf(channel);
  if (pUploadConf == NULL) {
    resp.nCode = 0;
    resp.strMsg = "错误的频道参数";
    LOG(WARNING) << "new upload, but channel error:" << request->get("channel");
  } else {
    req.Init(request, pUploadConf);

    // 初始化resp
    resp.nTotalCount = request->file_array_.size();
    resp.vecFileResponse.resize(resp.nTotalCount);
    LOG(INFO) << "new upload, "
        << " pagetype: " << request->get("pagetype") 
        << " blumid: " << request->get("albumid")
        << " form count: " << request->forms().size()
        << " channel: " << request->get("channel")
        << " CONTENT_LENGTH: " << req.Env("CONTENT_LENGTH")
        << " file count: " << resp.nTotalCount
        << " Cookie: " << req.Env("COOKIE")
        << " Addr: " <<  req.Env("X_Forwarded_For");

    for (size_t i=0; i<resp.nTotalCount; i++) {
      TFileResponse& fileResponse = resp.vecFileResponse[i];
      fileResponse.nCode = 1;
      fileResponse.nFileSize = request->file_array_[i].data.size();

      // 如果是ie的话，给的是全路径，需要截取
      //LOG(INFO) << "<>" << req.Env();

      if(req.Env("USER_AGENT").find("MSIE") != string::npos) {
        std::string strName = GetFileName(request->file_array_[i].filename);
        if(request->get("pagetype") == "addPhotoActivex")//老版控件上传的文件名时GBK编码的
          strName = GBKToUTF8(strName);
        fileResponse.strFileName = strName;
      } else {
        fileResponse.strFileName = DecodeUrl(request->file_array_[i].filename);
      }
    }

    // 处理
    bool bProcessOk=g_dispatcher->Process(req, resp);
    if (!bProcessOk) {
      LOG(ERROR) << "Process Image Handle";
    }

    // 输出
    std::string strOutput;
    resp.Output(req,strOutput);
    response->WriteRaw(strOutput+'\n');

  }
  return HC_OK;
}




} // namespace 

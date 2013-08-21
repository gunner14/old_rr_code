#include "base3/stringencode.h"
#include "base3/logging.h"
#include "datatype.h"
#include "util.h"

/******************TRequest*****************/
void TRequest::Init(cwf::Request *req, const TUploadConf* conf) {
  req_ = req;
  pUploadConf= conf;//g_runTimeConfig.GetUploadConf(request.get("channel"));
}

string TRequest::Env(const string& key) const {
  //char* env = getenv(key.c_str());
  return  req_->header(key.c_str());
}

string TRequest::Env() const
{
  string strVar;
  size_t nPos=string::npos;
  /*char** var=environ;
  if (var != NULL) {
    strVar = *var;
    ++var;
  }
  for(; *var!=NULL; ++var){
    strVar += '&';
    strVar +=*var;
  }*/
  cwf::StringMap::const_iterator it = req_->header().begin();
  if (it != req_->header().end()) {
    strVar = it->first;
    strVar += '=';
    strVar += it->second;
    ++it;
  }
  for (;it != req_->header().end(); ++it) {
    strVar += '&';
    strVar += it->first;
    strVar += '=';
    strVar += it->second;
  }
  return strVar;
}
const string& TRequest::Query(const string& key)const
{
  return req_->get(key.c_str());
}

#define VALUE_ENCODE(str, value)    \
{                                   \
  char cc[1024*30 + 1];             \
  if (value.size()*3 > sizeof(cc))  \
    str += value;                   \
  else {                            \
    size_t end_pos = base::url_encode(cc, sizeof(cc), value.c_str(), value.size());\
    str.append(cc, end_pos);        \
  }                                 \
}                                   \


string TRequest::Query()const {

  std::string str;
  const cwf::StringMap &sm = req_->querys();
  cwf::StringMap::const_iterator it = sm.begin();
  if (it != sm.end()) {
    str += it->first;
    str += '=';
    VALUE_ENCODE(str, it->second);
    it++;
  }
  for (; it != sm.end(); ++it) {
    str += '&';
    str += it->first;
    str += '=';
    VALUE_ENCODE(str, it->second);
  }
  
  const cwf::StringMap &sm1 = req_->forms();
  cwf::StringMap::const_iterator it1 = sm1.begin();
  if (it1 != sm1.end()) {
    if (sm.empty() && !it1->first.empty()) {
      str += it1->first;
      str += '=';
      VALUE_ENCODE(str, it1->second);
      it1++;
    } else {
    }
  }
  for (; it1 != sm1.end(); ++it1) {
    if (it1->first.empty()) // form 有可能有空value
      continue;
    str += '&';
    str += it1->first;
    str += '=';
    VALUE_ENCODE(str, it1->second);
  }
  return str;
}

string TRequest::Cookie(const string& key)const {
  const cwf::Cookie &c = req_->cookie();
  return c.Get(key.c_str());
}

string TRequest::Cookie()const {
  const cwf::Cookie &c = req_->cookie();
  return c.text();
}

std::vector<TFormFile>& TRequest::files()const {
  return req_->files();
}

/******************TFileResponse*****************/
void TFileResponse::ToJson(Json::Value & val, bool bForSaveDb)const
{
  val.clear();
  val["code"]=nCode;
  val["msg"]=strMsg;
  val["fileSize"]=nFileSize;
  val["fileName"]=strFileName;
  if(bForSaveDb){
    for(map<string,string>::const_iterator itMap=mapOtherMsg.begin(); itMap!=mapOtherMsg.end(); itMap++)
      val[itMap->first]=itMap->second;
    for(map<string,string>::const_iterator itMap=mapExif.begin(); itMap!=mapExif.end(); itMap++)
      val["exif"][itMap->first]=itMap->second;
  }

}
string TFileResponse::ToJson(bool bForSaveDb)const
{
  Json::Value val;
  ToJson(val,bForSaveDb);

  Json::FastWriter writer;
  string strJson=writer.write(val);
  return strJson;
}

bool  TFileResponse::FromJson(const Json::Value& val)
{
  for(Json::Value::const_iterator it=val.begin(); it!=val.end(); it++){
    const Json::Value& key=it.key();
    const Json::Value& value=*it;
    
    if(!key.isString())
      continue;
    string strKey=key.asString();
    
    if(strKey=="code" && value.isInt())
      nCode=value.asInt();
    else if(strKey=="msg" && value.isString())
      strMsg=value.asString();
    else if(strKey=="fileSize" && value.isInt())
      nFileSize=value.asInt();
    else if(strKey=="fileName" && value.isString())
      strFileName=value.asString();
    else if(strKey=="exif"){
      for(Json::Value::const_iterator it2=value.begin(); it2!=value.end(); it2++){
        const Json::Value& key2 = it2.key();
        const Json::Value& value2 = *it2;
        if(key2.isString() && value2.isString())
          mapExif[key2.asString()]=value2.asString();
      }
    }
    else if(strKey!="" && value.isString())
      mapOtherMsg[strKey]=value.asString();
  }
  return true;
}

bool  TFileResponse::FromJson(const string& strJson)
{
  Json::Value val;
  Json::Reader reader;
  if(!reader.parse(strJson,val))
    return false;
  return FromJson(val);  
}

/************TResponse******************/
void TResponse::Init()
{
  nCode=1;      ///<1 表示成功,其他的未定义,目前均表示失败,默认成功
  strMsg.clear();
  nTotalCount=0;
  nErrorCount=0;
  vecFileResponse.clear();

  //request.pUploadConf = g_runTimeConfig.GetUploadConf(request.Query("channel"));
  bHaveSaveDb=false;
  nAfterHttpCode=500;
  strAfterResHeader.clear();
  strAfterResBody.clear();
}

void TResponse::ToJson(Json::Value& val, bool bForSaveDb) const
{
  if(nTotalCount == 0)//没有上传文件
  {
    val["code"]=0;
    val["msg"]="没有上传文件";
  }else if(nTotalCount == nErrorCount){
    val["code"]=0;
    val["msg"]="所有文件都处理失败";
  }else{
    val["code"]=nCode;
    val["msg"]=strMsg;
  }
  val["totalCount"] = nTotalCount;
  val["errorCount"] = nErrorCount;
  for(size_t i=0; i<vecFileResponse.size(); i++){
    const TFileResponse& fileResponse=vecFileResponse[i];
    Json::Value file;
    fileResponse.ToJson(file, bForSaveDb);
    val["files"].append(file);
  }
}

string TResponse::ToJson(bool bForSaveDb) const
{
  Json::Value val;
  ToJson(val,bForSaveDb);

  Json::FastWriter writer;
  string strJson=writer.write(val);
  return strJson;
}

bool  TResponse::FromJson(const Json::Value& val){

  if(val["code"].isInt())
    nCode=val["code"].asInt();
  if(val["msg"].isString())
    strMsg=val["msg"].asString();
  if(val["totalCount"].isInt())
    nTotalCount=val["totalCount"].asInt();
  if(val["errorCount"].isInt())
    nErrorCount=val["errorCount"].asInt();
  for(int i=0; i<val["files"].size(); i++){
    TFileResponse fileResponse;
    const Json::Value& file=val["files"][i];
    if(!fileResponse.FromJson(file))
      return false;
    vecFileResponse.push_back(fileResponse);
  }
  return true;      
}

bool TResponse::FromJson(const string& strJson)
{
  Json::Value val;
  Json::Reader reader;
  if(!reader.parse(strJson,val))
    return false;
  return FromJson(val);
}

bool TResponse::Output(const TRequest& request, string& strOutput) 
{
  string strResBody;
  bool bResJson =true;//不需要回调java应用写数据库的一般返回json
  //int nHttpCode = (nCode==1)?200:500;
  int nHttpCode = 0;
  if(nCode == 1)
    nHttpCode = 200;
  else if(nCode == 0)
    nHttpCode = 500;
  else
    nHttpCode = nCode;
  vector<string>vecResHeader;
  vecResHeader.push_back("Content-Type: text/html; charset=utf-8");
  
  const TUploadConf * pUploadConf = request.pUploadConf;
  bool bHaveAfterUrl = (pUploadConf!=NULL && pUploadConf->strAfterUrl!="");


  ///image解析请求strAfterUrl的结果
  TResponse r2;
  if(bHaveSaveDb){  
    nHttpCode = nAfterHttpCode;
    if(nAfterHttpCode == 301 || nAfterHttpCode == 302){
      vecResHeader.push_back("Location: "+GetHeader(strAfterResHeader,"Location"));
      bResJson=false;
    }
    else if(pUploadConf->strChannel == "video")//视频上传,服务器返回的是什么就返回什么
    {
      strResBody = strAfterResBody;
      bResJson = false;
    }
    else{
      //解析strAfterResBody为json:成功则和本地json合并,失败且返回码为200则返回strAfterResBody
      /*bool bFromJsonOk=r2.FromJson(strAfterResBody);
      if(bFromJsonOk){
        nCode=r2.nCode;
        strMsg=r2.strMsg;
        for(size_t i=0; i<r2.vecFileResponse.size() && i<vecFileResponse.size(); i++){
          TFileResponse& f2 = r2.vecFileResponse[i]; 
          TFileResponse& f = vecFileResponse[i];
          if(f.nCode == 1){
            f.nCode = f2.nCode;
            f.strMsg = f2.strMsg;
          }
        }
      }
      else{
        strResBody = strAfterResBody;
        bResJson = false;
      }*/
      strResBody = strAfterResBody;
      bResJson = false;
    }
  }

  ///如返回json
  if(bResJson){
    if(pUploadConf->strChannel == "gossip_pic")//留言上传，直接返回json，不需要回调,并且总是返回200,不然flash没法拿到错误信息
    {
      nHttpCode = 200;
      Json::Value val;
      if((nCode != 1)){
        val["code"]=0;
        if(vecFileResponse.empty())
          val["msg"]=strMsg;
        else
          val["msg"]=vecFileResponse[0].strMsg;
      }
      else{
        if (vecFileResponse.empty()) {// 预防一下,但愿nCode机制正确
          val["msg"]=strMsg;
          LOG(ERROR) << "gossip_pic error,vecFileReponse is null,but nCode ==1";
        } else {
          const TFileResponse& fileRes = vecFileResponse[0];
          const map<string, string>& msgMap = fileRes.mapOtherMsg;
          map <string, string>::const_iterator ite;
          ite = msgMap.find("url0");
          if(ite == msgMap.end()){
            val["code"] = 0;
            val["msg"] = "查找large图出错";
          }else{
            val["large"]= ite->second;
            ite = msgMap.find("url1");
            if(ite == msgMap.end()){
              val["code"] = 0;
              val["msg"] = "查找thumbnail图出错";
            }else{
              val["code"] = 1;
              val["thumbnail"] = ite->second;
              map <string, string>::const_iterator itMd5 = msgMap.find("md5");
              if(itMd5 != msgMap.end())
                val["md5"] = itMd5->second;
            }
          }
        }
      }
      Json::FastWriter write;
      strResBody = write.write(val);
    }
    else
      strResBody = ToJson(!bHaveAfterUrl);  //保存db的信息发给afterurl即可,不用再发给client了
    /*Json::Value val;
    val["code"]=nCode;
    val["msg"]=strMsg;
    Json::FastWriter writer;
    strResBody=writer.write(val);*/
  }

  //拼返回字符串
  strOutput.clear();
  if(request.Query("pagetype") == "addPhotoPlain")//简单上传返回200，为了跳转需求
    strOutput+="Status: " + ToString<int>(200)+"\r\n";
  else
    strOutput+="Status: " + ToString<int>(nHttpCode)+"\r\n";
  for(size_t i=0; i<vecResHeader.size(); i++){
    strOutput += vecResHeader[i] +"\r\n";
  }
  strOutput+="\r\n";
  strOutput+=strResBody;

  //输日志
  
  if(pUploadConf->strChannel == "video"){//视频上传log
    const TFileResponse& file=vecFileResponse[0];
    if(!bHaveSaveDb)//保存出错，没有调用afterURL
      Log("OutputVideoFile%s %d %d %s %s(%d)" , file.nCode==1?"Ok":"Fail", nHttpCode, file.nCode, file.strMsg.c_str(), file.strFileName.c_str(),file.nFileSize);
    else
      Log("OutputVideoFile%s %d %s)" , file.nCode==1?"Ok":"Fail", nHttpCode, strResBody.c_str());
    return true;
  }else if(pUploadConf->strChannel == "head"){//头像上传
    string from = request.Query("from")!=""?request.Query("from"):"unknown";
    string hostid = request.Query("hostid")!=""?request.Query("hostid"):request.Cookie("id");
    if(request.files().empty()){//头像上传，没有文件即为错误
      Log("OutputHeadFail %s from:%s,userId:%s",strMsg.c_str(), from.c_str(), hostid.c_str());
    }else{
      const TFileResponse& file = vecFileResponse[0];//只有一个文件
      if(file.nCode != 1)//处理图片的错误
        Log("OutputHeadFail %s from:%s,userId:%s", file.strMsg.c_str(), from.c_str(),hostid.c_str());
      else{
        if(nCode != 1)//java调用错误
          Log("OutputHeadFail %s from:%s,userId:%s", strMsg.c_str(), from.c_str(),hostid.c_str());
        else
          Log("OutputHeadOk from:%s,userId:%s",from.c_str(), hostid.c_str());
      }
    }
    return true;
  }

  string strHostId=request.Query("hostid")!=""?request.Query("hostid"):request.Cookie("id");
  string strPageType = request.Query("pagetype");
  string strAlbumId = request.Query("albumid");
  string strCommonLogInfo = strHostId + " " + strPageType +" "+ strAlbumId;
  if(!request.files().empty()){
    for(size_t i=0; i<vecFileResponse.size(); i++){
      const TFileResponse& file=vecFileResponse[i];
      Log("OutputFile%s %d %d %s %s(%d) %s" , (file.nCode==1 && nCode==1)?"Ok":"Fail", nHttpCode, file.nCode, file.strMsg.c_str(), file.strFileName.c_str(), file.nFileSize,  strCommonLogInfo.c_str());
    }
  }
  else{
    string strTest;
    if(nCode!=1){  //出错提示,放在这儿很有必要,便于跟踪bug
      strTest = "output="+strOutput+",query=" +request.Query()+",env="+request.Env()+",cookie="+request.Cookie()+",aftercode="+ToString<int>(r2.nCode)+",aftermsg="+r2.strMsg+",afterbody="+strResBody;
    }
    Log("OutputFinish%s %d %d %s %d,%d %s %s" , (nCode==1?"Ok":"Fail") , nHttpCode, nCode, strMsg.c_str(), nTotalCount , nErrorCount , strCommonLogInfo.c_str(), strTest.c_str());
    for(size_t i=0; i<vecFileResponse.size(); i++){
      const TFileResponse& file=vecFileResponse[i];
      if(file.nCode!=1){
        Log("OutputFinish%s.%d %d %d %s %s(%d) %s" , file.nCode==1?"Ok":"Fail", i, nHttpCode,file.nCode, file.strMsg.c_str(), file.strFileName.c_str(), file.nFileSize, strCommonLogInfo.c_str());
      }
    }
  }
  return true;
}

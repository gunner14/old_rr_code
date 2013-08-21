#include <curl/curl.h>
#include "downloader.h"

CDownloader g_downloader;
CDownloader::CDownloader(){
     curl_global_init(CURL_GLOBAL_ALL);
}
//官方文档:http://curl.haxx.se/libcurl/c/  

int CDownloader::Download(const string& strUrl, THttpArg & httpArg)
{
  //注意,一定要long型,否则会千万内存泄漏河,如此处为int,则curl_easy_getinfo()后curl=0,会引起curl_easy_cleanup不起作用
  //因而会导致系统打开文件数很快超过系统上限,这时任何文件操作都会失败,upload_service服务于090807上线那天的不稳定就是此问题引起
  long nHttpCode=404;
  
  //初始化
  CURL *curl=curl_easy_init();
  CURLcode code;
  if(curl==NULL){
    httpArg.strError="curl_easy_init失败";
    return nHttpCode;
  }
  code = curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());

  //附加的请求header
  struct curl_slist *headers = NULL; 
  if(!httpArg.vecReqHeader.empty()){
    for(size_t i=0; i<httpArg.vecReqHeader.size(); i++){
      headers = curl_slist_append(headers,httpArg.vecReqHeader[i].c_str());
    }
  }
  headers = curl_slist_append(headers,"Expect:");    //不添加,则提交到lighttpd1.50以下版本可能会出现错误:417 Expectation Failed
  
  //请求post数据
  if(!httpArg.strReqBody.empty()){
    headers = curl_slist_append(headers,"Content-Type: text/plain");
    code=curl_easy_setopt(curl,CURLOPT_POST,1);
    code=curl_easy_setopt(curl,CURLOPT_POSTFIELDS , httpArg.strReqBody.c_str());
    code=curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE, httpArg.strReqBody.length());
  }
    
  //设置headers
  code=curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  
  //返回数据的回调写接口
  code=curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
  code=curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, LibCurlWriter);
  code=curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(httpArg.strResBody));
  code=curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, LibCurlWriter);
  code=curl_easy_setopt(curl, CURLOPT_HEADERDATA,&(httpArg.strResHeader));

  //下载
  code = curl_easy_perform(curl);
  if(code == CURLE_OK)
    code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &nHttpCode); 
  //清除
  if(headers)
    curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  return nHttpCode;
}

long CDownloader::LibCurlWriter(void *data, size_t size, size_t nmemb, void* stream)
{
  string* content=(string*)stream;
  *content+=string((char*)data,size * nmemb);
  return size * nmemb;
}

/*#include <iostream>
using namespace std;
int main(){
  int a=1;
  int b=2;
  int c=3;
  int d[10000]={0};
  int e[10]={4};
  int f=5;
  
  int*p1=new int[10000];
  int*p2=new int[10000];
  int*p3=new int[10000];
  int*p4=new int[10000];

  int arr1[10000]={0};
  int arr2[10000]={0};
  int arr3[10000]={0};
  int arr4[10000]={0};

  for(int i=0; i<1024;i++){
    THttpArg httpArg;
    //httpArg.vecReqHeader.push_back("User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.0.11) Gecko/2009060215 Firefox/3.0.11");
    //httpArg.vecReqHeader.push_back("Referer: http://fang.kuxun.cn");
    //httpArg.vecReqHeader.push_back("Cookie: BAIDUID=6FC023EA8CB60770B8DEE2166A897390:FG=1; BDFORUMPB=OLDPB; BDRCVFR[eHt_ClL0b_s]=mk3SLVN4HKm");
    //httpArg.strReqBody="lvweiheorieorieorioeir";

    CDownloader downloader;
    int nHttpCode=downloader.Download("http://www.baidu.com",httpArg);
    cout<<nHttpCode<<"\t"<<httpArg.strResBody.size()<<endl;
    //cout<<httpArg.strResHeader.size()<<"\n"<<httpArg.strResHeader<<endl;
    //cout<<httpArg.strResBody.size()<<"\n"<<httpArg.strResBody<<endl;
    //cout<<httpArg.strError<<endl;
  }
  return 0;
}*/

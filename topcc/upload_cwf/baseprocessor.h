#ifndef _BASE_PROCESSOR_H_
#define _BASE_PROCESSOR_H_
#include "datatype.h"

///上传处理基类
class CBaseProcessor
{
public:
	virtual ~CBaseProcessor(){}
	
	///处理
	virtual bool Process(TRequest& request,TResponse& response);
	
protected:	
	///判断请求是否合法
	virtual bool Validate(const TRequest& request,TResponse& response);
	
	///上传处理
	virtual bool Upload(TRequest& request, TResponse& response)=0;

	///上传后处理
	virtual bool AfterUpload(const TRequest& request, TResponse& response);
	
protected:
	///是否登录
	bool IsLogin(const TRequest& request,TResponse& response);
	
	///获取保存文件path和保存dbpath
	virtual bool GenerateSavePath(const TUploadConf *pUploadConf,const string&strSubfix,vector<string>& vecDbPath,vector<string>& vecFilePath)=0;

};
#endif

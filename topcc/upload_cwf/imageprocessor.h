#ifndef _IMAGE_PROCESSOR_H_
#define _IMAGE_PROCESSOR_H_

#include "baseprocessor.h"
#include "upload/upload.h"
#include <string>

///上传处理基类
class CImageProcessor:public CBaseProcessor
{
public:
        CImageProcessor();
protected:
	///验证
	bool Validate(const TRequest& request, TResponse& response);
	
	///上传:缩图和保存,必须是缩略图的长宽在本项目配置过的程序
	virtual bool Upload(TRequest& request, TResponse& response);

	///上传:缩图和保存,缩图的长度和宽度是由客户端提交的w和h参数决定的，仅生成一幅图
	bool UploadConfigured(const TRequest& request, TResponse& response);
	
	///上传后处理
	virtual bool AfterUpload(const TRequest& request, TResponse& response);

	///获取保存文件path和保存dbpath
	virtual bool GenerateSavePath(const TUploadConf *pUploadConf,const string&strSubfix,vector<string>& vecDbPath,vector<string>& vecFilePath);
private:
#ifndef kHeadVersion
  net::WebPool webpool_;
#endif
};
#endif

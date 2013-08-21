#ifndef _ATTACHMENT_PROCESSOR_H_
#define _ATTACHMENT_PROCESSOR_H_

#include "baseprocessor.h"

///上传处理基类
class CAttachmentProcessor:public CBaseProcessor
{
protected:	
	bool Validate(const TRequest& request, TResponse& resonse);

	///上传
	bool Upload(TRequest& request, TResponse& response);
	
	///上传后处理
	bool AfterUpload(const TRequest& request, TResponse& response);

	///获取保存文件path和保存dbpath
	bool GenerateSavePath(const TUploadConf *pUploadConf,const string&strSubfix,vector<string>& vecDbPath,vector<string>& vecFilePath);

};
#endif

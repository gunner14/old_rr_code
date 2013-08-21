#ifndef _VIDEO_PROCESSOR_H_
#define _VIDEO_PROCESSOR_H_

#include "baseprocessor.h"

class CVideoProcessor: public CBaseProcessor
{
protected:
	bool Validate(const TRequest& request, TResponse& resonse);

	bool Upload(TRequest& request, TResponse& response);

	bool AfterUpload(const TRequest& request, TResponse& response);

	bool GenerateSavePath(const TUploadConf *pUploadConf,const string&strSubfix,vector<string>& vecDbPath,vector<string>& vecFilePath);
};

#endif

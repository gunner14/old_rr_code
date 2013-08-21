#include <assert.h>

#include "attachmentprocessor.h"
#include "util.h"
#include "imagehelper.h"

bool CAttachmentProcessor::Validate(const TRequest& request, TResponse& response)
{
	const TUploadConf* pUploadConf = request.pUploadConf;
	if(NULL == pUploadConf)
		return false;
	if(request.files().empty()){
		response.nCode = 0;
		response.strMsg = "没有上传文件";
		return false;
	}
	return CBaseProcessor::Validate(request, response);
}

bool CAttachmentProcessor::Upload(TRequest& request, TResponse& response)
{
	const TUploadConf* pUploadConf = request.pUploadConf;
	if(NULL == pUploadConf)
		return false;

	const vector<TFormFile>& files = request.files();
	vector<TFileResponse>& vecFileResponse = response.vecFileResponse;
	vecFileResponse.resize(files.size());

	int nSuccess = 0;
	for(size_t i=0; i<files.size(); i++){
		const TFormFile& formFile = files[i];
		TFileResponse& fileResponse = vecFileResponse[i];
		if(fileResponse.nCode != 1)
			continue;
		try{
			//获取路经
			vector<string> vecFilePath, vecDbPath;
			const string& strFileName = fileResponse.strFileName;
			size_t index = strFileName.find(".");
			string strExt;
			if(index != string::npos)
				strExt = strFileName.substr(index+1);
			//生成一次保存地址,只是简单保存下来
			GenerateSavePath(pUploadConf, strExt, vecDbPath, vecFilePath);

			//直接保存附件
			if(!SaveFile(vecFilePath[0], formFile.data)){
				fileResponse.nCode = 0;
				fileResponse.strMsg = "保存附件出错";
				continue;
			}

			fileResponse.nCode = 1;
			fileResponse.mapOtherMsg["url"] = vecDbPath[0];
			nSuccess++;
		}
		catch(Exception & e){
			fileResponse.nCode=0;
			fileResponse.strMsg = "处理附件出错";
			string strError=e.what();
			Log("UploadAttachmentFail %s %s", EncodeUrl(formFile.filename).c_str(), strError.c_str());
		}
	}
	response.nErrorCount = response.nTotalCount - nSuccess;
	if(nSuccess == 0){
		response.nCode = 0;
		response.strMsg = "所有附件处理失败";
		return false;
	}

	return true;
}

bool CAttachmentProcessor::AfterUpload(const TRequest& request, TResponse& response)
{
	return CBaseProcessor::AfterUpload(request, response);
}

bool CAttachmentProcessor::GenerateSavePath(const TUploadConf *pUploadConf,const string& strSubfix,vector<string>& vecDbPath,vector<string>& vecFilePath)
{
	if(NULL == pUploadConf)
		return false;
	const vector<TSaveGroupConf>& vecSaveGroupConf = pUploadConf->vecSaveGroupConf;
	if(vecSaveGroupConf.empty())
		return false;

	int size = vecSaveGroupConf.size();
	int index = GetRandomNumber(0, size);
	const TSaveGroupConf& saveGroupConf = vecSaveGroupConf.at(index);

	string timeStr = GetTimeStr();
	string uniqueStr = GetUniqueStr();
	string namePrefix = pUploadConf->strSaveNamePrefix;
	namePrefix = (namePrefix != "") ? (namePrefix+"_") : "";
	string strSubDir = pUploadConf->strSaveSubDir;
	strSubDir = (strSubDir!="")?(strSubDir+"/"):"";

	string saveDir = saveGroupConf.strSaveNodePath + "/" + strSubDir + timeStr;
	Mkdir(saveDir);

	string filePath = saveDir + "/" + namePrefix + uniqueStr + "." + strSubfix;
	string dbPath = saveGroupConf.strGroupName + "/" + strSubDir + timeStr + + "/" + namePrefix + uniqueStr + "." + strSubfix;
	vecDbPath.push_back(dbPath);
	vecFilePath.push_back(filePath);
	return true;
}


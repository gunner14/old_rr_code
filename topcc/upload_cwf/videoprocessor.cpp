#include <assert.h>

#include "videoprocessor.h"
#include "util.h"
#include "imagehelper.h"

bool CVideoProcessor::Validate(const TRequest& request, TResponse& response)
{
	const TUploadConf* pUploadConf = request.pUploadConf;
	if(NULL == pUploadConf)
		return false;
	if(pUploadConf->strChannel == "video"){
		if(request.files().empty()){
			response.nCode = 0;
			response.strMsg = "没有上传文件";
			return false;
		}
	}
	return CBaseProcessor::Validate(request, response);
}

bool CVideoProcessor::Upload(TRequest& request, TResponse& response)
{
	const TUploadConf* pUploadConf = request.pUploadConf;
	if(NULL == pUploadConf)
		return false;

	const vector<TFormFile>& vecFormFile = request.files();
	vector<TFileResponse>& vecFileResponse = response.vecFileResponse;
	vecFileResponse.resize(vecFormFile.size());

	int nSuccess = 0;
	for(size_t i=0; i<vecFormFile.size(); i++){
		const TFormFile& formFile = vecFormFile[i];
		TFileResponse& fileResponse = vecFileResponse[i];
		if(fileResponse.nCode != 1)
			continue;
		try{
			//获取路经
			vector<string> vecFilePath, vecDbPath;
			//现在生成的保存地址只有一个,只是简单保存下来
			GenerateSavePath(pUploadConf, "flv", vecDbPath, vecFilePath);

			//直接保存视频，以后可能加入视频转换功能
			if(!SaveFile(vecFilePath[0], formFile.data)){
				fileResponse.nCode = 0;
				fileResponse.strMsg = "保存视频出错";
				continue;
			}

			fileResponse.nCode = 1;
			fileResponse.mapOtherMsg["url"] = vecDbPath[0];
			nSuccess++;
		}
		catch(Exception & e){
			fileResponse.nCode=0;
			fileResponse.strMsg = "处理视频出错";
			string strError=e.what();
			Log("UploadVideoFail %s %s", EncodeUrl(formFile.filename).c_str(), strError.c_str());
		}
	}
	response.nErrorCount = response.nTotalCount - nSuccess;
	if(nSuccess == 0){
		response.nCode = 0;
		response.strMsg = "所有视频处理失败";
		return false;
	}

	return true;
}

bool CVideoProcessor::AfterUpload(const TRequest& request, TResponse& response)
{
	return CBaseProcessor::AfterUpload(request, response);
}

bool CVideoProcessor::GenerateSavePath(const TUploadConf *pUploadConf,const string& strSubfix,vector<string>& vecDbPath,vector<string>& vecFilePath)
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


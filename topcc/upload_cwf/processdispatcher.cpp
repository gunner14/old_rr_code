#include "processdispatcher.h"
#include "imageprocessor.h"
#include "attachmentprocessor.h"
#include "videoprocessor.h"
#include "runtimeconfig.h"
#include "base3/logging.h"

#include <assert.h>

CProcessDispatcher::CProcessDispatcher()
{
	m_mapChannel2Processor.insert(pair<string,CBaseProcessor*>("image",new CImageProcessor()));
	m_mapChannel2Processor.insert(pair<string,CBaseProcessor*>("attachment",new CAttachmentProcessor()));
	m_mapChannel2Processor.insert(pair<string,CBaseProcessor*>("video",new CVideoProcessor()));
}
CProcessDispatcher::~CProcessDispatcher()
{
	for(map<string,CBaseProcessor*>::iterator itMap=m_mapChannel2Processor.begin(); itMap!=m_mapChannel2Processor.end(); itMap++){
		if(itMap->second!=NULL){
			delete itMap->second;
			itMap->second=NULL;
		}
	}	
}
CBaseProcessor* CProcessDispatcher::GetProcessor(const TRequest& request)
{
	///获取上传处理器
	CBaseProcessor* pProcessor=NULL;
	const TUploadConf* pUploadConf=request.pUploadConf;	
	assert(pUploadConf);
	while(pUploadConf){
		map<string,CBaseProcessor*>::iterator itMap=m_mapChannel2Processor.find(pUploadConf->strChannel);
		if(itMap!=m_mapChannel2Processor.end()){
			pProcessor=itMap->second;
			break;
		}
		pUploadConf=g_runTimeConfig.GetUploadConf(pUploadConf->strBaseChannel);
	}
	return pProcessor;
}
bool CProcessDispatcher::Process(TRequest& request,TResponse& response)
{
	CBaseProcessor* pProcessor=GetProcessor(request);
	if(!pProcessor){
		response.strMsg="找不到相应的处理器";
   LOG(ERROR) << "找不到相应的处理器";
		return false;
	}
	//处理
	return pProcessor->Process(request,response);
}

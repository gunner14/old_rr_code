#ifndef _PROCESS_DISPATCHER_H_
#define _PROCESS_DISPATCHER_H_
#include "baseprocessor.h"

///上传处理分发器
class CProcessDispatcher
{
public:
	///构造:构造m_mapChannel2Processor
	CProcessDispatcher();
	///析构:析构m_mapChannel2Processor
	~CProcessDispatcher();
	
	///获取处理器
	CBaseProcessor* GetProcessor(const TRequest& request);
	
	///处理
	bool Process(TRequest& request,TResponse& response);

private:
	map<string,CBaseProcessor*> m_mapChannel2Processor;
};
#endif

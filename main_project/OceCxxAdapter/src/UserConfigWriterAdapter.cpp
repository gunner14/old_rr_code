#include "UserConfigWriterAdapter.h"

using namespace xce::adapter;
using namespace std;
using namespace mop::hi::svc::model;
using namespace MyUtil;
using namespace boost;

//原有的供外部调用的接口
//--------------------------------------------------------------
void UserConfigWriterAdapter::setUserConfig(int userId, const UserConfigInfoPtr& info) {
	if (userId <= 0) {
		return;
	}
	getUserConfigWriter(userId)->setUserConfig(userId, info->demark());
}

void UserConfigWriterAdapter::createUserConfig(int userId, const UserConfigInfoPtr& info) {
	if (userId <= 0) {
		return;
	}
	getUserConfig10sWriter(userId)->createUserConfig(info->demark());
}

//修改接口。修改实现，功能不变
//--------------------------------------------------------------
void UserConfigWriterAdapter::reload(int userId) {
	IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
	_idSet.insert(userId);
	_mutex.notify();
}

void UserConfigWriterAdapter::run()
{
	while(true)
	{
		MyUtil::IntSeq ids;
		{
			IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_mutex);
			if(_idSet.empty())
			{
				_mutex.wait();
			}
			ids.insert(ids.end(), _idSet.begin(), _idSet.end());
			_idSet.clear();
		}
		for(MyUtil::IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it)
		{
			UserConfigWriterPrx prx = getUserConfigWriter(*it);
			if(prx)
			{
				try
				{
					prx->reload(*it);
				}catch(Ice::Exception& e)
				{
					MCE_WARN("UserConfigWriterAdapter::run reload Exception id=" << *it <<" " << e.what());
				}
			}
		}
	}
}

//获取代理
//--------------------------------------------------------------
UserConfigWriterPrx UserConfigWriterAdapter::getUserConfig10sWriter(int userId) {
	return getProxy(userId);
}

UserConfigWriterPrx UserConfigWriterAdapter::getUserConfigWriter(int userId) {
	return getProxy(userId);
}

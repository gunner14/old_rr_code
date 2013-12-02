/*
 * =====================================================================================
 *
 *       Filename:  UserDetailAdapter.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月14日 20时10分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jun.liu (), jun.liu@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */

#include    "UserDetailAdapter.h"

using namespace std;
using namespace boost;

namespace wap {
namespace adapter {

	UserDetailAdapter::~UserDetailAdapter()
	{
	
	}

	void UserDetailAdapter::userBornChange(int userId, const PropertyMap& properties)
	{
		UserDetailServicePrx prx = getProxy();
		if(prx) {
      prx->userBornChange(userId, properties);
		}
	}
	
	void UserDetailAdapter::userNameChange(int userId, const PropertyMap& properties)
	{
		UserDetailServicePrx prx = getProxy();
		if(prx) {
			prx->userNameChange(userId, properties);
		}
	}

	void UserDetailAdapter::userPassportChange(int userId, const PropertyMap& properties)
	{
		UserDetailServicePrx prx = getProxy();
		if(prx) {
			prx->userPassportChange(userId, properties);
		}
	}
	
	void UserDetailAdapter::userUrlChange(int userId, const PropertyMap& properties)
	{
		UserDetailServicePrx prx = getProxy();
		if(prx) {
			prx->userUrlChange(userId, properties);
		}
	}

	void UserDetailAdapter::userContactChange(int userId, const PropertyMap& properties)
	{
		UserDetailServicePrx prx = getProxy();
		if(prx) {
			prx->userContactChange(userId, properties);
		}
	}

};
};

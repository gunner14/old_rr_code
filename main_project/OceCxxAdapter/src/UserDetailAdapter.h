/*
 * =====================================================================================
 *
 *       Filename:  UserDetailAdapter.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月14日 19时47分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jun.liu (), jun.liu@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */

#ifndef  USERDETAILADAPTER_H
#define  USERDETAILADAPTER_H

#include  <string.h>
#include  <boost/thread.hpp>
#include  <vector>
#include  <Ice/Ice.h>
#include    "AbstractAdapter.h"
#include    "UserDetail.h"

using namespace wap::user::detail; 
using namespace std;
using namespace boost;
using namespace Ice;

namespace wap {
namespace adapter {

class UserDetailAdapter : public wap::adapter::AbstractAdapter<UserDetailServicePrx> 
{
	public:
			UserDetailAdapter() : AbstractAdapter<UserDetailServicePrx>("UserDetailIceService","commons")
			{
			
			}

			~UserDetailAdapter();
			
			/**
			 * @brief 用户生日表变更通知 
			 * 
			 **/
			void userBornChange(int userId, const PropertyMap& properties);
		
			/**
			 * @brief 用户名字表变更通知 
			 * 
			 **/
			void userNameChange(int userId, const PropertyMap& properties);
			
			/**
			 * @brief 用户账户表变更通知 
			 * 
			 **/
			void userPassportChange(int userId, const PropertyMap& properties);
		
			/**
			 * @brief 用户头像表变更通知 
			 * 
			 **/
			void userUrlChange(int userId, const PropertyMap& properties);
		
			/**
			 * @brief 用户联系方式表变更通知 
			 * 
			 **/
			void userContactChange(int userId, const PropertyMap& properties);

};

};
};

#endif  /*USERDETAILADAPTER_H*/

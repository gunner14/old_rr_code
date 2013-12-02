/*
 * FeedExploreI.h
 *
 *  Created on: Mar 22, 2012
 *      Author: rong.tang
 */
#include "./FeedExploreI.h"

#include "LogWrapper.h"
#include "ServiceI.h"
#include "FeedExploreI.h"

#include "FeedCreatorAdapter.h"

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace boost;
using namespace std;
using namespace xce::feed;
using namespace MyUtil;


void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&FeedExploreI::instance(), service.createIdentity("M", ""));
}


void FeedExploreI::AddExploreFeed(int uid, const Ice::Current& ) {
	MCE_INFO("FeedExploreI::AddExploreFeed  uid:" << uid);

	bool first_time = ExploreStatusPool::instance().ShouldAdd(uid);
	int stype = 9903;
	int version = 1;

	if (first_time) {
		//MCE_INFO("****  duspatch new");
		MyUtil::Str2StrMap props = CreateProperty(uid, time(NULL));
		try {
			FeedCreatorAdapter::instance().Create(stype, version, props);
		} catch (Ice::Exception & e) {
			MCE_WARN("FeedExplore::AddExploreFeed    Ice  error:" << e);
		} catch (std::exception & e) {
			MCE_WARN("FeedExplore::AddExploreFeed     std error:" << e.what());
		}
	}
}

MyUtil::Str2StrMap FeedExploreI::CreateProperty(int uid, time_t time) {
	MyUtil::Str2StrMap props;
	stringstream str;
	try {
//		props.insert(make_pair("time",time+""));
		str<<uid;
		props.insert(make_pair("uid",str.str()));

		str.str("");
		str<<"u("<<uid<<")";
		//MCE_INFO("CreateProperty:"<<str.str());
		props.insert(make_pair("expr",str.str()));

		str.str("");
		str<<time;
		props.insert(make_pair("time",str.str()));
//		props.insert(make_pair("from.name","tangr206"));
//		props.insert(make_pair("from.icon","tangr206"));
//		props.insert(make_pair("from.tinyimg","hdn121/20120114/2350/h_head_70WX_7a8b00034c582f75.jpg"));
//		props.insert(make_pair("from.url","http://www.renren.com/profile.do?id=259790573"));
//		props.insert(make_pair("like.id","7743105"));
//		props.insert(make_pair("merge.id","259790573481676"));
//		props.insert(make_pair("music.desc","115818人已收听</p><p>1747人已喜欢"));
//		props.insert(make_pair("music.id","481676"));
//		props.insert(make_pair("music.singer","48167dfsdf6"));
//		props.insert(make_pair("music.img","http://music.store.renren.com/album_120/n54/c788268139e262f10a8a9b3df6f536.jpg"));
//		props.insert(make_pair("music.playUrl","http://music.renren.com/musicbox?tab=1&autoPlay=true&songId=481676"));
//		props.insert(make_pair("music.url","http://music.renren.com/song/481676?from=renrenHome"));
//		props.insert(make_pair("box.url","http://music.renren.com/musicbox?tab=1&autoPlay=true&songId=240040"));
//		props.insert(make_pair("button.url","http://music.renren.com/musicbox?tab=1&autoPlay=true&songId=240040"));
//		props.insert(make_pair("music.name","xinbuliaoqing"));
//		props.insert(make_pair("listening.id","481676"));
//		props.insert(make_pair("listener","481699976"));
//		props.insert(make_pair("listener.0.id","481676"));
//		props.insert(make_pair("listener.0.name","reren"));
//		props.insert(make_pair("listener.0.url","rerdfasdfasdfaen"));

	} catch (std::exception & e) {
		MCE_WARN("FeedExplore::AddExploreFeed     std error:" << e.what());
	}

	return props;
}


//=====================

bool ExploreStatusPool::ShouldAdd(int uid) {
	int system_date = GetMDay();
	int user_date = GetUserDate(uid);

	MCE_INFO("ExploreStatusPool::ShouldAdd  uid:"<<uid
				<<"   sys day:"<< system_date
				<<"   user_date:"<< user_date
				//<< ToString()
	);
	return system_date != user_date;
}

int ExploreStatusPool::GetUserDate(int uid) {
	UserExploreStatusPtr uinfo = Find(uid, true);
	if (!uinfo) { //new
		Put(uid);
		return -1;
	}

	int date = uinfo->GetDate();
	uinfo->SetDate(GetMDay());
	return date;
}




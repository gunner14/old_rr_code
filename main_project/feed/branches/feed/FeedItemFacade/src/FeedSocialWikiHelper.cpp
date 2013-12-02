
#include "FeedSocialWikiHelper.h"
#include "util/cpp/TimeCost.h"
#include "QueryRunner.h"
#include <sstream>

using namespace xce::feed;
using namespace MyUtil;
using namespace std;
using namespace com::xiaonei::xce;

//-----------------------------------------------------------------------------
// FeedSocialWikiHelper
void FeedSocialWikiHelper::put(const FeedMeta& fm, const MyUtil::Str2StrMap& ctx)
{
	ostringstream oss;
	oss << "FeedSocialWikiHelper::put. uid: " << fm.userid << " feedid: " << fm.feedid 
		<< " type: " << fm.type << " time: " << fm.time;
	TimeCost tm = TimeCost::create(oss.str());

	SocialWikiItemSeq items;
	if (!feedmeta2wikiitem(fm, ctx, items)) {
		MCE_WARN("FeedSocialWikiHelper::put. fid: " << fm.feedid << ", userid: "
				<< fm.userid << ". error: --> feedmeta2wikiitem exception.");
		return;
	}

	Ice::Int privacy = -1;
	if (!items.empty()) 
		privacy = items.begin()->privacy;

	if (PrivacyPublic == privacy) {
		FeedSocialWikiDBHelper::instance().puts_asyn(items);
		puts2userid(items);
		puts2wikiid(items);
	}
	else if (PrivacyFriend == privacy) {
		FeedSocialWikiDBHelper::instance().puts_asyn(items);
		puts2userid(items);
	}
	else {
		MCE_WARN("FeedSocialWikiHelper::put. fid: " << fm.feedid << ", userid: "
				<< fm.userid << ". error: --> privacy=" << privacy);
	}
}

void FeedSocialWikiHelper::del(Ice::Int userid, Ice::Long feedid, const MyUtil::IntSeq& wikiids)
{
	ostringstream oss;
	oss << "FeedSocialWikiHelper::del. uid: " << userid << " feedid: " << feedid; 
	TimeCost tm = TimeCost::create(oss.str());

	if (userid <= 0 || feedid <= 0 || wikiids.empty()) {
		return;
	}
	FeedSocialWikiDBHelper::instance().remove(feedid, wikiids);
	delFromUserid(userid, wikiids, feedid);
	delFromWikiid(wikiids, feedid);
}

void FeedSocialWikiHelper::changePrivacy(Ice::Int userid, Ice::Long feedid, Ice::Int privacy, 
		const MyUtil::IntSeq& wikiids)
{
	ostringstream oss;
	oss << "FeedSocialWikiHelper::changePrivacy. uid: " << userid << " feedid: " << feedid 
		<< " privacy: " << privacy << " wiki size: " << wikiids.size(); 
	TimeCost tm = TimeCost::create(oss.str());

	if (userid <= 0 || feedid <= 0 || !checkPrivacy(privacy) || wikiids.empty()) {
		return;
	}

	SocialWikiItem item = FeedSocialWikiDBHelper::instance().get(*(wikiids.begin()), feedid);
	if (item.feedid <= 0 || item.privacy == privacy) {
		return;
	}
	int tp = item.privacy;
	item.privacy = privacy;

	FeedSocialWikiDBHelper::instance().updatePrivacy(feedid, wikiids, privacy);

	SocialWikiItemSeq seq;
	for (MyUtil::IntSeq::const_iterator it = wikiids.begin(); it != wikiids.end(); it++) {
		item.wikiid = *it;
		seq.push_back(item);
	}
	switch(privacy) {
		case PrivacyFriend:
			switch(tp) {
				case PrivacyPersonal:
					puts2userid(seq);
					break;
				case PrivacyPublic:
					delFromWikiid(wikiids, feedid);
					break;
			}
			break;
		case PrivacyPublic:
			switch(tp) {
				case PrivacyFriend:
					puts2wikiid(seq);
					break;
				case PrivacyPersonal:
					puts2userid(seq);
					puts2wikiid(seq);
					break;
			}
			break;
		case PrivacyPersonal:
			switch(tp) {
				case PrivacyPublic:
					delFromUserid(userid, wikiids, feedid); 
					delFromWikiid(wikiids, feedid);
					break;
				case PrivacyFriend:
					delFromUserid(userid, wikiids, feedid); 
					break;
			}
			break;
		default:
			break;
	}
}

void FeedSocialWikiHelper::puts2userid(const SocialWikiItemSeq& items)
{
	try {
		SocialWikiByUseridReplicaAdapter::instance().put(items);
	} catch(const Ice::Exception& e) {
		MCE_WARN("FeedSocialWikiHelper::puts2userid. size: " << items.size()
				<< ". error: " << e.what());
	} catch(...) {
		MCE_WARN("FeedSocialWikiHelper::puts2userid. size: " << items.size()
				<< ". error: UNKNOWN EXCEPTION!");
	}
}

void FeedSocialWikiHelper::puts2wikiid(const SocialWikiItemSeq& items)
{
	try {
		SocialWikiNewsByWikiidReplicaAdapter::instance().puts(items);
	} catch(const Ice::Exception& e) {
		MCE_WARN("FeedSocialWikiHelper::puts2wikiid. size: " << items.size()
				<< ". error: " << e.what());
	} catch(...) {
		MCE_WARN("FeedSocialWikiHelper::puts2wikiid. size: " << items.size()
				<< ". error: UNKNOWN EXCEPTION!");
	}
}

void FeedSocialWikiHelper::delFromUserid(Ice::Int userid, const MyUtil::IntSeq& wikiids, Ice::Long feedid)
{
	try {
		SocialWikiByUseridReplicaAdapter::instance().deleteFeed(userid, wikiids, feedid);
	} catch(const Ice::Exception& e) {
		MCE_WARN("FeedSocialWikiHelper::delFromUserid. userid: " << userid << ", fid: " << feedid
				<< ". error: " << e.what());
	} catch(...) {
		MCE_WARN("FeedSocialWikiHelper::delFromUserid. userid: " << userid << ", fid: " << feedid
				<< ". error: UNKNOWN EXCEPTION!");
	}
}
void FeedSocialWikiHelper::delFromWikiid(const MyUtil::IntSeq& wikiids, Ice::Long feedid)
{
	for (MyUtil::IntSeq::const_iterator it = wikiids.begin(); it != wikiids.end(); it++) {
		try {
			SocialWikiNewsByWikiidReplicaAdapter::instance().remove(*it, feedid);
		} catch(const Ice::Exception& e) {
			MCE_WARN("FeedSocialWikiHelper::delFromWikiid. --> news. wikiid: " << (*it)  
					<< ", fid: " << feedid << ". error: " << e.what());
		} catch(...) {
			MCE_WARN("FeedSocialWikiHelper::delFromWikiid. --> news. wikiid: " << (*it) 
					<< ", fid: " << feedid << ". error: UNKNOWN EXCEPTION!");
		}
		//try {
		//    SocialWikiHotByWikiidReplicaAdapter::instance().remove(*it, feedid);
		//} catch(const Ice::Exception& e) {
		//    MCE_WARN("FeedSocialWikiHelper::delFromWikiid. --> hot. wikiid: " << (*it)  
		//            << ", fid: " << feedid << ". error: " << e.what());
		//} catch(...) {
		//    MCE_WARN("FeedSocialWikiHelper::delFromWikiid. --> hot. wikiid: " << (*it) 
		//            << ", fid: " << feedid << ". error: UNKNOWN EXCEPTION!");
		//}
	}
}

bool FeedSocialWikiHelper::feedmeta2wikiitem(const FeedMeta& fm, const MyUtil::Str2StrMap& ctx, 
		SocialWikiItemSeq& items)
{
	items.clear();

	Ice::Int privacy = -1;
	IntSeq wikiids;
	Str2StrMap::const_iterator pit = ctx.find(PrivacyContextPrefix);
	Str2StrMap::const_iterator wit = ctx.find(WikiidContextPrefix);
	if (pit != ctx.end()) {
		istringstream sin(pit->second);
		if (!(sin >> privacy)) privacy = -1;
	}
	if (wit != ctx.end()) {
		//分隔符：空格
		istringstream sin(wit->second);
		Ice::Int id = 0;
		while (sin >> id) {
			if (id > 0) wikiids.push_back(id);
		}
	}
	if (fm.userid <= 0 || fm.feedid <= 0 || !checkPrivacy(privacy) || wikiids.empty()) {
		MCE_WARN("FeedSocialWikiHelper::feedmeta2wikiitem. error: --> uid:" 
				<< fm.userid << " fid:" << fm.feedid << " privacy:" << privacy 
				<< " size:" << wikiids.empty());
		return false;
	}

	SocialWikiItem item;
	item.feedid = fm.feedid;
	item.userid = fm.userid;
	item.time = fm.time;
	item.type = fm.type;
	item.mergeid = fm.mergeid;
	item.privacy = privacy;

	for (IntSeq::iterator it = wikiids.begin(); it != wikiids.end(); it++) {
		item.wikiid = *it;
		items.push_back(item);
	}
	return true;
}
bool FeedSocialWikiHelper::checkPrivacy(const Ice::Int privacy)
{
	return privacy >= PrivacyPublic && privacy <= PrivacyPersonal;
}

//-----------------------------------------------------------------------------
// FeedSocialWikiDBHelper
string FeedSocialWikiDBHelper::getTable(Ice::Int wikiid)
{
	ostringstream pattern;
	pattern << SW_TABLE_PRE << (wikiid % SW_TABLE_COUNT);
	return pattern.str();
}

Statement FeedSocialWikiDBHelper::getSql(const SocialWikiItem& item)
{
	Statement sql;
	sql << "INSERT IGNORE INTO " << getTable(item.wikiid)
		<< " (" << SW_COL_FEEDID << ", " << SW_COL_WIKIID << ", " << SW_COL_USERID 
		<< ", " << SW_COL_TYPE << ", " << SW_COL_PRIVACY << ", " << SW_COL_TIME 
		<< ", " << SW_COL_MERGEID << ")"
		<< " VALUES(" << item.feedid << ", " << item.wikiid << ", " << item.userid
		<< ", " << item.type << ", " << item.privacy << ", " << item.time 
		<< ", " << item.mergeid << ")";
	return sql;
}

AsynDBWriterPtr FeedSocialWikiDBHelper::getWriter(Ice::Int wikiid)
{
	if (wikiid < 0) {
		return NULL;
	}
	Ice::Int mod = wikiid % SW_TABLE_COUNT;
	{
		IceUtil::RWRecMutex::RLock lock(mutex_[mod]);
		if (writers_[mod])
			return writers_[mod];
	}
	{
		IceUtil::RWRecMutex::WLock lock(mutex_[mod]);
		writers_[mod] = new AsynDBWriter(mod, getTable(mod));
		if (writers_[mod])
			TaskManager::instance().scheduleRepeated(writers_[mod]);
		return writers_[mod];
	}
}

void FeedSocialWikiDBHelper::put(const SocialWikiItem& item) 
{
	Statement sql = getSql(item);
	try {
		QueryRunner(SW_INSTANCE_NAME, CDbWServer, getTable(item.wikiid)).store(sql);
	} catch (const mysqlpp::Exception& e) {
		MCE_WARN("FeedSocialWikiDBHelper:put. feedid: " << item.feedid << ", wikiid: " << item.wikiid
				<< ", userid: " << item.userid << ", privacy: " << item.privacy << ", type"
				<< item.type << ". error: " << e.what());
	} catch(...) {
		MCE_WARN("FeedSocialWikiDBHelper:put. feedid: " << item.feedid << ", wikiid: " << item.wikiid
				<< ", userid: " << item.userid << ", privacy: " << item.privacy << ", type"
				<< item.type << ". error: UNKNOWN EXCEPTION!");
	}
}

void FeedSocialWikiDBHelper::puts(const SocialWikiItemSeq& items)
{
	for (SocialWikiItemSeq::const_iterator it = items.begin(); it != items.end(); it++) {
		put(*it);
	}
}

void FeedSocialWikiDBHelper::put_asyn(const SocialWikiItem& item)
{
	AsynDBWriterPtr writer = getWriter(item.wikiid);
	if (!writer) {
		MCE_WARN("FeedSocialWikiDBHelper::put_asyn. cannot get writer.");
	}
	else {
		//log
		writer->put(item);
	}
}

void FeedSocialWikiDBHelper::puts_asyn(const SocialWikiItemSeq& items)
{
	//for (SocialWikiItemSeq::const_iterator it = items.begin(); it != items.end(); it++) {
	//    put_asyn(*item);
	//}
	map<Ice::Int, vector<SocialWikiItemSeq::const_iterator> > mp;
	map<Ice::Int, vector<SocialWikiItemSeq::const_iterator> >::iterator mpit;
	for (SocialWikiItemSeq::const_iterator it = items.begin(); it != items.end(); it++) {
		Ice::Int mod = it->wikiid % SW_TABLE_COUNT;
		if ((mpit = mp.find(mod)) != mp.end())
			mpit->second.push_back(it);
		else {
			vector<SocialWikiItemSeq::const_iterator> seq;
			seq.push_back(it);
			mp.insert(make_pair<Ice::Int, vector<SocialWikiItemSeq::const_iterator> >(mod, seq));
		}
	}

	for (mpit = mp.begin(); mpit != mp.end(); mpit++) {
		AsynDBWriterPtr writer = getWriter(mpit->first);
		if (!writer) continue;
		vector<SocialWikiItemSeq::const_iterator>& tmp = mpit->second;
		for (size_t i = 0; i < tmp.size(); i++)
			writer->put(*tmp[i]);
	}
}

SocialWikiItem FeedSocialWikiDBHelper::get(Ice::Int wikiid, Ice::Long feedid)
{
	SocialWikiItem item = {0};
	//TODO
	Statement sql;
	sql << "SELECT * FROM " << getTable(wikiid)
		<< " WHERE " << SW_COL_WIKIID << "= " << wikiid
		<< " AND " << SW_COL_FEEDID << "= " << feedid;

	mysqlpp::StoreQueryResult res;
	try {
		res = QueryRunner(SW_INSTANCE_NAME, CDbRServer, getTable(wikiid)).store(sql);
	} catch (mysqlpp::Exception& e) {
		MCE_WARN("FeedSocialWikiDBHelper::get. wikiid: " << wikiid << ", fid: " << feedid 
				<< ". error: " << e.what()); 
	} catch (...) {
		MCE_WARN("FeedSocialWikiDBHelper::get. wikiid: " << wikiid << ", fid: " << feedid 
				<< ". error: UNKNOWN EXCEPTION!"); 
	}

	if (res.empty()) {
		MCE_WARN("FeedSocialWikiDBHelper::get. wikiid: " << wikiid << ", fid: " << feedid 
				<< ". warn: result empty."); 
	}
	else {
		mysqlpp::StoreQueryResult::iterator it = res.begin();

		item.feedid = (Ice::Long)(*it)[SW_COL_FEEDID.c_str()];
		item.wikiid = (Ice::Int)(*it)[SW_COL_WIKIID.c_str()];
		item.userid = (Ice::Int)(*it)[SW_COL_USERID.c_str()];
		item.type = (Ice::Int)(*it)[SW_COL_TYPE.c_str()];
		item.privacy = (Ice::Int)(*it)[SW_COL_PRIVACY.c_str()];
		item.time = (Ice::Int)(*it)[SW_COL_TIME.c_str()];
		item.mergeid = (Ice::Long)(*it)[SW_COL_MERGEID.c_str()];
	}
	return item;
}

void FeedSocialWikiDBHelper::remove(Ice::Long feedid, const MyUtil::IntSeq& wikiids)
{
	map<Ice::Int, MyUtil::IntSeq> mod2wid;
	map<Ice::Int, MyUtil::IntSeq>::iterator m2w_it;
	for (MyUtil::IntSeq::const_iterator it = wikiids.begin(); it != wikiids.end(); it++) {
		Ice::Int mod = (*it) % SW_TABLE_COUNT;
		if ((m2w_it = mod2wid.find(mod)) != mod2wid.end()) {
			m2w_it->second.push_back(*it);
		}
		else {
			MyUtil::IntSeq seq;
			seq.push_back(*it);
			mod2wid.insert(make_pair<Ice::Int, MyUtil::IntSeq>(mod, seq));
		}
	}

	for (m2w_it = mod2wid.begin(); m2w_it != mod2wid.end(); m2w_it++) {
		Statement sql;
		sql << "DELETE FROM " << getTable(m2w_it->first)
			<< " WHERE feedid=" << feedid
			<< " AND wikiid in (";
		MyUtil::IntSeq& seq = m2w_it->second;
		for (MyUtil::IntSeq::iterator it = seq.begin(); it != seq.end(); it++) {
			if (it != seq.begin())
				sql << ", ";
			sql << (*it);
		}
		sql << ")";

		QueryRunner(SW_INSTANCE_NAME, CDbWServer, getTable(m2w_it->first)).schedule(sql);
	}
}

void FeedSocialWikiDBHelper::updatePrivacy(Ice::Long feedid, const MyUtil::IntSeq& wikiids,
		Ice::Int privacy)
{
	map<Ice::Int, MyUtil::IntSeq> mod2wid;
	map<Ice::Int, MyUtil::IntSeq>::iterator m2w_it;
	for (MyUtil::IntSeq::const_iterator it = wikiids.begin(); it != wikiids.end(); it++) {
		Ice::Int mod = (*it) % SW_TABLE_COUNT;
		if ((m2w_it = mod2wid.find(mod)) != mod2wid.end()) {
			m2w_it->second.push_back(*it);
		}
		else {
			MyUtil::IntSeq seq;
			seq.push_back(*it);
			mod2wid.insert(make_pair<Ice::Int, MyUtil::IntSeq>(mod, seq));
		}
	}

	for (m2w_it = mod2wid.begin(); m2w_it != mod2wid.end(); m2w_it++) {
		Statement sql;
		sql << "UPDATE IGNORE " << getTable(m2w_it->first)
			<< " SET privacy=" << privacy
			<< " WHERE feedid=" << feedid
			<< " AND wikiid in (";
		MyUtil::IntSeq& seq = m2w_it->second;
		for (MyUtil::IntSeq::iterator it = seq.begin(); it != seq.end(); it++) {
			if (it != seq.begin())
				sql << ", ";
			sql << (*it);
		}
		sql << ")";

		QueryRunner(SW_INSTANCE_NAME, CDbWServer, getTable(m2w_it->first)).schedule(sql);
	}
}

//-----------------------------------------------------------------------------
// AsynDBWriter
void AsynDBWriter::put(const SocialWikiItem& item)
{
	IceUtil::Mutex::Lock lock(mutex_);
	items_.push_back(item);
}

void AsynDBWriter::handle()
{
	SocialWikiItemSeq tmp;
	{
		IceUtil::Mutex::Lock lock(mutex_);
		items_.swap(tmp);
	}

	if (tmp.empty()) {
		return;
	}
	Statement sql;
	sql << "INSERT IGNORE INTO " << table_
		<< " (" << SW_COL_FEEDID << ", " << SW_COL_WIKIID << ", " << SW_COL_USERID 
		<< ", " << SW_COL_TYPE << ", " << SW_COL_PRIVACY << ", " << SW_COL_TIME 
		<< ", " << SW_COL_MERGEID << ")" << " VALUES "; 
	for (SocialWikiItemSeq::iterator it = tmp.begin(); it != tmp.end(); it++) {
		if (it != tmp.begin())
			sql << ", ";
		sql << "( " << it->feedid << ", " << it->wikiid << ", " << it->userid
			<< ", " << it->type << ", " << it->privacy << ", " << it->time 
			<< ", " << it->mergeid << " )";
	}
	try {
		QueryRunner(SW_INSTANCE_NAME, CDbWServer, table_).store(sql);
		MCE_DEBUG("AsynDBWriter::handle. talbe: " << table_ << ", save size: " << tmp.size());
	} catch (const mysqlpp::Exception& e) {
		MCE_WARN("AsynDBWriter::handle. error: " << e.what());
	} catch(...) {
		MCE_WARN("AsynDBWriter::handle. error: UNKNOWN EXCEPTION!");
	}
}



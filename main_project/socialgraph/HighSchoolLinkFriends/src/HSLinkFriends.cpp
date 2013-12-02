#include "HSLinkFriends.h"
#include "BuddyByAddTimeReplicaAdapter.h"
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;

static int cluster = 100;

HSLinkFriends::HSLinkFriends( string fileNamePatt) {
	cout<<"[HSLinkFriends::HSLinkFriends] ConnectionPoolManager initializing" << endl;
	ConnectionPoolManager::instance().initialize();
	cout<<"[HSLinkFriends::HSLinkFriends] ConnectionPoolManager initialize done" << endl;

	_succ = true;
	for( int i=0;i<cluster;++i ){
		ostringstream filename;
		filename << fileNamePatt << i;
		//filename << "/mnt/dsk11/sg/PREFIX_" << i;
		ofstream* tmp = new ofstream( filename.str().c_str() );
		_ofss.push_back( tmp );
        	if( _ofss[i]->is_open() ){
        		cout << "opening file: " << filename.str() << endl;
        	} else{
			_succ = false;
                	cout << "Error opening file" << filename.str() << endl;
		}
	}
}

HSLinkFriends::~HSLinkFriends(){
	for( int i=0;i<cluster;++i ){
		if( _ofss[i]->is_open() ){
			_ofss[i]->close();
		}
		delete _ofss[i];
	}
}

void HSLinkFriends::collectHSData(){
	if( !_succ ){
		cout << "[HSLinkFriends::collectHSData] File Not Open!" << endl;
		return;
	}
	int beginTime = time(NULL);
	cout << "[HSLinkFriends::collectHSData] start in highschool_info at time: " << beginTime << endl;
	int countTotal = 0;

	int max_id = 0;
	int last_id = 0;
	do {
		map<int, map<int,HSData> > uds;
		Statement sql;
        
		sql	<< "SELECT userid,high_school_id, h_class1, enroll_year FROM high_school_info "
			<< " WHERE userid >= " << max_id
			<< " LIMIT 100000";
        //sql << "select userid, high_school_name, h_class1, enroll_year from high_school_info where high_school_id = 10000261";
		size_t count = QueryRunner("high_school_info_friendfinder", CDbRServer).query(sql,
				HSDataByIDHandler(last_id,uds));
		countTotal += count;
        cout << count << endl;
		map<int, map<int,HSData> > uds_linked = linkFriends( uds );

		writeFile( uds_linked );

		if (last_id > max_id) {
			max_id = last_id;
		} else if( count > 0) {
			++max_id;
			++last_id;
		} else{
			break;
		}
		int thisTime = time(NULL);
		cout << "[HSLinkFriends::collectHSData] collecting to " << last_id << " count: " << countTotal << " timecost: " << (thisTime-beginTime)/60/60 << "h" << ((thisTime-beginTime)/60)%60 << "m" << endl;
	} while (true);

	int endTime = time(NULL);
	cout << "[HSLinkFriends::collectHSData] ended at time: " << endTime << ". cost : " << (endTime-beginTime)/60/60 << "h" << ((endTime-beginTime)/60)%60 << "m" << endl;
}

map<int, map<int,HSData> > HSLinkFriends::linkFriends(map<int, map<int,HSData> >& uds){
	map<int, map<int,HSData> > res;
	int bTotal = 0;
	int aTotal = 0;
	for (map<int, map<int,HSData> >::iterator itMap1 = uds.begin(); itMap1
			!= uds.end(); ++itMap1) {
		IntSeq ids;
		for ( map<int,HSData>::iterator itMap2 = itMap1->second.begin(); itMap2 != itMap1->second.end(); ++itMap2) {
			ids.push_back( itMap2->first );
		}
		bTotal += itMap1->second.size();

		map<int,string> friends;

		//get friends 
		MyUtil::Int2IntSeqMap buddies = xce::buddy::adapter::BuddyByAddTimeCacheAdapter::instance().getFriendLists(ids);
		for( MyUtil::Int2IntSeqMap::iterator it1=buddies.begin(); it1!=buddies.end(); ++it1 ){
			if( it1->second.size() <= 0 ){
				continue;
			}
			string& guestStr = friends[it1->first];
			for( MyUtil::IntSeq::iterator it2=it1->second.begin(); it2!=it1->second.end(); ++it2 ){
				guestStr = guestStr + "" + boost::lexical_cast<string>(*it2) + ",";
			}
		} 

		aTotal += friends.size();

		cout << "[HSLinkFriends::linkFriends] linking mod " << itMap1->first << " size/fSize: " << itMap1->second.size() << "/" << friends.size() << endl;

		for ( map<int,string>::iterator itMap3 = friends.begin(); itMap3 != friends.end(); ++itMap3) {
			map<int,HSData>::iterator itMap4 = itMap1->second.find( itMap3->first );
			HSData ud = itMap4->second;
			ud.has_friends = true;
			ud.friend_list = itMap3->second;

			res[itMap1->first].insert( make_pair(itMap4->first,ud) );
		}
	}

	cout << "[HSLinkFriends::linkFriends] size/linkedSize: " << bTotal << "/" << aTotal << endl;

	return res;
}

/*
map<int, map<int,HSData> > HSLinkFriends::linkFriends(map<int, map<int,HSData> >& uds){
	map<int, map<int,HSData> > res;
	int bTotal = 0;
	int aTotal = 0;
	for (map<int, map<int,HSData> >::iterator itMap1 = uds.begin(); itMap1
			!= uds.end(); ++itMap1) {
		int min = 0;
		int max = 0;
		map<int,HSData>::iterator itMap2 = itMap1->second.begin();
		if( itMap2 != itMap1->second.end() ){
			min = itMap2->first;
			max = min+1;
		} else{
			continue;
		}
		for ( ++itMap2; itMap2 != itMap1->second.end(); ++itMap2) {
			if( itMap2->first < min ){
				min = itMap2->first;
			}
			if( itMap2->first >= max ){
				max = itMap2->first + 1;
			}
		}
		bTotal += itMap1->second.size();

		map<int,string> friends;

		Statement sql;
		sql     << "SELECT host,guest"
			<< " FROM relation_" << itMap1->first
			<< " WHERE host >= " << min << " AND host < " << max;
		ostringstream pattern;
		pattern << "relation_" << itMap1->first;
		//QueryRunner("relation_buddycache", CDbRServer, pattern.str()).query(
		QueryRunner("friendfinder_friend", CDbRServer, pattern.str()).query(
				sql, FriendsHandler( friends ));

		for ( map<int,string>::iterator itMap3 = friends.begin(); itMap3 != friends.end(); ++itMap3) {
			map<int,HSData>::iterator itMap4 = itMap1->second.find( itMap3->first );
			if( itMap4 == itMap1->second.end() ){
				continue;
			}
			HSData ud = itMap4->second;
			ud.has_friends = true;
			ud.friend_list = itMap3->second;

			res[itMap1->first].insert( make_pair(itMap4->first,ud) );
			aTotal++;
		}

		cout << "[HSLinkFriends::linkFriends] linking mod " << itMap1->first << " size/fSize: " << itMap1->second.size() << "/" << friends.size() << endl;

	}

	cout << "[HSLinkFriends::linkFriends] size/linkedSize: " << bTotal << "/" << aTotal << endl;

	return res;
}
*/

/*
map<int, map<int,HSData> > HSLinkFriends::linkFriends(map<int, map<int,HSData> >& uds){
	map<int, map<int,HSData> > res;
	int bTotal = 0;
	int aTotal = 0;
	for (map<int, map<int,HSData> >::iterator itMap1 = uds.begin(); itMap1
			!= uds.end(); ++itMap1) {
		ostringstream ids;
		map<int,HSData>::iterator itMap2 = itMap1->second.begin();
		if( itMap2 != itMap1->second.end() ){
			ids << itMap2->first;
		} else{
			continue;
		}
		for ( ++itMap2; itMap2 != itMap1->second.end(); ++itMap2) {
			ids << "," << itMap2->first;
		}
		bTotal += itMap1->second.size();

		map<int,string> friends;

		Statement sql;
		sql     << "SELECT host,guest"
			<< " FROM relation_" << itMap1->first
			<< " WHERE host IN (" << ids.str() << ")";
		ostringstream pattern;
		pattern << "relation_" << itMap1->first;
		QueryRunner("relation_buddycache", CDbRServer, pattern.str()).query(
				sql, FriendsHandler( friends ));

		aTotal += friends.size();

		cout << "[HSLinkFriends::linkFriends] linking mod " << itMap1->first << " size/fSize: " << itMap1->second.size() << "/" << friends.size() << endl;

		for ( map<int,string>::iterator itMap3 = friends.begin(); itMap3 != friends.end(); ++itMap3) {
			map<int,HSData>::iterator itMap4 = itMap1->second.find( itMap3->first );
			HSData ud = itMap4->second;
			ud.has_friends = true;
			ud.friend_list = itMap3->second;

			res[itMap1->first].insert( make_pair(itMap4->first,ud) );
		}
	}

	cout << "[HSLinkFriends::linkFriends] size/linkedSize: " << bTotal << "/" << aTotal << endl;

	return res;
}
*/

void HSLinkFriends::writeFile(std::map<int, std::map<int,HSData> >& uds){
	for (map<int, map<int,HSData> >::iterator itMap1 = uds.begin(); itMap1
			!= uds.end(); ++itMap1) {
		cout << "[HSLinkFriends::writeFile] ------ writing at file: " << itMap1->first << " size: " << itMap1->second.size() << endl;
		for (map<int,HSData>::iterator itMap2 = itMap1->second.begin(); itMap2
				!= itMap1->second.end(); ++itMap2) {
			ostringstream ctx;
			ctx	<< itMap2->second.userid << "\t"
				<< itMap2->second.high_school_id << "\t"
				<< itMap2->second.h_class << "\t"
				<< itMap2->second.enroll_year << "\t"
				<< itMap2->second.friend_list << "\t"
				<< "\n";
        		*(_ofss[itMap1->first]) << ctx.str();
		}
	}
}


bool HSDataByIDHandler::handle(mysqlpp::Row& row) const{

        int userid = row["userid"];
        int high_school_id = row["high_school_id"];
        int h_class = row["h_class1"];
        int enroll_year = row["enroll_year"];

	HSData ud;
	ud.userid = userid;


	ud.high_school_id = high_school_id;
	ud.h_class = h_class;

	ud.enroll_year = enroll_year;

	ud.has_friends = false;
	ud.friend_list = "fNull";

	_results[userid%cluster].insert( make_pair(userid,ud) );

	_last_id = userid > _last_id ? userid : _last_id;

	return true;
}

bool FriendsHandler::handle(mysqlpp::Row& row) const{

        int host = row["host"];
        int guest = row["guest"];
	string& guestStr = _results[host];
	guestStr = guestStr + "" + boost::lexical_cast<string>(guest) + ",";

	return true;
}


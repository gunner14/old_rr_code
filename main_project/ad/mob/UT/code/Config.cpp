#include "Config.h"
#include "Struct.h"

using namespace boost;
using namespace xce::ad;
using namespace MyUtil;
using namespace com::xiaonei::xce;

string EngineConfig::GetProperty(string& key) {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<string, string>::const_iterator it = property_dict_.find(key);
	if ( it != property_dict_.end() ) {
		return it->second;
	}
	return "";
}

string EngineConfig::GetPropertyWithDefault(string& key, string& value) {
	string result = GetProperty(key);
	return ( true == result.empty() ) ? value : result;
}

int EngineConfig::GetPropertyAsInt(string& key) {
	return atoi( GetProperty(key).c_str() );
}

int EngineConfig::GetPropertyAsIntWithDefault(string& key, int value) {
	string result = GetProperty(key);
	return ( true == result.empty() ) ? value : atoi(result.c_str());
}

void EngineConfig::Print() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<string, string>::const_iterator it = property_dict_.begin();
	for (; it != property_dict_.end(); ++it) {
		MCE_INFO("EngineConfig::Print --> property:  [" << it->first << "]:" << it->second);
	}
}

size_t EngineConfig::Size() {
	return property_dict_.size();
}


Ice::Long PlatformMap::GetPlatform(string OsVersion) {
	Ice::Long ans = 0;
	size_t pos;
	string version = OsVersion;
	try {
		pos = OsVersion.find(".");
		if ( pos != string::npos ) {
			version = OsVersion.substr(0,pos+2);
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("PlatformMap::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("PlatformMap::TotalLoad : std exception: " << e.what());
	}

	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<string, AdPlatformPtr>::iterator it = pool_.find( version );
	if (it != pool_.end()) {
		ans = it->second->code();
	}
	return ans;
}

Ice::Long PlatformMap::GetPlatformX(string OsVersion) {
	Ice::Long ans = 0;
	size_t pos;
	string versionx = OsVersion;
	try {
		pos = OsVersion.find(".");
		if ( pos != string::npos ) {
			versionx = OsVersion.substr(0,pos+2);
			versionx[pos+1]='x';
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("PlatformMap::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("PlatformMap::TotalLoad : std exception: " << e.what());
	}

	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<string, AdPlatformPtr>::iterator it = pool_.find( versionx );
	if (it != pool_.end()) {
		ans = it->second->code();
	}
	return ans;
}

void PlatformMap::TotalLoad() {
	MCE_INFO("PlatformMap::TotalLoad --> start");
	Statement sql; 
	map<string, AdPlatformPtr> local_pool;
	sql << " SELECT    code, platform_name, platform_version "
			<< " FROM      3g_platform ";

	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "platform", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				AdPlatformPtr platform = new AdPlatform;
				mysqlpp::Row row = res.at(i);
				platform->set_code( mysql_long(row["code"]) );
				platform->set_name( mysql_string(row["platform_name"]) );
				platform->set_version( mysql_string(row["platform_version"]) );

				local_pool[ platform->name()+platform->version() ] = platform;
			}
			MCE_INFO("PlatformMap::TotalLoad --> end <DB Table 3G_platform> size: " << res.size() << " platform_map size:" << local_pool.size() );
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				pool_.swap(local_pool);
			}
		} else {
			MCE_WARN("PlatformMap::TotalLoad --> end <DB> 3G_platform have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("PlatformMap::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("PlatformMap::TotalLoad : std exception: " << e.what());
	}
}

size_t PlatformMap::Size() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	return pool_.size();
}

string PlatformMap::Print(const int size) {
	stringstream result;
	result << "[platform map]:\n";
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<string, AdPlatformPtr>::iterator it = pool_.begin();
	int size_local = (size>0)?size:65535;
	for(int i=1; it != pool_.end() && i <= size_local;it++,i++) {
		result << "["<<i<<"]"<<it->second->code()<<" "<<it->second->name()<<" "<<it->second->version()<<" ";
		if ( i % 5 == 0 ) {
			result << "\n";
		}
	}
	return result.str();
}

Ice::Long Brand3GMap::GetBrand3G(string brand3G) {
	Ice::Long ans = 0;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<string, Ice::Long>::iterator it = pool_.find( brand3G );
	if (it != pool_.end()) {
		ans = it->second;
	}
	return ans;
}

void Brand3GMap::TotalLoad() {
	MCE_INFO("Brand3GMap::TotalLoad --> start");
	Statement sql; 
	map<string, Ice::Long> local_pool;
	sql << " SELECT    code, Brand_name "
			<< " FROM      3g_brand ";

	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "brand3G", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				mysqlpp::Row row = res.at(i);
				Ice::Long code = mysql_long(row["code"]);
				string name = mysql_string(row["Brand_name"]);
				local_pool[name] = code;
			}
			MCE_INFO("Brand3GMap::TotalLoad --> end <DB Table 3G_network> size: " << res.size() << " network_map size:" << local_pool.size() );
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				pool_.swap(local_pool);
			}
		} else {
			MCE_WARN("Brand3GMap::TotalLoad --> end <DB> 3G_network have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("Brand3GMap::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("Brand3GMap::TotalLoad : std exception: " << e.what());
	}
}

size_t Brand3GMap::Size() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	return pool_.size();
}

string Brand3GMap::Print(const int size) {
	stringstream result;
	result << "[brand3G map]:\n";
	int size_local = (size>0)?size:65535;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	map<string, Ice::Long>::iterator it = pool_.begin();
	if (size <= 0 ) {
		size_local = 65530;
	} else {
		size_local = size;
	}
	for(int i=1;it != pool_.end() && i <= size_local;it++,i++) {
		result << "["<<i<<"]"<<it->first<<" "<<it->second<<" ";
		if ( i % 5 == 0 ) {
			result << "\n";
		}
	}
	return result.str();
}

short AbstractResolutionMap::GetFixedWidth(short width) {
	//由于一些手机发来的分辨率不是标准格式，例如320*480可能传为319*480或321*480
	//在此对分辨率做统一处理
	if ( width > 5) {
		width -= 5;
	}
	set<short> widths = GetCeilWidths(width);
	set<short>::iterator it = widths.begin();
	short min = MAX_WIDTH;
	if ( false == widths.empty() && true == sorted_width_seq_.empty() ) {
		return min;
	}
	for(;it != widths.end(); ++it) {
		if ( min > *it ) {
			min = *it;
		}
	}
	return min;
}

set<short> AbstractResolutionMap::GetCeilWidths(short width) {
	set<short> ans;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	vector< pair<short, string> >::iterator it = sorted_width_seq_.begin();
	for(;it != sorted_width_seq_.end();++it) {
		if ( width <= (*it).first ) {
			ans.insert( (*it).first );
		}
	}
	return ans;
}

set<short> AbstractResolutionMap::GetFloorWidths(short width) {
	set<short> ans;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	vector< pair<short, string> >::iterator it = sorted_width_seq_.begin();
	for(;it != sorted_width_seq_.end();++it) {
		if ( width >= (*it).first ) {
			ans.insert( (*it).first );
		}
	}
	return ans;
}

short AbstractResolutionMap::GetFloorWidth(short width) {
	short ans = 0;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	vector< pair<short, string> >::iterator it = sorted_width_seq_.begin();
	for(;it != sorted_width_seq_.end();++it) {
		if ( width >= (*it).first ) {
			ans = (*it).first;
		} else {
			break;
		}
	}
	return ans;
}

size_t AbstractResolutionMap::Size() {
	IceUtil::RWRecMutex::RLock lock(mutex_);
	return sorted_width_seq_.size();
}

string AbstractResolutionMap::Print(const int size) {
	stringstream result;
	result << "[Resolution map]:\n";
	int i = 1;
	int size_local = (size>0)?size:65535;
	IceUtil::RWRecMutex::RLock lock(mutex_);
	vector< pair<short, string> >::iterator it = sorted_width_seq_.begin();
	for(;it != sorted_width_seq_.end() && i <= size_local;it++,i++) {
		result << "["<<i<<"]"<<it->first<<" "<<it->second<<" ";
		if ( i % 5 == 0 ) {
			result << "\n";
		}
	}
	return result.str();
}

void ResolutionMap::TotalLoad() {
	MCE_INFO("ResolutionMap::TotalLoad --> start");
	Statement sql; 
	vector< pair<short, string> > local_width_seq;
	sql << " SELECT    code, resolution, width, height "
			<< " FROM      3g_resolution ";

	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "resolution", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				mysqlpp::Row row = res.at(i);
				Ice::Long width = mysql_long(row["width"]);
				string resolution = mysql_string(row["resolution"]);
				local_width_seq.push_back( make_pair(width, resolution) );
			}
			MCE_INFO("ResolutionMap::TotalLoad --> end <DB> 3G_network size: " << res.size() << " network_map size:" << local_width_seq.size() );
			sort( local_width_seq.begin(), local_width_seq.end() );
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				sorted_width_seq_.swap(local_width_seq);
			}
		} else {
			MCE_WARN("ResolutionMap::TotalLoad --> end <DB> 3G_network have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("ResolutionMap::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("ResolutionMap::TotalLoad : std exception: " << e.what());
	}
}

void ResolutionMatcher::TotalLoad() {
	MCE_INFO("ResolutionMatcher::TotalLoad --> start");
	Statement sql; 
	vector< pair<short, string> > local_width_seq;
	sql << " SELECT    code, resolution, width, height "
			<< " FROM      3g_widget_resolution ";

	mysqlpp::StoreQueryResult res;
	try {
		Util::ExecuteQuery(sql, GetDB(), "resolution", res);
		if ( res && res.num_rows() > 0 ) {
			for( size_t i = 0; i < res.num_rows(); i++){
				mysqlpp::Row row = res.at(i);
				Ice::Long width = mysql_long(row["width"]);
				string resolution = mysql_string(row["resolution"]);
				local_width_seq.push_back( make_pair(width, resolution) );
			}
			MCE_INFO("ResolutionMatcher::TotalLoad --> end <DB> 3G_network size: " << res.size() << " network_map size:" << local_width_seq.size() );
			sort( local_width_seq.begin(), local_width_seq.end() );
			{
				IceUtil::RWRecMutex::WLock lock(mutex_);
				sorted_width_seq_.swap(local_width_seq);
			}
		} else {
			MCE_WARN("ResolutionMatcher::TotalLoad --> end <DB> 3G_network have no data");
		}
	} catch (Ice::Exception& e) {
		MCE_WARN("ResolutionMatcher::TotalLoad : Ice Exception: " << e);
	} catch (std::exception& e) {
		MCE_WARN("ResolutionMatcher::TotalLoad : std exception: " << e.what());
	}
}


#ifndef _SEARCHJSONHELPER_H_
#define _SEARCHJSONHELPER_H_
#include <json/json.h>
#include "SearchCacheData.h"
#include <boost/foreach.hpp>
namespace xce {
namespace searchcache {

using namespace boost;
class JsonHelper{
public:
	// cat c.py|tr -d ";"  |awk '{print "root[\""$2"\"] = data->"$2";"}'|sed "s/\[\"_/[\"/"
	static bool convert(const SearchCacheDataPtr & data,Json::Value & root){
		root["id"] = data->_id;
		root["name"] = data->_name;
		root["gender"] = data->_gender;
		root["signature"] = data->_signature;
		root["star"] = data->_star;
		root["status"] = data->_status;
		root["stage"] = data->_stage;
		root["birthYear"] = data->_birthday_year;
		root["birthMonth"] = data->_birthday_month;
		root["birthDay"] = data->_birthday_day;
		root["state"] = data->_state;
		root["lastLoginTime"] = data->_lastlogintime;
		root["tinyUrl"] = data->_tinyurl;
		root["headUrl"] = data->_headurl;
		root["astrology"] = data->_astrology;
		root["browseConfig"] = data->_browse_config;
		root["statusConfig"] = data->_status_config;
		root["basicConfig"] = data->_basic_config;
		root["auth"] = data->_auth;
		root["viewCount"] = data->_viewcount;
		root["homeCity"] = data->_home_city;
		root["homeProvince"] = data->_home_province;
		//root["interest"] = data->_interest;
		//root["music"] = data->_music;
		//root["movie"] = data->_movie;
		//root["game"] = data->_game;
		//root["comic"] = data->_comic;
		//root["sport"] = data->_sport;
		//root["book"] = data->_book;
		//root["society"] = data->_society;
		root["profilePrivacy"] = data->_profile_privacy;

		root["fav"] = data->_interest+ " " + data->_music+ " " + data->_movie+ " " + data->_game+ " " + data->_comic+ " " + data->_sport+ " " + data->_book + " " + data->_society;

		convertNetwork(data->_networks,root);

		convertElementarySchool(data->_elementarySchools,root);

		convertJuniorSchool(data->_juniorSchools,root);

		convertHighSchool(data->_highSchools,root);

		convertCollege(data->_colleges,root);

		convertUniversity(data->_universities,root);

		convertWorkplace(data->_workplaces,root);

		convertRegion(data->_regions,root);

		return true;

//	  std::vector<Network> _networks;
//	  std::vector<Region> _regions;
//	  std::vector<Workplace> _workplaces;
//	  std::vector<University> _universities;
//	  std::vector<College> _colleges;
//	  std::vector<HighSchool> _highSchools;
//	  std::vector<JuniorSchool> _juniorSchools;
//	  std::vector<ElementarySchool> _elementarySchools;

	}

	static string convert2String(const SearchCacheDataPtr & data){
		Json::Value root;
		convert(data,root);
		return root.toStyledString();
	}
	static string buildDeleteJson(int uid){
		Json::Value root;
		root["id"] = uid;
		root["_delete"] = true;
		return root.toStyledString();
	}
static void convertNetwork( const vector<Network> & networks, Json::Value & root) {
	if(networks.empty()) { 
		root["networks"] = Json::arrayValue;
		return ;
	}
	Json::Value ja;
	BOOST_FOREACH(const Network & v , networks){
		Json::Value j;
		j["id"] = v.id; 
		j["stage"] = v.stage; 
		j["name"] = v.name; 
		ja.append(j);
	}
	root["networks"] = ja;
}

static void convertElementarySchool( const vector<ElementarySchool> & elementaryschools, Json::Value & root) {
	if(elementaryschools.empty()) { 
		root["elementarySchools"] = Json::arrayValue;
		return ;
	}

	Json::Value ja;
	BOOST_FOREACH(const ElementarySchool & v , elementaryschools){
		Json::Value j;
		j["id"] = v.id; 
		j["year"] = v.year; 
		j["name"] = v.name; 
		ja.append(j);
	}
	root["elementarySchools"] = ja;
}

static void convertJuniorSchool( const vector<JuniorSchool> & juniorschools, Json::Value & root) {
	if(juniorschools.empty()) { 
		root["juniorSchools"] = Json::arrayValue;
		return ;
	}
	Json::Value ja;
	BOOST_FOREACH(const JuniorSchool & v , juniorschools){
		Json::Value j;
		j["id"] = v.id; 
		j["year"] = v.year; 
		j["name"] = v.name; 
		ja.append(j);
	}
	root["juniorSchools"] = ja;
}

static void convertHighSchool( const vector<HighSchool> & highschools, Json::Value & root) {
	if(highschools.empty()) { 
		root["highSchools"] = Json::arrayValue;
		return ;
	}
	Json::Value ja;
	BOOST_FOREACH(const HighSchool & v , highschools){
		Json::Value j;
		j["id"] = v.id; 
		j["year"] = v.year; 
		j["name"] = v.name; 
		j["class1"] = v.class1; 
		j["class2"] = v.class2; 
		j["class3"] = v.class3; 
		ja.append(j);
	}
	root["highSchools"] = ja;
}

static void convertCollege( const vector<College> & colleges, Json::Value & root) {
	if(colleges.empty()) { 
		root["colleges"] = Json::arrayValue;
		return ;
	}
	Json::Value ja;
	BOOST_FOREACH(const College & v , colleges){
		Json::Value j;
		j["id"] = v.id; 
		j["year"] = v.year; 
		j["name"] = v.name; 
		j["department"] = v.department; 
		ja.append(j);
	}
	root["colleges"] = ja;
}

static void convertUniversity( const vector<University> & universitys, Json::Value & root) {
	if(universitys.empty()) { 
		root["universities"] = Json::arrayValue;
		return ;
	}
	Json::Value ja;
	BOOST_FOREACH(const University & v , universitys){
		Json::Value j;
		j["id"] = v.id; 
		j["year"] = v.year; 
		j["name"] = v.name; 
		j["department"] = v.department; 
		j["dorm"] = v.dorm; 
		ja.append(j);
	}
	root["universities"] = ja;
}

static void convertWorkplace( const vector<Workplace> & workplaces, Json::Value & root) {
	if(workplaces.empty()) { 
		root["workplaces"] = Json::arrayValue;
		return ;
	}
	Json::Value ja;
	BOOST_FOREACH(const Workplace & v , workplaces){
		Json::Value j;
		j["id"] = v.id; 
		j["name"] = v.name; 
		ja.append(j);
	}
	root["workplaces"] = ja;
}

static void convertRegion( const vector<Region> & regions, Json::Value & root) {
	if(regions.empty()) { 
		root["regions"] = Json::arrayValue;
		return ;
	}
	Json::Value ja;
	BOOST_FOREACH(const Region & v , regions){
		Json::Value j;
		j["id"] = v.id; 
		j["city"] = v.city; 
		j["province"] = v.province; 
		ja.append(j);
	}
	root["regions"] = ja;
}




};
/* python脚本
import sys
struct = ("Network",["id","stage","name"])
def convert(st):
	name = st[0].strip()	
	vs= name.lower() + "s"
	s = "static void convert" + name + "( const vector<" + name + "> & " + vs + ", Json::Value & root) {\n"
	s = s + "\tif(" + vs + ".empty()) { return ;}\n"
	s = s + "\tJson::Value j;\n" 
	s = s + "\tBOOST_FOREACH(const " + name +" & v , " + vs + "){\n"
	for f in st[1]:
		s = s + "\t\tj[\"" + f +"\"] = "  + "v." + f + "; \n"
	s = s + "\t}\n"
	s = s + "\troot[\"" + vs + "\"] = j;\n"
	s = s + "}\n"
	print s
def makeCall(st):
	name = st[0].strip()	
	ln = name[0:1].lower() + name[1:]
	s ="convert" + name + "(data->_" + ln + "s,root);\n" 
	print s
structs = []
def scan():
	f = open(sys.argv[1])
	one = []
	for line in f.readlines():
		line = line.strip()	
		if len(line) == 0:
			continue
		if "struct" in line:
			if(len(one) > 0):
				structs.append(one)
			tmp= line.replace("struct","")
			name = tmp.replace("{","")
			print "name:" + name
			one = (name,[]) 
		elif "string" in line or "int" in line:
			f = line.strip().split()[1]
			f = f.replace(";","")
			one[1].append(f)
	
	structs.append(one)
	#print structs
	for s in structs:
		convert(s)
	for s in structs:
		makeCall(s)

scan()
*/




};
};
#endif

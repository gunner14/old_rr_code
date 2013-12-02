#ifndef __DATA_H__
#define __DATA_H__

#include <vector>
#include "AdMob.h"
using namespace std;

class Uid_List {
public:
	Uid_List() {
		int uid_pass[] = {1,2,3,4,5,100,1000,10000,100000,1000000,123456789};
		int uid_fail[] = {-1,-2,-123450};
		pass.insert(uid_pass, uid_pass + sizeof(uid_pass)/sizeof(int));
		fail.insert(uid_fail, uid_fail + sizeof(uid_fail)/sizeof(int));
	}
public:
	set<int> pass;
	set<int> fail;
};

class Zone_List {
public:
	Zone_List() {
		Ice::Long zone_pass[] = {100000000098, 100000000099};
		Ice::Long zone_fail[] = {-100000000098, 100000000100};
		pass.insert(zone_pass, zone_pass + sizeof(zone_pass)/sizeof(Ice::Long));
		fail.insert(zone_fail, zone_fail + sizeof(zone_fail)/sizeof(Ice::Long));
	}
public:
	set<Ice::Long> pass;
	set<Ice::Long> fail;
};

class Net_List {
public:
	Net_List() {
		std::string net_pass[] = {"0100", "0200"};
		std::string net_fail[] = {"0101", "0211", "-0101", "0000", "ABCD", "错误"};
		pass.insert(net_pass, net_pass + 2);
		fail.insert(net_fail, net_fail + 6);
	}
public:
	set<std::string> pass;
	set<std::string> fail;
};

class IpArea_List {
public:
	IpArea_List() {
		std::string ipArea_pass[] = {"0011000000000000","0086000000000000","0086110000000000","0086310000000000"};
		std::string ipArea_fail[] = {"1186000000000000","-0086","9527","ABCD","0086ABCD","错误"};
		pass.insert(ipArea_pass, ipArea_pass + 4);
		fail.insert(ipArea_fail, ipArea_fail + 6);
	}
public:
	set<std::string> pass;
	set<std::string> fail;
};

class Screen_List {
public:
	Screen_List() {
		std::string screen_pass[] = {"320X480","450*800","480*800","640X960","220X300"};
		std::string screen_fail[] = {"*320","X320","ABXCD","df*320"};
		pass.insert(screen_pass, screen_pass + 5);
		fail.insert(screen_fail, screen_fail + 4);
	}
public:
	set<std::string> pass;
	set<std::string> fail;
};

class OS_List {
public:
	OS_List() {
		std::string os_pass[] = {"Android 1.5.1","Android 2.0.1","Android 2.2.1","Android 2.3.1","Android 2.8",
			                       "Android 4.5.1","iPhone OS3.1","iPhone OS4.0","iPhone OS4.1","iPhone OS4.3",
														 "iPhone OS5.0","iPhone OS3.5","iPhone OS3.a"};
		std::string os_fail[] = {"Android a.b.c","iPhone 1.5.1","Android OS5.1","Android 5","Android Y",
														 "华为 荣耀5","Android iPhone OS5.1","iPhone Android OS3.1","sldfk","你懂的OS"};
		pass.insert(os_pass, os_pass + 10);
		fail.insert(os_fail, os_fail + 10);
	}
public:
	set<std::string> pass;
	set<std::string> fail;
};

class AdGroupId_List {
public:
	AdGroupId_List() {
		Ice::Long group_pass[] = {10000000010001, 10000000010002,10000000010003,10000000010004,10000000010005,10000000010006};
		Ice::Long group_fail[] = {-1,0};
		pass.insert(group_pass, group_pass + sizeof(group_pass)/sizeof(Ice::Long));
		fail.insert(group_fail, group_fail + sizeof(group_fail)/sizeof(Ice::Long));
	}
public:
	set<Ice::Long> pass;
	set<Ice::Long> fail;
};

#endif


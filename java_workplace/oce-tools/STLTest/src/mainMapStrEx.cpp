#include <string>
#include <map>
#include <ext/hash_map>
#include <vector>
#include <iostream>
#include <sstream>
#include "UtilCxx/src/Evictor.h"

#define USE_HASHMAP
#define USE_ICEHANDLE

// using namespace std;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

const size_t oneK = 1000;
const size_t oneM = 1000000;
const string numStr = "1234567890";
const string letterStr = "abcdefghijklmnopqrstuvwxyz";
const string letterX5 = letterStr + letterStr + letterStr + letterStr + letterStr;
const string letterX35 = letterX5 + letterX5 + letterX5 + letterX5 + letterX5 + letterX5 + letterX5;
const int strSeqLen = 4;
const string strs[] = {numStr, letterStr, letterX5, letterX35};

struct RamResult {
	int ramA;
	int ramB;
	int ramC;
	int ramD;
	int ramE;
	int ram0C;
	int ram0E;
	int mapSize;
	int cellSize;
};

struct TestIceObject : virtual Ice::Object {
public:
	TestIceObject(int newid, const string& newstr):
		id(newid),
		str(newstr) {
	};

	void set(int newid, const string& newstr) {
		id = newid;
		str = newstr;
	};
private:
	int id;
	vector<int> ints;
	string str;
};
typedef IceUtil::Handle<TestIceObject> TestIceObjectPtr;

#ifdef USE_ICEHANDLE
	typedef TestIceObjectPtr ObjTest;
	ObjTest setObj(int newid, const string& newstr) {
		TestIceObjectPtr obj = new TestIceObject(newid, newstr);
		return obj;
	}
	string objInfo = "[IceHandle] the test use IceUtil::Handle";	

#else
	typedef string ObjTest;
	ObjTest setObj(int newid, const string& newstr) {
		return newstr;
	}
	string objInfo = "[string] the test use std::string";
#endif

#ifdef USE_HASHMAP
	typedef __gnu_cxx::hash_map<int, ObjTest> MapTest;
	typedef __gnu_cxx::hash_map<int, ObjTest>::iterator MapIterator;
	string mapInfo = "[HashMap] the test use ext/hash_map";
#else
	typedef std::map<int, ObjTest> MapTest;
	typedef std::map<int, ObjTest>::iterator MapIterator;
	string mapInfo = "[Map] the test use std::map";
#endif

// (A) create map
int processA(MapTest& mapT, const string& str, int size, int step) {
	// (A) CreateMap
	int ram;
	cout << "(step " << step << ": A) now create map" << endl;
	for ( size_t i = 0; i < size; ++i ) {
		std::ostringstream value;
	       	value << str << i;
		ObjTest obj = setObj(i, value.str());
		mapT.insert(std::make_pair<int, ObjTest>(i, obj));
	}
	cout << "map of " << mapT.size() << " objects created. check it's ram and input it.." << endl;
	cin >> ram; 
	return ram;
}

// (B) erase and insert half object
int processB(MapTest& mapT, const string& str, int size, int step) {
	cout << "(step " << step << " : B) now erase and insert half-object of map" << endl;
	size_t esSize = 0;
	int ram;
	string theObjHalf = str.substr(0, str.size()/2);
	for ( size_t i = 0; i < size; ++i ) {
		MapIterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			std::ostringstream value;
			value << theObjHalf << size-i;
			mapT.erase(pos);
			ObjTest obj = setObj(i, value.str());
			mapT.insert(std::make_pair<int, ObjTest>(i, obj));
			++esSize;
		}
	}
	cout << "map of " << mapT.size() << " objects erase&insert " << esSize << " half-object of them. check it's ram and input it.." << endl;
	cin >> ram;
        return ram;
}

// (C) erase all and insert half-object
int processC(MapTest& mapT, int& ram0, const string& str, int size, int step) {
	int ram;
	cout << "(step " << step << " : C) erase all of objects of map" << endl;
	size_t esSize = 0;
	for (int i = 0; i < size; ++ i ) {
		MapIterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			mapT.erase(pos);
			esSize++;
		}
	}
	cout << esSize << " objects has been erased, check it's ram and input it.." << endl;
	cin >> ram0;
	esSize = 0;
	cout << "insert all of half-object of map" << endl;
	string theObjHalf = str.substr(0, str.size()/2);
	for ( size_t i = 0; i < size; ++i ) {
		std::ostringstream value;
		value << theObjHalf << size-i;
		ObjTest obj = setObj(i, value.str());
		mapT.insert(std::make_pair<int, ObjTest>(i, obj));
		++esSize;		
	}
	cout << "map of " << mapT.size() << " objects inserted " << esSize << " of them. check it's ram and input it.." << endl;
	cin >> ram;
	return ram;
}

// (D) erase and insert half size
int processD(MapTest& mapT, const string& str, int size, int step) {
	cout << "(step " << step << " : D) now erase and insert objects of map" << endl;
	size_t esSize = 0;
	int ram;
	for ( size_t i = 0; i < size; ++i ) {
		MapIterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			std::ostringstream value;
			value << str << size-i;
			ObjTest obj = setObj(i, value.str());
			mapT.erase(pos);
			if ( i < size/2 ) {
				mapT.insert(std::make_pair<int, ObjTest>(i, obj));
			}
			++esSize;
		}
	}
	cout << "map of " << mapT.size() << " objects erase&insert " << esSize << " of them. check it's ram and input it.." << endl;
	cin >> ram;
        return ram;	
}

// (E) erase all and insert half size
int processE(MapTest& mapT, int& ram0, const string& str, int size, int step) {
	cout << "(step : " << step << " : E) erase all of objects of map" << endl;
	int esSize = 0, ram;
	for (int i = 0; i < size; ++ i ) {
		MapIterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			mapT.erase(pos);
			esSize++;
		}
	}
	cout << esSize << " objects has been erased, check it's ram and input it.." << endl;
	cin >> ram0;
	esSize = 0;
	cout << "insert all of objects of map" << endl;
	for ( size_t i = 0; i < size/2; ++i ) {
		std::ostringstream value;
		value << str << size-i;
		ObjTest obj = setObj(i, value.str());
		mapT.insert(std::make_pair<int, ObjTest>(i, obj));
		++esSize;		
	}
	cout << "map of " << mapT.size() << " objects inserted " << esSize << " of them. check it's ram and input it.." << endl;
	cin >> ram; 
	return ram;
}

void Input(string& str, int& size, int& cellSize) {

	cout << mapInfo << endl;
	cout << objInfo << endl;
	
	int choose = -1;
	// (1/7) Choose object
	cout << "(1/7) set params of test" << endl;
	while ( (choose<0) || (choose >= strSeqLen) ) {
		cout << "choose object : " << endl;
		for (size_t i = 0; i < strSeqLen; ++i ) {
			string info = (strs[i].size() <= 30)?(strs[i]):(strs[i].substr(0,10)+"..."+strs[i][strs[i].size()-1]); 
			cout << "\t" << i << " : " << info << " [size:" << strs[i].size() << "]\n";
		}
		cin >> choose;
	}
	str = strs[choose];
	cout << "choose num : what size of objects?" << endl;
	cin >> size;
	string info = (str.size() <= 30)?(str):(str.substr(0,10)+"..."+str[str.size()-1]); 
	cellSize = sizeof(int) + str.size() + 1;
	cout << "object-str : " << info << endl;
	cout << "size of a cell : " << cellSize << endl;

}

void Output(const RamResult& rams, const string& order) {

	map<char, string> outs;

	ostringstream a, b, c, d, e;
	a << "[A] ram map : " << rams.ramA << " k\n";
	b << "[B] ram e&i half-object : " << rams.ramB << " k\n"
  	  << "    ram increase : " << rams.ramB-rams.ramA << " kilo Byte(s)\n";
	c << "[C] ram erase all &insert half-object : " << rams.ramC << " k\n"
	  << "    ram increase : " << rams.ramC-rams.ramA << " kilo Byte(s) \n"
	  << "    ram after erase all : " << rams.ram0C << " k\n";
	d << "[D] ram e&i half-size : " << rams.ramD << " k\n"
	  << "    ram increase : " << rams.ramD-rams.ramA << " kilo Byte(s)\n";
	e << "[E] ram erase all &insert half size : " << rams.ramE << " k\n"
	  << "    ram increase : " << rams.ramE-rams.ramA << " kilo Byte(s)\n"
	  << "    ram after erase all : " << rams.ram0E << " k\n";
	outs['A'] = a.str();
	outs['B'] = b.str();
	outs['C'] = c.str();
	outs['D'] = d.str();
	outs['E'] = e.str();

	cout << "(7/7) result : " << endl;
	cout << "map size : " << rams.mapSize << endl;
	cout << "object size : " << rams.cellSize << endl;
	cout << "'ram in theory' : " << (rams.cellSize+sizeof(int))*rams.mapSize/1024 << "\n";
	for ( size_t i = 0; i < order.size(); ++i ) {
		cout << outs[order[i]];
	}
}

void TestGroup1(const string& str, int size, int cellSize) {

	MapTest mapT;
	RamResult rams;

	rams.ramA = processA(mapT, str, size, 2);
	rams.ramB = processB(mapT, str, size, 3);
	rams.ramC = processC(mapT, rams.ram0C, str, size, 4);
	rams.ramD = processD(mapT, str, size, 5);
	rams.ramE = processE(mapT, rams.ram0E, str, size, 6);
	rams.mapSize = size;
	rams.cellSize = cellSize;

	// 输出结论
	Output(rams, "ABCDE");
}

int main(int argv, char** argc) {

	string str;
	int size, cellSize;
	
	cout << "\nTest I. Test Map" << endl;

	// 输入参数
	Input(str, size, cellSize);

	// 测试Group1
	TestGroup1(str, size, cellSize);
}

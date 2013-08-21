#include <string>
#include <map>
#include <ext/hash_map>
#include <vector>
#include <iostream>
#include <sstream>
#include "UtilCxx/src/Evictor.h"

#define USE_HASHMAP

// using namespace std;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

const size_t oneK = 1000;
const size_t oneM = 1000000;

int main(int argv, char** argc) {

	// Map
	cout << "\nTest I. Test Map" << endl;
#ifndef USE_HASHMAP
	std::map<int, string> mapT;
	typedef std::map<int, string>::iterator MapIterator;
	cout << "[Map] the test use std::map" << endl;
#else
	__gnu_cxx::hash_map<int, string> mapT;
	typedef __gnu_cxx::hash_map<int, string>::iterator MapIterator;
	cout << "[HashMap] the test use ext/hash_map" << endl;
#endif

	// src
	vector<string> objs;
	objs.push_back("1234567890");
	objs.push_back("abcdefghijklmnopqrstuvwxyz");
	objs.push_back(objs[0]+objs[1]+objs[1]);
	objs.push_back(objs[2]+objs[2]);
	objs.push_back(objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]);
		
	// I -- Test Map
	int choose = -1, size, reverse = 0;
	int ram0, ramA, ramB, ramC, ram0C, ramD, ram0D;
	string obj;
	std::ostringstream sizeStr;
	// (1/7) Choose object
	cout << "(1/7) set params of test" << endl;
	while ( (choose<0) || (choose >= objs.size()) ) {
		cout << "choose object : " << endl;
		for (size_t i = 0; i < objs.size(); ++i ) {
			if ( objs[i].size() <= 30 ) {
				cout << "\t" << i << " : " << objs[i] << " [size:" << objs[i].size() << "]\n";
			} else {
				cout << "\t" << i << " : " << objs[i].substr(0, 10) << "..." 
					<< objs[i].at(objs[i].size()-1) << " [size:" << objs[i].size() << "]\n";
			}
		}
		cin >> choose;
	}
	cout << "choose num : what size of objects?" << endl;
	cin >> size;
	cout << "reverse num when erase&insert map?" << endl;
	cin >> reverse;
	obj = objs[choose];
	sizeStr << size;
	int cellSize = sizeof(int) + obj.size() + 1;
	if ( obj.size() <= 30 ) {
		cout << "object " << obj << endl;
	} else {
		cout << "object " << obj.substr(0, 10) << "..." << obj.substr(obj.size()-3, 3) << endl;
	}
	cout << "size of a cell : " << cellSize << endl;

	// (2/7) Get intitial ram
	cout << "(2/7) get initial ram of empty map" << endl;
	// cout << "check it's ram and input it .." << endl;
	// cin >> ram0;
	ram0 = 6868;

	// (3/7) CreateMap
	cout << "(3/7 | A) now create map" << endl;
	for ( size_t i = 0; i < size; ++i ) {
		std::ostringstream value;
	       	value << obj << i;
		mapT.insert(std::make_pair<int, string>(i, value.str()));
	}

	cout << "map of " << mapT.size() << " objects created. check it's ram and input it.." << endl;
	cin >> ramA; 

	// (4/7) erase and insert
	cout << "(4/7 | B) now erase and insert objects of map" << endl;
	size_t esSize = 0;
	for ( size_t i = 0; i < size; ++i ) {
		MapIterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			std::ostringstream value;
			if ( reverse > 0 ) {
				value << obj << size-i;
			} else {
				value << obj << i;
			}
			mapT.erase(pos);
			mapT.insert(std::make_pair<int, string>(i, value.str()));
			++esSize;
		}
	}
	cout << "map of " << mapT.size() << " objects erase&insert " << esSize << " of them. check it's ram and input it.." << endl;
	cin >> ramB; 

	// (5/7) erase all and insert
	cout << "(5/7 | C) erase all of objects of map" << endl;
	esSize = 0;
	for (int i = 0; i < size; ++ i ) {
		MapIterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			mapT.erase(pos);
			esSize++;
		}
	}

	cout << esSize << " objects has been erased, check it's ram and input it.." << endl;
	cin >> ram0C;
	esSize = 0;
	cout << "insert all of objects of map" << endl;
	for ( size_t i = 0; i < size; ++i ) {
		std::ostringstream value;
		if ( reverse > 0 ) {
			value << obj << size-i;
		} else {
			value << obj << i;
		}
		mapT.insert(std::make_pair<int, string>(i, value.str()));
		++esSize;		
	}
	cout << "map of " << mapT.size() << " objects inserted " << esSize << " of them. check it's ram and input it.." << endl;
	cin >> ramC; 

	// (6-1/7) erase all and insert other
	cout << "(6-1/7 | D) erase all of objects of map" << endl;
	esSize = 0;
	for (int i = 0; i < size; ++ i ) {
		MapIterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			mapT.erase(pos);
			esSize++;
		}
	}

	cout << esSize << " objects has been erased, check it's ram and input it.." << endl;
	cin >> ram0D;
	cout << "insert all of objects of map" << endl;
	esSize = 0;
	for ( size_t i = 0; i < size; ++i ) {
		std::ostringstream value;
		if ( reverse > 0 ) {
			value << obj.substr(0,obj.size()-1) << "_" << size-i;
		} else {
			value << obj.substr(0,obj.size()-1) << "_" << i;
		}
		mapT.insert(std::make_pair<int, string>(i+size, value.str()));
		++esSize;		
	}
	cout << "map of " << mapT.size() << " objects inserted " << esSize << " of them. check it's ram and input it.." << endl;
	cin >> ramD; 
	
	// (6-2/7) erase all and insert other	
	cout << "(6-2/7 | E) repeat 6-1 for 5 times" << endl;
	int ramE[5];
	string abc = "#$%@!";
	for ( size_t times = 0; times < 5; ++times ) {
		esSize = 0;
		for (int i = 0; i < 5*size; ++ i ) {
			MapIterator pos = mapT.find(i);
			if ( pos != mapT.end() ) {
				mapT.erase(pos);
				esSize++;
			}
		}
		cout << " erased " << esSize << " objects" << endl;
		esSize = 0;
		int mapSize = size;
		string theObj = obj.substr(0, obj.size()-1);
		if ( times == 2 ) {
			// 第3次, object尺寸/2
			int thelen = obj.size()>2?obj.size()/2:2;
			theObj = obj.substr(0, thelen-1);
		} else if ( times == 3 ) {
			mapSize = size/2;
		} else if ( times == 4 ) {
			// 第5次, object所有尺寸x2
			theObj = obj + obj;
		}
		for ( size_t i = 0; i < mapSize; ++i ) {
			std::ostringstream value;
			if ( reverse > 0 ) {
				value << theObj << abc[times] << size-i;
			} else {
				value << theObj << abc[times] << i;
			}
			mapT.insert(std::make_pair<int, string>(i+size*times, value.str()));
			++esSize;		
		}
		cout << "[repeat " << times << "] map of " << mapT.size() << " objects inserted " << esSize << " of them. check it's ram and input it.." << endl;
		cin >> ramE[times]; 
	}


	// (7/7)result
	cout << "(7/7) result : " << endl;
	cout << "map size : " << mapT.size() << endl;
	cout << "object size : " << cellSize << endl;
	cout << "erase&insert : " << esSize << endl;
	cout << "reverse : " << reverse << endl;
	cout << "'ram in theory' : " << (cellSize+1)*mapT.size()/1024 << "-" 
		<< (cellSize+sizeStr.str().size())*mapT.size()/1024 << " k" << endl;
	cout << "ram of empty map " << ram0 << " k" << endl;
	cout << "A. ram before e&i " << ramA << " k" << endl;
	cout << "B. ram after e&i " << ramB << " k" << endl;
	cout << "[B ram increase] " << ramB-ramA << " kilo Byte(s)" << endl;
	cout << "C. ram of erase all " << ram0C << " k" << endl;
	cout << "C. ram of erase&insert same " << ramC << " k" << endl;
	cout << "[C ram increase] " << ramC-ramA << " kilo Byte(s)" << endl;
	cout << "D. ram of erase&insert other " << ramD << " k" << endl;
	cout << "[D ram increase] " << ramD-ramA << " kilo Byte(s)" << endl;	
	// Ex5
	string msg[] = {"", "", "(object尺寸减半)", "(size/2)", "(object全体x2)"};
	for ( size_t times = 0; times < 5; ++times ) {
		cout << "E. " << times << msg[times] << " ram : " << ramE[times] << " k" << endl;
		cout << "[E " << times << " ram increase ] " << ramE[times]-ramA << " kilo Byte(s)" << endl;
	}

}

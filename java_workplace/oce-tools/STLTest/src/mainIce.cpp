#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <Ice/Ice.h>

using namespace std;

const size_t oneK = 1000;
const size_t oneM = 1000000;

int main(int argv, char** argc) {

	// src
	vector<string> objs;
	objs.push_back("1234567890");
	objs.push_back("abcdefghijklmnopqrstuvwxyz");
	objs.push_back(objs[0]+objs[1]+objs[1]);
	objs.push_back(objs[2]+objs[2]);
	objs.push_back(objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]+objs[3]);
		
	// I -- Test Map
	cout << "I. Test Map" << endl;
	map<int, string> mapT;
	int choose = -1, size, reverse = 0;
	int ram0, ramA, ramB, ramC, ram0C, ramD, ram0D;
	string obj;
	ostringstream sizeStr;
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
	cout << "check it's ram and input it .." << endl;
	cin >> ram0;

	// (3/7) CreateMap
	cout << "(3/7) now create map" << endl;
	for ( size_t i = 0; i < size; ++i ) {
		ostringstream value;
	       	value << obj << i;
		mapT.insert(make_pair<int, string>(i, value.str()));
	}

	cout << "map of " << mapT.size() << " objects created. check it's ram and input it.." << endl;
	cout << "'size in theory' is " << (cellSize+1)*mapT.size()/1024 << "-" 
	       << (cellSize+sizeStr.str().size())/1024 << " k" << endl;
	cin >> ramA; 

	// (4/7) erase and insert
	cout << "(4/7) now create and insert objects of map" << endl;
	size_t esSize = 0;
	for ( size_t i = 0; i < size; ++i ) {
		map<int, string>::iterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			ostringstream value;
			if ( reverse > 0 ) {
				value << obj << size-i;
			} else {
				value << obj << i;
			}
			mapT.erase(pos);
			mapT.insert(make_pair<int, string>(i, value.str()));
			++esSize;
		}
	}
	cout << "map of " << mapT.size() << " objects erase&insert " << esSize << " of them. check it's ram and input it.." << endl;
	cout << "'size in theory' is " << (cellSize+1)*mapT.size()/1024 
		<< "-" << (cellSize+sizeStr.str().size())*mapT.size()/1024 << " k" << endl;
	cin >> ramB; 

	// (5/7) erase all and insert
	cout << "(5/7) erase all of objects of map" << endl;
	esSize = 0;
	for ( size_t i = 0; i < size; ++i ) {
		map<int, string>::iterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			mapT.erase(pos);
			++esSize;
		}
	}
	cout << esSize << " objects has been erased, check it's ram and input it.." << endl;
	cin >> ram0C;
	esSize = 0;
	cout << "insert all of objects of map" << endl;
	for ( size_t i = 0; i < size; ++i ) {
		ostringstream value;
		if ( reverse > 0 ) {
			value << obj << size-i;
		} else {
			value << obj << i;
		}
		mapT.insert(make_pair<int, string>(i, value.str()));
		++esSize;		
	}
	cout << "map of " << mapT.size() << " objects inserted " << esSize << " of them. check it's ram and input it.." << endl;
	cout << "'size in theory' is " << (cellSize+1)*mapT.size()/1024 
		<< "-" << (cellSize+sizeStr.str().size())*mapT.size()/1024 << " k" << endl;
	cin >> ramC; 

	// (6/7) erase all and insert other
	cout << "(6/7) erase all of objects of map" << endl;
	esSize = 0;
	for ( size_t i = 0; i < size; ++i ) {
		map<int, string>::iterator pos = mapT.find(i);
		if ( pos != mapT.end() ) {
			mapT.erase(pos);
			esSize++;
		}
	}
	cout << esSize << " objects has been erased, check it's ram and input it.." << endl;
	cin >> ram0D;
	cout << "insert all of objects of map" << endl;
	for ( size_t i = 0; i < size; ++i ) {
		ostringstream value;
		if ( reverse > 0 ) {
			value << obj.substr(0,obj.size()-1) << "_" << size-i;
		} else {
			value << obj.substr(0,obj.size()-1) << "_" << i;
		}
		mapT.insert(make_pair<int, string>(i+size, value.str()));
		++esSize;		
	}
	cout << "map of " << mapT.size() << " objects inserted " << esSize << " of them. check it's ram and input it.." << endl;
	cout << "'size in theory' is " << (cellSize+1)*mapT.size()/1024 
		<< "-" << (cellSize+sizeStr.str().size())*mapT.size()/1024 << " k" << endl;
	cin >> ramD; 

	// (7/7)result
	cout << "(7/7) result : " << endl;
	cout << "map size : " << mapT.size() << endl;
	cout << "object size : " << cellSize << endl;
	cout << "erase&insert : " << esSize << endl;
	cout << "reverse : " << reverse << endl;
	cout << "'ram in theory' : " << (cellSize+1)*mapT.size()/1024 << "-" 
		<< (cellSize+sizeStr.str().size())*mapT.size()/1024 << " k" << endl;
	cout << "ram of empty map " << ram0 << " k" << endl;
	cout << "ram before e&i " << ramA << " k" << endl;
	cout << "ram after e&i " << ramB << " k" << endl;
	cout << "[ram increase B] " << ramB-ramA << " kilo Byte(s)" << endl;
	cout << "ram of erase all] " << ram0C << " k" << endl;
	cout << "ram of insert all] " << ramC << " k" << endl;
	cout << "[ram increase C] " << ramC-ramA << " kilo Byte(s)" << endl;
	cout << "ram of insert all] " << ramD << " k" << endl;
	cout << "[ram increase D] " << ramD-ramA << " kilo Byte(s)" << endl;

}

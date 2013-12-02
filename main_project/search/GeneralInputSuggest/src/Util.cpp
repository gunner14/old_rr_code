/*
 * Util.cpp
 *
 *  Created on: 2012-8-24
 *      Author: cm
 */

#include "Util.h"

Util::Util() {
	// TODO Auto-generated constructor stub
}

Util::~Util() {
	// TODO Auto-generated destructor stub
}

 vector<string> Util::tokenize(string& text, string seq) {//按空格切分，多个空格也可以
    int last_pos = 0;
    string::size_type  pos = 0;
    vector<string> splits;
    while(true) {
      pos = text.find_first_of(seq,last_pos);
      if(pos == string::npos) {
        splits.push_back(text.substr(last_pos, pos-last_pos));
        break;
      } else {
        splits.push_back(text.substr(last_pos, pos-last_pos));
        last_pos = pos+1;
        while(text.at(last_pos) == ' ') {
          cout<<"NULL"<<endl;
          last_pos++;
        }
      }
    }
    return splits;
}

string& Util::trim(string& value) {//去除字符首尾的空格
  if(value.empty()) {
    return value;
  }
  value.erase(0,value.find_first_not_of(" "));
  value.erase(value.find_last_not_of(" ")+1);
  return value;
}

void Util::union_vec(std::vector<int>&A, const std::vector<int>& B) {
	vector<int> C;
	vector<int>::iterator itA = A.begin();
	vector<int>::const_iterator itB = B.begin();
	while(itA != A.end()&&itB != B.end()){
		if((*itA) == (*itB)){
			C.push_back(*itA);
			++itA;
			++itB;
		}else if((*itA) < (*itB)){
			C.push_back(*itA);
			++itA;
		}else{
			C.push_back(*itB);
			++itB;
		}
	}
	while(itA != A.end()){
		C.push_back(*itA);
		++itA;
	}
	while(itB != B.end()){
		C.push_back(*itB);
		++itB;
	}
	A.swap(C);
}

bool Util::startsWith(const std::string& base, const std::string& pre) {
	if (base.size() < pre.size()) {
		return false;
	}
	int pc = pre.size();
	int po = 0;
	while (--pc >= 0) {
		if (base[po] != pre[po++] ) {
			return false;
		}
	}
	return true;
}









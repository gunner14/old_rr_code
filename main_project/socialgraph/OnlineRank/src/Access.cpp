#include "Access.h"

Access::Access(const char* logfile){
	_load(logfile);
}

void Access::_load(const char* logfile){
	ifstream ifs(logfile);
	string line="";
	while(getline(ifs, line)){
		UserLog ul;
		size_t pos=line.find_first_of("\t");
		if(pos==string::npos)continue;
		ul.uid=atoi(line.substr(0, pos).c_str());
		while(1){
			size_t pos1=line.find_first_of(",", pos+1);
			if(pos1==string::npos)break;
			size_t subject=atoi(line.substr(pos+1, pos1-pos).c_str());
			size_t pos2=line.find_first_of(";", pos1+1);
			if(pos2==string::npos)break;
			int type = GetAccessType( line.substr(pos1+1, pos2-pos1-1).c_str() ) ;
			if(type!=-1)ul.userAccess.insert(pair<int, int>(subject, type));
			pos=pos2;
		}
		this->push_back(ul);
	}
	ifs.close();
}


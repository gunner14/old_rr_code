/*
 * PreferStat.h
 *
 *  Created on: Dec 15, 2011
 *      Author: rong.tang
 */

//./Prefer ../data/dispatch/dispatch.2011-12-15.v2 ../data/clickreply/clickreply.2011-12-15 ../data/score/score.2011-12-15

#include "PreferStat.h"
using namespace xce::feed;

int main(int argc, char ** argv) { 
	if(argc != 4){
		cout << "err argc:" << argc<< endl
				<< "should be like:   ./Prefer dispatch clickreply result";
	}

	string dispatch_file = string(argv[1]);
	dispatch_file += ".v2";
	cout << dispatch_file << endl;

  PreferStat::instance().ProcessDispatch(dispatch_file);
	PreferStat::instance().ProcessClickReply(string(argv[2]),string(argv[3]));
	
	PreferStat::instance().Process(string(argv[3]));
	
  return 0;
}





#include <string.h>

#include "test_love.h"
#include "test_env.h"
#include "test_iconv.h"
#include "test_inet.h"
#include "test_map.h"
#include "test_vector.h"
#include "test_time.h"
#include "test_system.h"
//#include "test_thread.h"
//#include "test_readfile.h"

using namespace std;

void help() {
        cout << "Usage: go [OPTIONS]" << endl;
        cout << "-h, --help\t\t\tDisplay this help and exit." << endl;
        cout << "-testlove, --testlove\t\tDisplay test_love()." << endl;
        cout << "-testenv, --testenv\t\tDisplay test_env()." << endl;
        cout << "-testiconv, --testiconv\t\tDisplay test_iconv()." << endl;
        cout << "-testinet, --testinet\t\tDisplay test_inet()." << endl;
        cout << "-testmap, --testmap\t\tDisplay test_map()." << endl;
        cout << "-testvector, --testvector\tDisplay test_vector()." << endl;
        cout << "-testtime, --testtime\t\tDisplay test_time()." << endl;
        cout << "-testsystem, --testsystem\t\tDisplay test_system()." << endl;
        cout << "-testthread, --testthread\tDisplay test_thread()." << endl;
        cout << "-testreadfile, --testreadfile\tDisplay test_readfile(). Need file to open. ( \"/home/yexiang841/CxxTester/test_readfile.file\" etc. )" << endl;
}

int main (int argc, char * const argv[]) {

	if (argc<2) {
		help();
		return 1;
	}

	bool testlove	= false;
	bool testenv	= false;
	bool testiconv	= false;
	bool testinet	= false;
	bool testmap	= false;
	bool testvector	= false;
	bool testtime	= false;
	bool testsystem	= false;
	bool testthread	= false;
	bool testreadfile = false;
	string testreadfile_file = "";

#define OPT(_a1, _a2)   else if ( !strcmp(argv[i], _a1) || !strcmp(argv[i], _a2) )
	int i;
	for (i=1; i<argc; i++) {
		if ( i == 0);
		OPT( "-h", "--help" )			{ help(); return 0; }
		OPT( "-testlove", "--testlove" )	{ testlove	= true; }
		OPT( "-testenv", "--testenv" )		{ testenv	= true; }
		OPT( "-testiconv", "--testiconv" )	{ testiconv	= true; }
		OPT( "-testinet", "--testinet" )	{ testinet	= true; }
		OPT( "-testmap", "--testmap" )		{ testmap	= true; }
		OPT( "-testvector", "--testvector" )	{ testvector	= true; }
		OPT( "-testtime", "--testtime" )	{ testtime	= true; }
		OPT( "-testsystem", "--testsystem" )	{ testsystem	= true; }
		OPT( "-testthread", "--testthread" )	{ testthread	= true; }
		else if (i + 1 == argc) break;
		OPT("-testreadfile","--testreadfile") 	{ testreadfile = true; testreadfile_file = argv[++i]; }
		else break;
	}
#undef OPT

	if (i!=argc) {
		cout << "ERROR: malformed or unknown option near '" << argv[i] << "'." << endl;
		help();
		return 1;
	}

	if ( testreadfile && testreadfile_file=="" ) {
		cout << "Need file to open." << endl;
		return 1;
	}

	cout << "[main] starting tests." << endl;
	if( testlove   ) test_love();
	if( testenv    ) test_env();
	if( testiconv  ) test_iconv();
	if( testinet   ) test_inet();
	if( testmap    ) test_map();
	if( testvector ) test_vector();
	if( testtime   ) test_time();
	if( testsystem   ) test_system();
	//if( testthread ) test_thread();
	//if( testreadfile ) testreadfile(testreadfile_file);
	cout << "[main] finished tests." << endl;
	return 0;
}


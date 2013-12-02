#include <ScoreCacheAdapter.h>

using namespace std;
using namespace MyUtil;
using namespace xce::scorecache::adapter;

struct LoginNode{
	int id;
	int h;
	int c;
	long time;
	int type;
};

static void split(std::string src, std::string token, vector<std::string>& vect)    
{    
    int nend=0;    
    int nbegin=0;    
    while(nend != -1)    
    {    
        nend = src.find_first_of(token, nbegin);    
        if(nend == -1){
            vect.push_back(src.substr(nbegin, src.length()-nbegin));    
		cout << "testing " << nend-nbegin;
	} else{
            vect.push_back(src.substr(nbegin, nend-nbegin));    
		cout << "testing " << nend-nbegin;
	}
        nbegin = nend + 1;    
    }    
}

vector<LoginNode> test_readfile(const char* filename)
{
        cout << "opening file" << endl;
        vector<LoginNode> nodelist;
        ifstream ifs(filename);
        if( !ifs.is_open())
        {
                cout << "Error opening file" << endl;
                return nodelist;
        }
        string buffer;
        while (getline(ifs,buffer)){
                try{
                        string line=boost::lexical_cast<string>(buffer);
			vector<string> names;
			split(line,"|",names); 
			if( names.size() != 5 ){
				cout << " what?? " << line << endl;
			}
			LoginNode tmp;
			tmp.id=boost::lexical_cast<int>(line[0]);
			tmp.h=boost::lexical_cast<int>(line[1]);
			tmp.c=boost::lexical_cast<int>(line[2]);
			tmp.time=boost::lexical_cast<long>(line[3]);
			tmp.type=boost::lexical_cast<int>(line[4]);
			nodelist.push_back(tmp);
                        cout << "read user: " << tmp.id << " " << tmp.h << " " << tmp.c << " " << tmp.time << " " << tmp.type << endl;
                        nodelist.push_back(tmp);
                }catch(...){
                        cout << "skipped bad ID: " << buffer << endl;
                }
        }
        ifs.close();
        return nodelist;
}

int main(int argc, char* argv[]) {
	//ScoreLoaderAdapter::instance().setLoginDays(it->id,it->h,it->c,it->time,it->type);
	cout << ScoreCacheNAdapter::instance().getScoreDataN(24496255).score << endl;
	ScoreLoaderNAdapter::instance().incScore(24496255,100000);
/*
	vector<LoginNode> nodes = test_readfile("reload.ids");
	MyUtil::IntSeq tmp;
	for( MyUtil::IntSeq::iterator it = userIds.begin(); it != userIds.end(); ++it ){
		tmp.push_back( *it );
		if( tmp.size() >= 1000 ){
			cout << "reloadBatch size=" << tmp.size() << endl;
			ScoreLoaderAdapter::instance().reloadBatch(tmp);
			cout << "reloadBatch done" << endl;
			tmp = (MyUtil::IntSeq());
		}
	}
	if( tmp.size() >= 1 ){
		cout << "reloadBatch size=" << tmp.size() << endl;
		ScoreLoaderAdapter::instance().reloadBatch(tmp);
		cout << "reloadBatch done" << endl;
		tmp = (MyUtil::IntSeq());
	}
	return 0;
*/
/*
	vector<LoginNode> nodes = test_readfile("reload.ids");
	for( vector<LoginNode>::iterator it = nodes.begin(); it != nodes.end(); ++it ){
		cout << "setLoginDays user: " << it->id << " " << it->h << " " << it->c << " " << it->time << " " << it->type << endl;
		//ScoreLoaderAdapter::instance().setLoginDays(it->id,it->h,it->c,it->time,it->type);
	}
	return 0;
*/
/*
	long time = 1273939318231;
	ScoreLoaderAdapter::instance().setLoginDays(243532523,88,73,time,4);
*/
}


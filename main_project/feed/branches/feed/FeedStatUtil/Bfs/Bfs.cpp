#include "iostream"
#include "fstream"
#include <boost/foreach.hpp>
#include <vector>
#include <set>
#include <queue>
#include <ext/hash_map>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace __gnu_cxx;
using namespace boost;
class Node{
public:
  Node():distance_(1000){}
	void Dump(ofstream & ofs){
		ofs << uid_ << "\t" << distance_ << endl;
	}
  int uid_;
	vector<int> friends_;
	int distance_;
};
typedef hash_map<int,Node*> NodeMap;

class Computor{
public:
	Computor(const string & relation_file,const string & output_file):
    relation_file_(relation_file),output_file_(output_file){ }
	void Load(){
		ifstream ifs;
		ifs.open(relation_file_.c_str(),ifstream::in);
		string line;
    string tmp;
    vector<std::string> strs, friends;
    int scan_lines = 0;
		while (getline(ifs, line)) {
      split(strs, line, boost::is_any_of("\t,"));
      if(strs.size() < 3)
        continue;
      int uid = lexical_cast<int> (strs[0]);
      Node * node = new Node;
      node->uid_ = uid;
      //split(friends, strs[2], boost::is_any_of(","));
      node->friends_.reserve(strs.size() - 2);
      int cnt = strs.size();
      for(int i = 2; i < cnt ; ++i){
        string & f = strs[i]; 

        if(f.empty())
          continue;
        node->friends_.push_back(lexical_cast<int>(f)); 
      }
      node_map_[uid] = node;
      strs.clear();
      if(scan_lines % 10000== 0){
        cout << "scaned scan_lines:" << scan_lines << " map size:" << node_map_.size() << " q:" << q_.size()  << endl;
      }
      ++scan_lines;
		}
    cout << "Load done,map size:" << node_map_.size() << " q:" << q_.size() << endl;
	}
	void Run(int root){
    NodeMap::iterator it = node_map_.find(root);
    if(it == node_map_.end()){
      cout << "No root:" << root << endl;
      return; 
    }
    it->second->distance_ = 0;
    q_.push(it->second);

    ofstream ofs;
    ostringstream os;
    os << output_file_ << "." << root;
		ofs.open(os.str().c_str(),ifstream::out);
		while(!q_.empty()){
			Node * node = q_.front();
			q_.pop();
			int distance_f = node->distance_+1;
			BOOST_FOREACH(int f,node->friends_){
				hash_map<int,Node*>::iterator it = node_map_.find(f)	;
				if(it != node_map_.end()){
					Node * f_node = it->second;
          if(f_node->distance_ != 1000)
            continue;
					f_node->distance_ = distance_f;
					q_.push(f_node);
					//node_map_.erase(it);
				}
			}
			node->Dump(ofs);
		}
    //return;
    typedef hash_map<int,Node *>::value_type vt;
    BOOST_FOREACH(vt & p,node_map_){
      Node * node = p.second;
      if (node->distance_ == 1000 )
        node->Dump(ofs);
      else
        node->distance_ = 1000;
    }
	}
	hash_map<int,Node *> node_map_;
	queue<Node*> q_;
  int root_uid_;

	string relation_file_;
	string output_file_;
};

void PrintUsage(){
	cout << "./bfs requires parameters:\n"
		<< "-r relation_file "
		<< "-o output_file "
		<< "-u uid_root"
		<< endl;
	exit(-1);
}
set<int> GetRoots(const string & roots_file){
  set<int> roots;
  ifstream ifs;
  ifs.open(roots_file.c_str(),ifstream::in);
  string line;
  while (getline(ifs, line)) {
    int uid = lexical_cast<int>(line);
    cout << "GetRoots:" << uid << endl;
    roots.insert(uid); 
  }
  return roots;
}
int main(int argc, char **argv){
	string relation_file;
	string output_file;
	string roots_file;
	//while ((c = getopt(argc, argv, "b:e:s:o:x:g:m:t:i:r:l:u:p:y:v:n:")) >= 0) {
  char c = 0 ;
	while ((c = getopt(argc, argv, "r:o:u:")) >= 0) {
		switch (c) {
			case 'r':
				relation_file= optarg;
				break;
			case 'o':
				output_file= optarg;//distance file
				break;
			case 'u':
				roots_file= optarg; 
				break;
			default:
				PrintUsage();
				break;
		}
	}
  if(relation_file.empty() or output_file.empty())
    PrintUsage();
  Computor cp(relation_file,output_file);
  try{
    cp.Load();
  } catch (std::exception& e) {
    cerr << "error,line:" << __LINE__  << " "<<e.what()<< endl;;
  }
  set<int> roots = GetRoots(roots_file);
  cout << "roots:" << roots.size() << endl;
  BOOST_FOREACH(int root,roots){
    cout << "run root:" << root << endl;
    cp.Run(root);
  }
}

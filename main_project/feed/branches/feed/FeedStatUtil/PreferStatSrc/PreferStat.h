/*
 * PreferStat.h
 *
 *  Created on: Dec 15, 2011
 *      Author: rong.tang
 */

#ifndef PreferStat_H_
#define PreferStat_H_
#include <map>
#include <set>
#include <iostream>
#include <cmath>
#include <fstream>
#include <boost/foreach.hpp>
#include <ext/hash_map>
#include "Singleton.h"
#include <vector>
#include <IceUtil/Handle.h>
#include "BuddyByIdReplicaAdapter.h"

using namespace std;
using  __gnu_cxx::hash_map;
using namespace xce::buddy::adapter;

namespace xce {
namespace feed {

#define DoLog(msg) std::cout << msg << std::endl;;
//#define DoLog(msg) ;
static const int VECSIZE = 10000; 

class TimeStat {
  timeval _begin, _end;
  bool _log;
public:
  TimeStat(bool log = false) {
    _log = log;
    reset();
  }
  void reset() {
    gettimeofday(&_begin,NULL);
  }
  float getTime(bool clear = false) {
    gettimeofday(&_end, NULL);
    float timeuse = 1000000 * (_end.tv_sec - _begin.tv_sec) + _end.tv_usec
        - _begin.tv_usec;
    timeuse /= 1000;
    return timeuse;
  }
};


struct ClickReplyReceive {
public:
	int click,reply,receive;
}data;


class PreferStat: public MyUtil::Singleton<PreferStat>{
public:

  inline void AddReceive(hash_map<int, ClickReplyReceive> & map, int id, int count) {
	  hash_map<int, ClickReplyReceive>::iterator it = map.find(id);
	  if (it != map.end()) {
		  (it->second).receive += count;
	  } else {
		  data.click = 0;
		  data.reply = 0;
		  data.receive = count;
		  map[id] = data;
	  }
  }

  /*
      void ProcessDispatch(const string & dispatchlog){  //load dispatch file
		TimeStat ts;
		ifstream infile;
		infile.open(dispatchlog.c_str(), ifstream::in);
		if (!infile) {
			DoLog("PreferStat: ProcessDispatch open file fail");
			exit(0);
		}
		Clear();

		int to, stype, count;
		int cnt = 0, old_id = -1;
		vector<pair<int, int> > vecStypeCnt;
		string line;
		while (getline(infile, line)) {
			++cnt;
			if (cnt%10000000 == 0) DoLog("line: " << cnt);
			sscanf(line.c_str(), "%d\t%d\t%d", &to, &stype, &count);

			AddReceive(user_map_, to, count); // store for useraverage
			if(stype>10000) {
				DoLog("dispatch wrong:"<<stype);
			} else {
				AddReceive(stype_map_, stype, count);// store for stypeaverage
			}

			if (to != old_id) {
				if (old_id != -1) {
					from_map_[(long) old_id] = vecStypeCnt;//user:stypes that he create
				}
				vecStypeCnt.clear();
				old_id = to;
			}
			vecStypeCnt.push_back(make_pair(stype, count));
		}
		from_map_[(long) old_id] = vecStypeCnt;

		infile.close();
		DoLog("PreferStat::ProcessDispatch. from_ size:" << from_map_.size() << " linecnt:" << cnt
				<< "  user_map_ size:"<< user_map_.size()<<"  stype_map_ size:"<<stype_map_.size());
		DoLog("ProcessDispatch cost: " << ts.getTime());

	}
   * */

  void ProcessDispatch(const string & dispatchlog){  //load dispatch file
		TimeStat ts;
		ifstream infile;
		infile.open(dispatchlog.c_str(), ifstream::in);
		if (!infile) {
			DoLog("PreferStat: ProcessDispatch open file fail");
			exit(0);
		}
		Clear();

		int to, stype, count_before,count_after;
		int cnt = 0, old_id = -1;
		vector<pair<int, int> > vecStypeCnt;
		string line;
		while (getline(infile, line)) {
			++cnt;
			if (cnt%10000000 == 0) DoLog("line: " << cnt);
			sscanf(line.c_str(), "%d\t%d\t%d", &to, &stype, &count_before,&count_after);

			AddReceive(user_map_, to, count_after); // store for useraverage
			if(stype>10000) {
				DoLog("dispatch wrong:"<<stype);
			} else {
				AddReceive(stype_map_, stype, count_after);// store for stypeaverage
			}

			if (to != old_id) {
				if (old_id != -1) {
					from_map_[(long) old_id] = vecStypeCnt;//user:stypes that he create
				}
				vecStypeCnt.clear();
				old_id = to;
			}
			vecStypeCnt.push_back(make_pair(stype, count_before));
		}
		from_map_[(long) old_id] = vecStypeCnt;

		infile.close();
		DoLog("PreferStat::ProcessDispatch. from_ size:" << from_map_.size() << " linecnt:" << cnt
				<< "  user_map_ size:"<< user_map_.size()<<"  stype_map_ size:"<<stype_map_.size());
		DoLog("ProcessDispatch cost: " << ts.getTime());

	}


  //=====================

	void WriteFile4Stype(vector<string>& temp, map<int, int> & mapStypeCnt, ofstream& outfile) { //stype
		int to, featureid, click, reply, receive;
		float score = 0;
		for (vector<string>::iterator temp_it = temp.begin(); temp_it != temp.end(); ++temp_it) {
			receive = 0;
			sscanf(temp_it->c_str(), "%d\t%d\t%d\t%d", &to, &featureid, &click,	&reply);
			map<int, int>::iterator map_it = mapStypeCnt.find((-featureid));
			if (map_it != mapStypeCnt.end()) {
				receive = map_it->second;
			}

			if (IsTest(to)) {//new formula
				score = ScoreTen(click, reply, receive);
			} else {
				score = ScoreTwo(click, reply, receive);
			}
			outfile << to << '\t' << featureid << '\t' <<  score << '\n';
//			outfile << to << '\t' << featureid << '\t' << click << '\t' << reply << '\t' << receive << '\t' << score << '\n';
			// ++line_out_;
		}

	}

	 inline void AddClickReply(hash_map<int, ClickReplyReceive> & map, int key, int click, int reply) {
		  hash_map<int, ClickReplyReceive>::iterator it = map.find(key);
		  if (it != map.end()) {
			  (it->second).click += click;
			  (it->second).reply += reply;
		  } else {
			  data.click = click;
			  data.reply = reply;
			  data.receive = 0;
			  map[key] = data;
		  }
	  }

  void ProcessClickReply(const string & clickreply, const string & scorefile) {
		TimeStat ts;
		ifstream infile;
		infile.open(clickreply.c_str(), ifstream::in);
		ofstream outfile;
		outfile.open(scorefile.c_str(), ofstream::out);

		if (!infile) {
			DoLog("PreferStat: ProcessClic click_file fial 1: did not exist?");
			exit(0);
//			return;
		}
		if (!outfile.is_open()) {
			DoLog("PreferStat: ProcessClic score_file fail 2: could not open " << scorefile);
			exit(0);
//			return;
		}

		string line;
		int to, featureid, click, reply, receive;
		int old_id = -1, cnt = 0;
		float score;
		vector<string> temp;
		map<int, int> mapStypeCnt;
		//line_in_ = line_out_ = delta = 0;
		while (getline(infile, line)) {
			//++line_in_;
			++cnt;
			if (cnt % 10000000 == 0)  DoLog("line:" << cnt);

			sscanf(line.c_str(), "%d\t%d\t%d\t%d", &to, &featureid, &click,	&reply);// to 对 featureid 的新鲜事点击回复
			if (to == -1) {
				DoLog("evial input!!!\nline:"<<cnt << "    data-->"<<line);
				continue;
			}

			if (featureid < 0) {
				if(-featureid>10000) {
					DoLog("ClickRepy  data stype wrong:"<<featureid << "   escape line:"<<line);
				} else  {
					AddClickReply(stype_map_, -featureid, click, reply);
				}
			} else {
				AddClickReply(user_map_, featureid, click, reply);
			}

			// --------- calculate  ------------------
			if (old_id != to) {//new person!!
				//process older person
				if (old_id != -1 && !temp.empty()) {
					WriteFile4Stype(temp, mapStypeCnt, outfile);
				}
				old_id = to;
				mapStypeCnt.clear();
				temp.clear();
			}

			{
				//same person or first show
				if (featureid < 0) {//stype
					temp.push_back(line);
				} else {//friends
					receive = 0;
					hash_map<long, vector<pair<int, int> > >::iterator hi = from_map_.find((long) featureid);
					if (hi != from_map_.end()) {
						vector<pair<int, int> > res = hi->second;
						for (vector<pair<int, int> >::iterator res_it =
									res.begin(); res_it != res.end(); ++res_it) {
							int stype = res_it->first;
							int cnt = res_it->second;
							//1.addup
							receive += cnt;
							//add in mapStypeCnt
							mapStypeCnt[stype] += cnt;
						}
					} else {
						//DoLog("frined:  did not find friend:" << featureid);
					}

					if (IsTest(to)) {
						score = ScoreLogTen(click, reply, receive);
					} else {
						score = ScoreTwo(click, reply, receive);
					}
//					outfile << to << '\t' << featureid << '\t' << click << '\t' << reply << '\t' << receive << '\t' << score << '\n';
					outfile << to << '\t' << featureid << '\t' << score << '\n';
					// ++line_out_;
				}
			}//same person
		}
		// process the last person
		//DoLog("left tempsize:" << temp.size());
		if (!temp.empty()) {
			WriteFile4Stype(temp, mapStypeCnt, outfile);
		}

		infile.close();
		outfile.close();

		DoLog("ProcessClick cost: " << ts.getTime() << "linecnt: " << cnt);
	}

  //--------  process dispatch && clickreply done


	  class Person {
	  public:
		Person(int from=0, int to=0, float score=0.0):from_(from), to_(to), score_(score){
		}
		int from_, to_;
//		int clic_, repl_, rece_;
		float score_;
		bool operator < (const Person& B) const {
		  return this->score_ < B.score_;
		}
	  };
  


	  void WriteFile(vector<Person>& vecPerson, int vecPersonSize,
			  vector<Person>& vecType, int vecTypeSize, bool special, ofstream& outfile) {
		sort(vecPerson.begin(), vecPerson.begin()+vecPersonSize);
		float weight = 1.0, meta = 0.0, sum = 0.0;
		if (special) {
		  weight = 0.7;
		  sum = 0.3;
		}
		meta = weight/vecPersonSize, sum += meta;
		for(vector<Person>::iterator it = vecPerson.begin();
				it != vecPerson.begin()+vecPersonSize; ++it) {
		  outfile << it->to_ << '\t' << it->from_ << '\t' << sum << '\n';
//		  outfile << it->to_ << '\t' << it->from_ << '\t' << it->clic_ << '\t' << it->repl_ << '\t' << it->rece_ << '\t' << sum << '\n';
		  sum += meta;
		}

		sort(vecType.begin(), vecType.begin()+vecTypeSize); //reduct to [0.3~1]
		if (special) sum = 0.3;
		else sum = 0.0;
		meta = weight/vecTypeSize, sum += meta;
		for(vector<Person>::iterator it = vecType.begin();	it != vecType.begin()+vecTypeSize; ++it) {
//		  outfile << it->to_ << '\t' << it->from_ << '\t' << it->clic_ << '\t' << it->repl_ << '\t' << it->rece_ << '\t' << sum << '\n';
		  outfile << it->to_ << '\t' << it->from_ << '\t' << sum << '\n';
		  sum += meta;
		}

	  }

	  void ProcessStypeAver(ofstream & resfile) {//reduct to [0~0.5]
		  TimeStat ts;
		  typedef hash_map<int,ClickReplyReceive>::value_type valtype;
		  typedef multimap<float,int>::value_type valtype2;
		  float score, delta;
//	  	  const static float MININF = 1e-5;

	  	  //------------- the average for stype---------------
	  	  multimap<float,int> score_stype;
	  	  int stype_size = stype_map_.size();
	  	  BOOST_FOREACH(valtype & val, stype_map_) {
	  		  data = val.second;
	  		  if (data.click == 0 && data.reply == 0) {
	  			  DoLog("0 data!!  stype = "<<val.first<<"   click:" << data.click
	  					  <<"  reply:"<<data.reply<<"  dispatch:"<<data.receive);
	  			  continue;
	  		  }//score==0
  			  score = ScoreTen(data.click, data.reply, data.receive);
	  		  score_stype.insert(make_pair(score, val.first));
	  	  }
	  	  DoLog("ProcessAver::stype  before:"<<stype_size<< "   after:"<<score_stype.size());

	  	  score =  0;
	  	  delta = 0.5 / score_stype.size();
	  	  int cnt = 0;
	  	  BOOST_FOREACH(valtype2 & val, score_stype) {
	  		  cnt++;
//	  		  data = stype_map_[val.second];
//	  		  resfile << -val.second << "\t0\t"<<data.click<<"\t"<<data.reply<<"\t"<<data.receive<<"\t" << score << "\n";
	  		  resfile << -val.second << "\t0\t" << score << "\n";
	  		  score += delta;
	  	  }
	      DoLog("ProcessStypeAver cost: " << ts.getTime() << "StypeAver cnt: " << cnt);

	      stype_map_.clear();
	  }

	  void ProcessUserAver(ofstream & resfile) {//reduct to [0~0.5]
		  TimeStat ts;
		  typedef hash_map<int,ClickReplyReceive>::value_type valtype;
		  int user_size = user_map_.size(), count = 0, range_number = user_size / 1000;
		  float score;
		  // ---------the average for stype------------------
		  vector<int> vec_user_id;
		  vector<float> vec_user_score;
		  set<float> set_range;

		  vec_user_id.reserve(user_size);
		  vec_user_score.reserve(user_size);

		  int cnt = 0;
		  BOOST_FOREACH(valtype & val, user_map_) {
			  data = val.second;
//			  if (cnt++ % 100000 ==0 )
//    			  DoLog("line: " << cnt << "   "<< val.first);
  			  score = ScoreLogTen(data.click, data.reply, data.receive);
//			  score = ScoreUserAver(val.first, data.click, data.reply, data.receive );

			  vec_user_id.push_back(val.first);
			  vec_user_score.push_back(score);

			  if (count++ % range_number ==0) {//select 1000 user to get the sample
				  set_range.insert(score);
				  cnt++;
			  }
			  //resfile << val.first << "\t\t0\t" << score << "\n";
		  }

		  vector<float> vec_range(set_range.begin(), set_range.end());
		  DoLog("ProcessUserAver: chosen size "<< cnt << "    res size:" << vec_range.size());
		  int pos, range_size = vec_range.size();
		  cnt = 0;
		  for (int i = 0; i < vec_user_score.size(); ++i) {//compare with the 1000 sample and get each one's score
			  cnt++;
			  pos = BSearch(&vec_range[0], 0, range_size-1, vec_user_score[i]);
//	  		  data = user_map_[vec_user_id[i]];
//	  		  resfile << vec_user_id[i] << "\t0\t"<<data.click<<"\t"<<data.reply<<"\t"<<data.receive<< "\t" <<  (0.5f / (range_size - pos)) << "\n";
	  		  resfile << vec_user_id[i] << "\t0\t" << (pos + 1) / (range_size * 2.0) << "\n";
		  }

		  DoLog("ProcessUserAver cost: " << ts.getTime() << "UserAvercnt: " << cnt);
	      user_map_.clear();
	  }
	  //大于等于 val 的第一个值
	  int BSearch(float array[], int low, int high, float val) {
		  int mid;
		  while (low < high) {
			  mid = (low + high) >>1;
			  if (array[mid] < val) low = mid + 1;
			  else high = mid ;
		  }
		  return low;
	  }


	  void Process(const string& filename) { //reduct the scores
		string resfile = filename;
		resfile += ".result";
		ofstream outfile (resfile.c_str());
		ProcessStypeAver(outfile);
		ProcessUserAver(outfile);

		TimeStat ts;
		DoLog("PreferStat::Process!!")
		ifstream infile (filename.c_str(), ifstream::in);
		if(!infile) {
		  DoLog("Process open infile fail");
		  exit(0);
		  return ;
		}
		if(!outfile.is_open()) {
		  DoLog("Process open outfile fail");
		  exit(0);
		  return ;
		}
		cout << "Process: resultname " << resfile << endl;

		string line;
		int from, to, clic, repl, rece, old_id = -11;
		float score;
		vector<Person> vecPerson(VECSIZE);
		vector<Person> vecType(VECSIZE);
		int vecPersonSize = 0, vecTypeSize = 0, cnt = 0;
		while(getline(infile, line)) {
		  ++cnt;
		  if(cnt%10000000 == 0) {
			DoLog("line:" << cnt);
			//cout << "\nline: " << cnt;
		  }
	//      sscanf(line.c_str(), "%d\t%d\t%d\t%d\t%d\t%f", &to, &from, &clic, &repl, &rece, &score);
		  sscanf(line.c_str(), "%d\t%d\t%f", &to, &from, &score);
		  if(from == -1) {
			DoLog("evial data:!!  line:" << cnt <<" \n"<<line);
			continue;
		  }

		  if(old_id != to) {
			if(old_id != -11){
	//          int flag_bit = (old_id / 10) % 10;
	//          if ( flag_bit == 0 || flag_bit == 9 ) {
				WriteFile(vecPerson, vecPersonSize, vecType, vecTypeSize, true, outfile);
	//          } else {
	//            WriteFile(vecPerson, vecPersonSize, vecType, vecTypeSize, false, outfile);
	//          }
			}
			vecPersonSize = 0;
			vecTypeSize = 0;
			old_id = to;
		  }

		  if(from > 0) {//person
			if(vecPersonSize < VECSIZE) {
			  vecPerson.at(vecPersonSize).from_ = from;
			  vecPerson.at(vecPersonSize).to_ = to;
	//          vecPerson.at(vecPersonSize).clic_ = clic;
	//          vecPerson.at(vecPersonSize).repl_ = repl;
	//          vecPerson.at(vecPersonSize).rece_ = rece;
			  vecPerson.at(vecPersonSize).score_ = score;
			  vecPersonSize++;
			} else {
			  DoLog("WARN   *****  Process::vecPerson cache is too small!!!");
			  return ;
			}
		  } else {//Type
			if(vecTypeSize < VECSIZE) {
			  vecType.at(vecTypeSize).from_ = from;
			  vecType.at(vecTypeSize).to_ = to;
	//          vecType.at(vecTypeSize).clic_ = clic;
	//          vecType.at(vecTypeSize).repl_ = repl;
	//          vecType.at(vecTypeSize).rece_ = rece;
			  vecType.at(vecTypeSize).score_ = score;
			  vecTypeSize++;
			} else {
			  DoLog("WARN  ****** Process::vecType cache is too small!!!");
			  return ;
			}
		  }
		}

	//    int flag_bit = (old_id / 10) % 10;
	//    if ( flag_bit == 0 || flag_bit == 9 ) {
		  WriteFile(vecPerson, vecPersonSize, vecType, vecTypeSize, true, outfile);  //TODO
	//    } else {
	//      WriteFile(vecPerson, vecPersonSize, vecType, vecTypeSize, false, outfile);
	//    }

		infile.close();
		outfile.close();
		DoLog("Process cost: " << ts.getTime() << "linecnt: " << cnt);
	  }

	  void Clear(){
		  from_map_.clear();
		  stype_map_.clear();
		  user_map_.clear();
	  }

//
//	  float ScoreUserAver(int uid, float click, float reply, float receice) {
//		int friend_size ;
//		try {
//			friend_size = BuddyByIdCacheAdapter::instance().getFriendCount(uid);
//		} catch (...) {
//			DoLog("Buddy Wrong");
//		}
//
//		if (friend_size == 0) {
//			friend_size = 200;
//		}
//		return (click + 4*reply) / friend_size*log2(receice + 10);
//	  }


	  float ScoreTen(float click, float reply, float receice) {
		    return (click + 4*reply) / log2(receice + 10);
	  }

	  float ScoreLogTen(float click, float reply, float receice) {
		    return (click + 4*reply) / log2(receice + 10);
	  }

	  float ScoreTwo(float click, float reply, float receice) {
		    return (click + 3*reply) / log2(receice + 2);
	  }

	  bool IsTest(int uid) {
		  if (uid/100%100 == 69
					|| uid == 220678672 //jw
					|| uid == 258081713//jin.zhang
					|| uid == 238489851 //fei.yuan
					|| uid == 239556923 //jicheng.zhao
					|| uid == 263734453 //rong.tang
					|| uid == 200000032 //zhuanghongbin

						  ) {
			  return true;
		  }
		  return false;
	  }

private:
	hash_map<long, vector< pair<int,int> > > from_map_;
	hash_map<int, struct ClickReplyReceive> stype_map_;
	hash_map<int, struct ClickReplyReceive> user_map_;

  //int line_in_, line_out_, delta;
};


}

}
#endif



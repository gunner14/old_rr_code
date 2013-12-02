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
#include "FeedNaiveBayesAdapter.h"

using namespace std;
using  __gnu_cxx::hash_map;
using namespace xce::buddy::adapter;

namespace xce {
namespace feed {

#define DoLog(msg) std::cout << msg << std::endl;;
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
	int click,reply;
  long receive;
}data;

const int kMAX_FAIL_CNT = 3;

class PreferStat: public MyUtil::Singleton<PreferStat>{
public:
  PreferStat(): adapter_fail_cnt_(0) {
		Clear();
  }

  // dispatch :   A   stype   cnt(分发)   sum(展示)
  // eg:
  // 1000006096      8002    5       5
  // 100000749       8002    1       1
  // 100001395       102     2       756
  // 100001395       103     7       2638
  //
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
  void ProcessDispatch(const string & dispatchlog){  //load dispatch file
		TimeStat ts;
		ifstream infile;
		DoLog("PreferStat: ProcessDispatch. --> open file : " << dispatchlog);
		infile.open(dispatchlog.c_str(), ifstream::in);
		if (!infile) {
			DoLog("PreferStat: ProcessDispatch open file fail");
			exit(0);
		}

		int userid, stype, count, sum;
		int cnt = 0, old_id = -1;
		vector<pair<int, int> > vecStypeCnt;
		string line;
		while (getline(infile, line)) {
			if (++cnt % 10000000 == 0) DoLog("line: " << cnt);
			sscanf(line.c_str(), "%d\t%d\t%d\t%d", &userid, &stype, &count, &sum);

			AddReceive(user_map_, userid, sum); // store for useraverage
			if(stype>10000) {
				DoLog("dispatch file. _--> wrong_data:"<< line );
			} else {
				AddReceive(stype_map_, stype, sum);// store for stypeaverage
			}

			if (userid != old_id) {
				if (old_id != -1) {
					user_dispatch_cnt_[(long) old_id] = vecStypeCnt;//user:stypes that he create
				}
				vecStypeCnt.clear();
				old_id = userid;
			}
			vecStypeCnt.push_back(make_pair(stype, count));
		}
		user_dispatch_cnt_[(long) old_id] = vecStypeCnt; // the lase one

		infile.close();
		DoLog("PreferStat::ProcessDispatch. from_map size:" << user_dispatch_cnt_.size() 
				<<"   user_map_ size:"<< user_map_.size()
        <<"   stype_map_ size:"<<stype_map_.size()
        <<"   linecnt:" << cnt
        <<"   cost: " << ts.getTime());
	}


  //clickreply: userid  featured  click reply   
  //100000389       -110    1       0
  //100000389       -103    4       0
  //100000389       221962392       1       0
  //100000389       224214825       1       0
  //
	void WriteFile4Stype(vector<string>& temp, map<int, int> & mapStypeCnt, ofstream& outfile) { //stype
		int userid, featureid, click, reply, receive;
		float score = 0;
		for (vector<string>::iterator temp_it = temp.begin(); temp_it != temp.end(); ++temp_it) {
			receive = 0;
			sscanf(temp_it->c_str(), "%d\t%d\t%d\t%d", &userid, &featureid, &click,	&reply);
			map<int, int>::iterator map_it = mapStypeCnt.find((-featureid));
			if (map_it != mapStypeCnt.end()) {
				receive = map_it->second;
			}

			if (IsTest(userid)) {//new formula
				score = ScoreTen(click, reply, receive);
			} else {
				score = ScoreTwo(click, reply, receive);
			}
			outfile << userid << '\t' << featureid << '\t' <<  score << '\n';
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
    string tmpfile = scorefile +  ".tmp";

    ifstream infile;
    ofstream outfile;
    DoLog("PreferStat: ProcessClicReply open:" << clickreply);
    infile.open(clickreply.c_str(), ifstream::in);
    DoLog("PreferStat: ProcessClicReply open:" << tmpfile);
    outfile.open(tmpfile.c_str(), ofstream::out);
    if (!infile) {
      DoLog("PreferStat: ProcessClic click_file fial 1: did not exist?");
      exit(0);
    }
		if (!outfile.is_open()) {
			DoLog("PreferStat: ProcessClic tmp_file fail 2: could not open " << tmpfile);
			exit(0);
		}

		string line;
		int userid, featureid, click, reply, receive;
		int old_id = -1, cnt = 0;
		float score;
		vector<string> temp;
		map<int, int> mapStypeCnt;
		while (getline(infile, line)) {
			if (++cnt % 10000000 == 0)  DoLog("line:" << cnt);
			sscanf(line.c_str(), "%d\t%d\t%d\t%d", &userid, &featureid, &click,	&reply);// userid 对 featureid 的新鲜事点击回复
			if (userid == -1 || featureid ==0) {
				//DoLog("evial input!!!\nline:"<<cnt << "    data-->"<<line);
				continue;
			}

			if (featureid < 0) { // stype
				if(-featureid>10000) {
					DoLog("ClickRepy escape line:"<<line);
				} else  {
					AddClickReply(stype_map_, -featureid, click, reply);
				}
			} else { // friendid
				AddClickReply(user_map_, featureid, click, reply);
			}

			// --------- calculate  ------------------
			if (old_id != userid) {//new person!!
				//process older person
				if (old_id != -1 && !temp.empty()) {
					WriteFile4Stype(temp, mapStypeCnt, outfile);
				}
				old_id = userid;
				mapStypeCnt.clear();
				temp.clear();
			}

      int stype, cnt;
			{
				//same person or first show
				if (featureid < 0) {//stype   handle thest in WriteFile4Stype
					temp.push_back(line);
				} else {//featuredid == friendid
					receive = 0;
					hash_map<long, vector<pair<int, int> > >::iterator hi = user_dispatch_cnt_.find((long) featureid);
					
          if (hi != user_dispatch_cnt_.end()) {
						vector<pair<int, int> > res = hi->second;
						for (vector<pair<int, int> >::iterator res_it = res.begin(); 
                  res_it != res.end(); ++res_it) {
							stype = res_it->first;
							cnt = res_it->second;
							mapStypeCnt[stype] += cnt;
							receive += cnt;
						}
					} else {
						//DoLog("frined:  did not find friend:" << featureid);
					}

					if (IsTest(userid)) {
						score = ScoreLogTen(click, reply, receive);
					} else {
						score = ScoreTwo(click, reply, receive);
					}
					outfile << userid << '\t' << featureid << '\t' << score << '\n';
				}
			}//same person
		}
		if (!temp.empty()) {
			WriteFile4Stype(temp, mapStypeCnt, outfile);
		}

		infile.close();
		outfile.close();
	  hash_map<long, vector< pair<int,int> > > ().swap(user_dispatch_cnt_);
		DoLog("ProcessClick cost: " << ts.getTime() << "linecnt: " << cnt);
	}


  //--------  process dispatch && clickreply done --------------normalization the result--------------
	  class Person {
	  public:
		int userid, featureid;
		float score;
		bool operator < (const Person& B) const {
		  return this->score < B.score;
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
        outfile << it->userid << '\t' << it->featureid << '\t' << sum << '\n';
        sum += meta;

        userids_.push_back(it->userid);
        featureids_.push_back(it->featureid);
        scores_.push_back(sum);
        if (userids_.size() > 40000) {
          try {
            FeedNaiveBayesAdapter::instance().UpdateScoreBatch(userids_, featureids_, scores_);
          } catch (Ice::Exception& e) {
            DoLog("WARN!!!   ----> error:" << e << " fail_cnt:" << adapter_fail_cnt_);
            if (++adapter_fail_cnt_ > kMAX_FAIL_CNT) {
              sleep(10*adapter_fail_cnt_);
              //exit(0);
            }
            sleep(5);
          }
          userids_.clear();
          featureids_.clear();
          scores_.clear();
        }
      }

      sort(vecType.begin(), vecType.begin()+vecTypeSize); //reduct to [0.3~1]
      if (special) sum = 0.3;
      else sum = 0.0;
      meta = weight/vecTypeSize, sum += meta;
      for(vector<Person>::iterator it = vecType.begin();	it != vecType.begin()+vecTypeSize; ++it) {
        outfile << it->userid << '\t' << it->featureid << '\t' << sum << '\n';
        sum += meta;

        userids_.push_back(it->userid);
        featureids_.push_back(it->featureid);
        scores_.push_back(sum);
        if (userids_.size() > 40000) {
          try {
            FeedNaiveBayesAdapter::instance().UpdateScoreBatch(userids_, featureids_, scores_);
          } catch (Ice::Exception& e) {
            DoLog("WARN!!!   ----> error:" << e << " fail_cnt:" << adapter_fail_cnt_);
            if (++adapter_fail_cnt_ > kMAX_FAIL_CNT) {
              sleep(10*adapter_fail_cnt_);
              //exit(0);
            }
            sleep(5);
          }
          userids_.clear();
          featureids_.clear();
          scores_.clear();
        }      
      }
    }

	  void ProcessStypeAver(ofstream & resfile) {//reduct to [0~0.5]
		  TimeStat ts;
		  typedef hash_map<int,ClickReplyReceive>::value_type valtype;
		  typedef multimap<float,int>::value_type valtype2;
		  float score, delta;

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
	  	  DoLog("ProcessAver::stype  check the diff --> before:" << stype_size << "   after:"<<score_stype.size());

	  	  score =  0;
	  	  delta = 0.5 / score_stype.size();
	  	  int cnt = 0;
	  	  BOOST_FOREACH(valtype2 & val, score_stype) {
	  		  cnt++;
	  		  resfile << -val.second << "\t0\t" << score << "\n";
	  		  score += delta;

          userids_.push_back(-val.second);
          featureids_.push_back(0);
          scores_.push_back(score);
          if (userids_.size() >= 40000) {
            try {
              FeedNaiveBayesAdapter::instance().UpdateScoreBatch(userids_, featureids_, scores_);
            } catch (Ice::Exception& e) {
              DoLog("WARN!!!   ----> error:" << e << " fail_cnt:" << adapter_fail_cnt_);
              if (++adapter_fail_cnt_ > kMAX_FAIL_CNT) {
                sleep(10*adapter_fail_cnt_);
                //exit(0);
              }
              sleep(5);
            }
            featureids_.clear();
            userids_.clear();
            scores_.clear();
          }
	  	  }
	      DoLog("ProcessStypeAver cost: " << ts.getTime() << "StypeAver cnt: " << cnt);
	      hash_map<int, struct ClickReplyReceive> ().swap(stype_map_);
	      //stype_map_.clear();
	  }

	  void ProcessUserAver(ofstream & resfile) {//reduct to [0~0.5]
		  TimeStat ts;
		  typedef hash_map<int,ClickReplyReceive>::value_type valtype;
		  int user_size = user_map_.size(), count = 0, range_number = user_size / 1000; // pick a sample each 1000
		  float score;
		  vector<int> vec_user_id;
		  vector<float> vec_user_score;
		  set<float> set_range; // clean the duplocated and sort

		  vec_user_id.reserve(user_size);
		  vec_user_score.reserve(user_size);
		  int cnt = 0;

		  BOOST_FOREACH(valtype & val, user_map_) {
			  data = val.second;
  			score = ScoreLogTen(data.click, data.reply, data.receive);

			  vec_user_id.push_back(val.first);
			  vec_user_score.push_back(score);
			  if (count++ % range_number ==0) {//select 1000 user to get the sample
				  set_range.insert(score);
				  cnt++;
			  }
		  }

		  vector<float> vec_range(set_range.begin(), set_range.end());
		  DoLog("ProcessUserAver: chosen size "<< cnt << "    res size:" << vec_range.size());

		  int pos, range_size = vec_range.size();
		  cnt = 0;
		  for (size_t i = 0; i < vec_user_score.size(); ++i) {//compare with the 1000 sample and get each one's score
			  cnt++;
			  pos = BSearch(&vec_range[0], 0, range_size-1, vec_user_score[i]);
        score = (pos + 1) / (range_size * 2.0); 

	  		resfile << vec_user_id[i] << "\t0\t" << score << "\n";

        userids_.push_back(vec_user_id[i]);
        featureids_.push_back(0);
        scores_.push_back(score);
        if (userids_.size() >= 40000) {
          try {
            FeedNaiveBayesAdapter::instance().UpdateScoreBatch(userids_, featureids_, scores_);
          } catch (Ice::Exception& e) {
              DoLog("WARN!!!   ----> error:" << e << " fail_cnt:" << adapter_fail_cnt_);
              if (++adapter_fail_cnt_ > kMAX_FAIL_CNT) {
                sleep(10*adapter_fail_cnt_);
                //exit(0);
              }
            sleep(5);
          }
          featureids_.clear();
          userids_.clear();
          scores_.clear();
        }

		  }
		    DoLog("ProcessUserAver cost: " << ts.getTime() << "UserAvercnt: " << cnt);
	      hash_map<int, struct ClickReplyReceive> ().swap(user_map_);
	      //user_map_.clear();
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
      DoLog("PreferStat::Process --> open resfile:"<< resfile);
      ofstream outfile (resfile.c_str());
      if (!outfile.is_open()) {
        DoLog("Process open outfile fail");
        exit(0);
        return ;
      }
      ProcessStypeAver(outfile);
      ProcessUserAver(outfile);

      TimeStat ts;
      DoLog("PreferStat::Process --> all");
      string tmpfile = filename + ".tmp";
      DoLog("PreferStat::Process --> open infile:"<< tmpfile);
      ifstream infile (tmpfile.c_str(), ifstream::in);
      if(!infile) {
        DoLog("Process open infile fail");
        exit(0);
        return ;
      }
      string line;
      int userid, featureid, old_id = -11;
      float score;
      vector<Person> vecPerson(VECSIZE);
      vector<Person> vecType(VECSIZE);
      int vecPersonSize = 0, vecTypeSize = 0, cnt = 0;
      bool small_flag = false;
  
      while(getline(infile, line)) {
        if (++cnt % 10000000 == 0) {
          DoLog("line:" << cnt );
        }
        sscanf(line.c_str(), "%d\t%d\t%f", &userid, &featureid, &score);
        if(featureid == -1 || featureid == 0) {
          //DoLog("evial data:!!  line:" << cnt <<" \n"<<line);
          continue;
        }

        if(old_id != userid) {
          if(old_id != -11){
            WriteFile(vecPerson, vecPersonSize, vecType, vecTypeSize, true, outfile);
          }
          vecPersonSize = 0;
          vecTypeSize = 0;
          old_id = userid;
        }

        if (featureid > 0) {//friendid
          if (vecPersonSize < VECSIZE) {
            vecPerson.at(vecPersonSize).userid = userid;
            vecPerson.at(vecPersonSize).featureid = featureid;
            vecPerson.at(vecPersonSize).score = score;
            vecPersonSize++;
          } else {
            DoLog("WARN   *****  Process::vecPerson cache is too small!!!");
            small_flag = true;
            continue;
          } 
        } else {//Type
          if(vecTypeSize < VECSIZE) {
            vecType.at(vecTypeSize).userid = userid;
            vecType.at(vecTypeSize).featureid = featureid;
            vecType.at(vecTypeSize).score = score;
            vecTypeSize++;
          } else {
            DoLog("WARN  ****** Process::vecType cache is too small!!!");
            small_flag = true;
            continue;
          }
        }

        if (small_flag == true) {
          small_flag = false;
          for (int si = 0; si < VECSIZE; ++si) { 
            DoLog(" [ from:" << vecPerson[si].userid << " to:" << vecPerson[si].featureid << " score:" << vecPerson[si].score << " ]");
            DoLog(" < from:" << vecType[si].userid << " to:" << vecType[si].featureid << " score:" << vecType[si].score << " >");
          }
        }

      }
		  WriteFile(vecPerson, vecPersonSize, vecType, vecTypeSize, true, outfile);  //TODO

		  infile.close();
		  outfile.close();
		  DoLog("Process cost: " << ts.getTime() << "linecnt: " << cnt);
	  }

	  void Clear(){
		  user_dispatch_cnt_.clear();
		  stype_map_.clear();
		  user_map_.clear();
	  }

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
	hash_map<long, vector< pair<int,int> > > user_dispatch_cnt_;  // userid --> <stype, dis_cnt>
	hash_map<int, struct ClickReplyReceive> stype_map_; // stype --> clicked replyed dispatch_sum
	hash_map<int, struct ClickReplyReceive> user_map_;  // userid --> clicked replyed dispatch_sum

  vector<int> featureids_, userids_;
  vector<float> scores_;
  int adapter_fail_cnt_;
};

}
}
#endif



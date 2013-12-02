#ifndef FEATURE_H
#define FEATURE_H
#include "thread.h"

#include <pthread.h>
#include <map>
#include <string>
#include <ostream>
using namespace std;

#define MAX_FEAT_INX 10000


class Feature{
	public:
		Feature(){
			_index = new int[MAX_FEAT_INX];
			memset(_index, 0, sizeof(int)*MAX_FEAT_INX);
			_size=0;
			_label=-1;
		}
		virtual ~Feature(){
			delete [] _index;
			_index=NULL;
			_label=-1;
		}
		inline void add(int fid){
			_index[_size++] = fid;
		}
		inline void setLabel(int l){
			_label=l;
		}
		inline size_t size(){
			return _size;
		}
		inline int at(size_t i){
			return _index[i];
		}
		inline void reset(){
			_size=0;
			_label=-1;
		}
		inline void dump(ostream &os){
			if(_size==0)return;
			for(size_t i=0; i<_size; i++){
				os<<_index[i]<<" ";
			}
			os<<_label<<endl;
		}
	private:
		int* _index;
		size_t _size;
		int _label;
};

class FeatureMap:public map<string, int>{
	public:
		FeatureMap(){pthread_mutex_init(&mu, NULL);index=1;}
		virtual	~FeatureMap(){pthread_mutex_destroy(&mu);this->clear();}

		int setHLevel(int hlevel);
		int setHStar(int hstar);
		int setHLogin(int hlog);
		int setHGender(string hgender);

		int setGLevel(int glevel);
		int setGStar(int gstar);
		int setGLogin(int glog);
		int setGGender(string ggender);

		int setBYear(int hyear, int gyear);
		int setBCity(string hcity, string gcity);
		int setBUniv(int huniv, int guniv);
		int setBStage(int hstage, int gstage);

		void dump(const char* file);
		void load(const char* file);
	private:
		int _setFeat(string key);
		pthread_mutex_t	mu;	
		int index;
};

void GetFeature(int host, int guest, int accesstype, FeatureMap* featMap, Feature* feat);

class FeatureThread:public Thread{
	public:
		virtual void distribute(int threadnum, int part_num, int scope){
			_threadnum=threadnum;
			_part_num=part_num;
			_scope=scope;
		}
	protected:
		int _threadnum;
		int _part_num;
		int _scope;
};
#endif

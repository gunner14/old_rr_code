#include "time.h"
#include "Predictor.h"
#include "socialgraph/socialgraphproto/item.pb.h"
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphUserPhotoStatsTripodAdapter.h"
using namespace xce::socialgraph;

struct Compare {
	bool operator()(RecommendItem item1, RecommendItem item2) {
		return (item1.weight > item2.weight);
	}
} compare;

Predictor::Predictor(Logistic* logistic) {
	_logistic = logistic;
}

Predictor::~Predictor() {
	delete _logistic;
}

void Predictor::run() {

	/********************************读取新二度好友测试算法*******************************************/
/*	clock_t start, finish;
	start = clock();
	string hid = "232491085";
	vector<string> gids;
        Items* items = new Items();//gids
        string value;
        bool flag = SocialGraphRcdTripodAdapter::instance().Get(hid, &value);
        istringstream in(value);
        items->ParseFromIstream(&in);

        for(int i = 0; i < items->items_size(); i++)
        {
                Item item = items->items(i);
                int i_gid = item.id();
                ostringstream oss;
                oss << i_gid;
                gids.push_back(oss.str());
        }
        cout<<"size"<<gids.size()<<endl;
	finish = clock();
	cout << "elapsed : " << (double) (finish - start) / CLOCKS_PER_SEC << endl;
*/
	/****************************测试我对以下用户的点击率**************************************/
	
/*	 string hid = "248528808";
	vector<string>::iterator it;
  	map<string, int>::iterator ita;
  	int a=0;
  	int p=0;
  	int s=0;
  	int v=0;
  	int k=0;
  	string hvalue;
  	SocialGraphUserPhotoStatsTripodAdapter::instance().Get("232491085", &hvalue);
  	Items hitems;
    	hitems.ParseFromString(hvalue);
    	Item hitem = hitems.items(0);
    	a = hitem.fields(0);
    	p = hitem.fields(1);
    	s = hitem.fields(2);

  	for(it = gids.begin(); it != gids.end(); ++it){
    		Feature *feat = new Feature();
    		string value;
    		bool flag=false;
    		flag = SocialGraphUserPhotoStatsTripodAdapter::instance().Get(*it, &value);//获取相册特征

    		Items items;
    		items.ParseFromString(value);
    		Item item = items.items(0);
    		v = item.fields(3);
   		cout<<"a:"<<a<<"p:"<<p<<"s:"<<s<<"v:"<<v<<endl;
   		GetFeature("232491085",*it , a, p, s, v, 1, feat);
    		double prob = Logistic::predict(feat);
   		cout << *it << "\t" << prob << endl;
    		delete feat;
  	}
*/	 
}

bool Predictor::rank(int i_hid, RecommendItemSeq& item_list) {
//	MCE_INFO("item_list.size:"<<item_list.size());
	ostringstream id_oss;
	id_oss << i_hid;
	string hid = id_oss.str();

	list<string> gids;
	gids.push_back(hid);
	RecommendItemSeq::iterator it;

	for(it = item_list.begin(); it != item_list.end(); ++it) {//准备id，以批量获取
		ostringstream oss;
		oss << it->id;
		gids.push_back(oss.str());
	}
	
	map<string, string> photo_feature_map;//相册属性map
	map<string, bool> photo_feature_exist;//相册属性是否存在
	vector<int> vec_hid_photo_feature;//hid相册属性容器
        vector<int> vec_gid_photo_feature;//gid相册属性容器

	photo_feature_exist = SocialGraphUserPhotoStatsTripodAdapter::instance().MultiGet(gids, &photo_feature_map);//批量获取hid和所有gid的相册特征

	map<string, Profile> user_profile_map = Cache::getProfilesFromCache(gids);//批量获取hid和所有gid的profile

	Profile host_profile = (user_profile_map.find(hid))->second;//hid的profile
	
	map<string, string>::iterator s2siter = photo_feature_map.find(hid);
	if(s2siter != photo_feature_map.end()){
		Items items;
                items.ParseFromString(s2siter->second);
                Item item = items.items(0);
                vec_hid_photo_feature.push_back(item.fields(0));
                vec_hid_photo_feature.push_back(item.fields(1));
                vec_hid_photo_feature.push_back(item.fields(2));	
	} else {
		return false;
	}
	int cnt = 0;
	for(it = item_list.begin(); it != item_list.end(); ++it) {
		Feature* feat = new Feature();

		ostringstream oss1;
                oss1 << it->id;
                string gid = oss1.str();

		Profile guest_profile = (user_profile_map.find(gid))->second;//gid的用户信息

                string gid_photo_feature;
		s2siter = photo_feature_map.find(gid);
		if(s2siter != photo_feature_map.end()) {
			gid_photo_feature = (photo_feature_map.find(gid))->second;
			Items g_items;
                        g_items.ParseFromString(gid_photo_feature);
                        vec_gid_photo_feature.push_back(g_items.items(0).fields(3));
		} else {
			vec_gid_photo_feature.push_back(0);
		}
		cnt++;
                //MCE_INFO("cnt:"<<cnt);
	//	MCE_INFO("hid:"<<hid<<"gid:"<<gid);
	//	MCE_INFO("a:"<<vec_hid_photo_feature[0]<<"p:"<<vec_hid_photo_feature[1]<<"s:"<<vec_hid_photo_feature[2]<<"v"<<vec_gid_photo_feature[0]);
                GetFeature(hid, gid, host_profile, guest_profile,vec_hid_photo_feature, vec_gid_photo_feature, 1, feat);
                it->weight = Logistic::predict(feat);
                vec_gid_photo_feature.clear();
                delete feat;
              //  finish = clock();
              //  double span = (finish - start);
              //  MCE_INFO("span,get photo_feature:"<<span);
	}

	sort(item_list.begin(), item_list.end(), compare);
	return true;
}



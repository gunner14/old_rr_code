#include <fstream>
#include "FriendRankCreator.h"
#include <BuddyByIdReplicaAdapter.h>
using namespace xce::adapter::userbasic;
using namespace xce::buddy::adapter;
using namespace xce::socialgraph;
using namespace com::xiaonei::xce;
using namespace MyUtil;
using namespace std;

// Constructor & Destructor {{{
FriendRankCreator::FriendRankCreator()
{
//	fpActiveRank = fopen("/data/xce/friendranknew/data/friend_rank/active_rank.dat", "rb");
	fpActiveRank = fopen("/data/xce/friendrank/data/friend_rank/active_rank.dat", "rb");
	if (fpActiveRank==NULL)
	{
		MCE_ERROR("Check file 'active_rank.dat' exist!");
		exit(-1);
	}

	dataActiveRank = new ActiveRank[1000];
	dataStaticRank = new StaticRank[2000];

	LoadActiveRankIndex(fpActiveRank);
	fpFriendRank = fopen("/data/xce/friendranknew/data/friend_rank/friend_rank.dat", "wb");
	
	ranker = new Ranker;
}
FriendRankCreator::~FriendRankCreator()
{
	delete[] dataActiveRank;
	delete[] dataStaticRank;
	delete ranker;
	fclose(fpActiveRank);
	fclose(fpFriendRank);
}// }}}

// LoadActiveRankIndex {{{
int FriendRankCreator::LoadActiveRankIndex(FILE* fp)
{
	long linenum = 0;
	long start = 0;
	time_t start_time = GetYestodayZero(time(NULL));
	hash_map<USERID,long>::iterator iter;

	MCE_INFO("Load active_rank index (*1000000): ");
	while (true)
	{
		start = ftell(fp);
		if (fread(&headActiveRank, sizeof(ActiveRankHead), 1, fp)!=1)
			break;
		if (fread(dataActiveRank, sizeof(ActiveRank), headActiveRank.num, fp)
				!=(size_t)headActiveRank.num)
			break;
		//printf("\n---------------- %d ----------------\n", headActiveRank.uid);
		//for (int i=0; i<headActiveRank.num; ++i)
		//{
		//  dataActiveRank[i].Print(1);
		//}
		//getchar();

		if (++linenum % 1000000 == 0)
			MCE_INFO("\t"<<linenum/1000000);

		// 只更新最近一天有ActiveLog的用户
//		if (headActiveRank.date >= start_time/*true*/)
		{
			iter = mActiveRankPos.find(headActiveRank.uid);
			if (iter != mActiveRankPos.end())
				iter->second = start;
			else
				mActiveRankPos.insert(make_pair<USERID,long>(headActiveRank.uid,start));
		}
	}
//	MCE_INFO("mActiveRankPos.SIZE()"<<mActiveRankPos.size());
	MCE_INFO("Load active_rank index ... OK");
	return 0;
}// }}}

// CreateFriendRank
int FriendRankCreator::CreateFriendRank(const char* fname)
{
	FILE* fpStaticRank = fopen(fname, "rb");
	assert(fpStaticRank!=NULL);

	const long PRINTSTEP = 10000;
	vector<RankScore> vecRank;
	long linenum = 0;
	hash_map<USERID,long>::const_iterator iter;

	//---------------------------------
	MCE_INFO(fname);
	MCE_INFO("CreateFriendRank [*"<<PRINTSTEP<<"]: ");
	while (GetNextStaticRank(fpStaticRank))
	{
//		MCE_INFO("GET next staticrank");
		iter = mActiveRankPos.find(headStaticRank.uid);
		if (iter == mActiveRankPos.end())
			continue;   // 最近1天没有产生ActiveLog

		// 读取交互分值
		fseek(fpActiveRank, iter->second, SEEK_SET);
//		MCE_INFO("READ ACTIVE HEAD");
		if (fread(&headActiveRank, sizeof(ActiveRankHead), 1, fpActiveRank)!=1)
			return -1;
//		MCE_INFO("ACTIVERANKS");
		if (fread(dataActiveRank, sizeof(ActiveRank), headActiveRank.num, fpActiveRank)
				!=(size_t)headActiveRank.num)
			return -1;

//		MCE_INFO("ranker->rank");
		// 合并资料分值与交互分值
		ranker->Rank(headStaticRank, dataStaticRank, headActiveRank, dataActiveRank, vecRank);
		
		// 保存综合分值
//		MCE_INFO("ready save friendrank");
		SaveFriendRank(headStaticRank.uid, vecRank);
		vecRank.clear();
		if (++linenum % PRINTSTEP == 0)
			MCE_INFO("\t"<<linenum/PRINTSTEP);
	}

	//fprintf(stderr, "\n");
	fclose(fpStaticRank);
	return 0;
}

// GetNextStaticRank {{{
bool FriendRankCreator::GetNextStaticRank(FILE* fp)
{
	bool valid = true;
	while (true)
	{
		if (fread(&headStaticRank, sizeof(StaticRankHead), 1, fp)!=1)
		{
			valid = false;
			break;
		}
		if (fread(dataStaticRank, sizeof(StaticRank), headStaticRank.num, fp)
				!=(size_t)(headStaticRank.num))
		{
			valid = false;
			break;
		}
		//printf("\n---------------- %d ----------------\n", headStaticRank.uid);
		//for (int i=0; i<headStaticRank.num; ++i)
		//{
		//	dataStaticRank[i].Print();
		//	printf("\n");
		//}
		//getchar();
		break;
	}
	return valid;
}// }}}

// SaveFriendRank {{{
void FriendRankCreator::SaveFriendRank(int uid, vector<RankScore>& vecRank)
{
	//int rank_data[200];
	//int rank_data_len = 200 * sizeof(int);
	// get top 100
//	MCE_INFO("SAVE FRIENDRANK:VEC.SIZE():"<<vecRank.size());
	if (vecRank.size()>100)
		vecRank.erase(vecRank.begin()+100, vecRank.end());
	sort(vecRank.begin(), vecRank.end(), RankScoreComp_ByFid());

	float maxScore = 0;
	float minScore = 10000;
	for (vector<RankScore>::const_iterator iter = vecRank.begin();
			iter != vecRank.end(); ++iter)
	{
		if (maxScore < iter->score)
			maxScore = iter->score;
		if (minScore > iter->score)
			minScore = iter->score;
	}

	int userid = 0;
	int weight = 0;
	int number = 0;

	//cout << ">>>>>>>>> " << uid << " <<<<<<<<<<" << endl;
	fwrite(&uid, sizeof(int), 1, fpFriendRank);
	number = vecRank.size();
	fwrite(&number, sizeof(int), 1, fpFriendRank);
	float activeScore = 0;
	float staticScore = 0;
	for (vector<RankScore>::const_iterator iter = vecRank.begin();
			iter != vecRank.end(); ++iter)
	{
		userid = iter->fid;
		activeScore = iter->scoreActive.first;
		staticScore = iter->scoreStatic.first;
//		weight = 10 + (int)(90*(0.001+iter->score-minScore)/(0.001+maxScore-minScore));
		fwrite(&userid, sizeof(int), 1, fpFriendRank);
		fwrite(&staticScore, sizeof(float), 1, fpFriendRank);
		fwrite(&activeScore, sizeof(float), 1, fpFriendRank);
//		fwrite(&weight, sizeof(int), 1, fpFriendRank);
		
/*		if (whiteUids.find(uid) != whiteUids.end())
		{
			ostringstream filename;
			string str = "/data/xce/friendranknew/whiteUserData/";
	                filename<<str<<uid;
                	ofstream out(filename.str().c_str());
                 	out<<"uid: "<<uid<<"\t"<<"maxScore: "<<maxScore<<"\t"<<"minScore: "<<minScore<<endl;
                 	out<<"fid"<<"\t"<<"fname"<<"\t"<<"static"<<"\t"<<"active"<<"\t"<<"score"<<endl;
			pair<float,string> sscore = iter->scoreStatic;
			pair<float,string> ascore = iter->scoreActive;
			string name = GetUserName(uid);
			out<<userid<<"\t"<<name<<"\t"<<sscore.first<<"\t"<<sscore.second<<"\t"<<ascore.first<<"\t"<<ascore.second<<"\t"<<weight<<endl;
		}
*/		//printf("<%d,%d>", userid, weight);
	}
	//printf("\n");	getchar();
}// }}}

void FriendRankCreator::LoadWhiteUids()
{
	int  uid = 0;
	
	uid = 238111132;
	whiteUids.insert(uid);

	uid = 248528808;
	whiteUids.insert(uid);

	uid = 224838055;
	whiteUids.insert(uid);
	
	uid = 250137860;
	whiteUids.insert(uid);

	uid = 256404211;
	whiteUids.insert(uid);

	uid = 329010065;
	whiteUids.insert(uid);
	
	uid = 46358;
	whiteUids.insert(uid);

	uid = 232491085;
	whiteUids.insert(uid);
	
	uid = 230683026;
	whiteUids.insert(uid);
	
	uid = 15289; 
	whiteUids.insert(uid);
	
	return;	
}
string FriendRankCreator::GetUserName(int uid)
{
	string name = "";
/*	try {
		UserBasicDataPtr basicPtr = UserBasicAdapter::instance().getUserBasic(uid);
		name = basicPtr->name;
	}catch( Ice::Exception e){
		cout << "\nIce-> " << e.what() << ": GetUserName(" << uid << ")" << endl;
	}catch( std::exception e){
		cout << "\nstd-> " << e.what() << ": GetUserName(" << uid << ")" << endl;
	}catch( ... ){
		cout << "\nunknown exception" << ": GetUserName(" << uid << ")" << endl;
	}
*/
	return name;
}

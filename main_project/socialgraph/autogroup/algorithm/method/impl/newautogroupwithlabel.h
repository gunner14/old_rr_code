#ifndef _NEW_AUTOGROUPWITHLABEL_H_
#define _NEW_AUTOGROUPWITHLABEL_H_
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <math.h>

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

#include "OceCxxAdapter/src/BuddyByIdReplicaAdapter.h"
#include "OceCxxAdapter/src/UserNameAdapter.h"

#include "socialgraph/autogroup/algorithm/method/autogroup.h"
#include "socialgraph/autogroup/algorithm/method/impl/friendsgraph.h"
#include "socialgraph/autogroup/algorithm/method/impl/genericsortutil.h"
#include "socialgraph/autogroup/algorithm/method/impl/group.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace socialgraph {
namespace newautogroupwithlabel {

const float THRESHOLD1 = 0.66f;
const float THRESHOLD2 = 0.8f;
const float THRESHOLD3 = 0.5;
const int MEMBERS_THRESHOLD = 2;
//决定如何删除组成员的策略
enum Strategy {
	HUMAN = 0,
	AUTO
};

//------------------------------------------------------------
/*@brief 小组标签类型*/
enum LabelType {
	UniversityType = 0,
	HighSchoolType,
	JuniorSchoolType,
	ElementarySchoolType,
	CollegeType
};

struct GroupLabel {
	std::vector<std::string> label_list;		//组标签
	int num;			//该标签的出现的次数
	float rate;			//该标签所占小组全部标签的比例
	LabelType type;		//该标签的类型
};

struct GroupLabelComp {
	bool operator() (const GroupLabel& g1, const GroupLabel& g2) {
		return g1.rate > g2.rate;
	}
};

//-------------------------------------------------------------------------------------------------
//大学资料
struct University {
	University(const std::string& uname = "",
						 int uenroll_year = 0,
						 const std::string& uacademy = "") :
		name(uname), enroll_year(uenroll_year), academy(uacademy) { }

	bool operator== (const University& u) const {
		if (name != u.name ||
				academy != u.academy ||
				enroll_year != u.enroll_year) {
			return false;
		}
		return true;
	}

	std::string name;
	int enroll_year;
	std::string academy;
};
typedef std::vector<University> UnivInfoList;

struct HighSchool {
	HighSchool(const std::string& hname = "",
					   int henroll_year = 0,
						 const std::string& tmp = "") :
		name(hname), enroll_year(henroll_year) { }

	bool operator== (const HighSchool& h) const {
		if (name != h.name ||
				enroll_year != h.enroll_year) {
			return false;
		}
		return true;
	}

	std::string name;
	int enroll_year;
};
typedef std::vector<HighSchool> HighSchoolList;

struct College {
	College(const std::string& cname = "",
				  int cenroll_year = 0,
				  const std::string& tmp = ""):
		name(cname), enroll_year(cenroll_year) { }

	bool operator== (const College& c) const {
		if (name != c.name ||
				enroll_year != c.enroll_year) {
			return false;
		}
		return true;
	}

	std::string name;
	int enroll_year;
};
typedef std::vector<College> CollegeList;

struct JuniorSchool {
	JuniorSchool(const std::string& jname = "",
							 int jenroll_year = 0,
							 const std::string& tmp = ""):
		name(jname), enroll_year(jenroll_year) { }

	bool operator== (const JuniorSchool& j) const {
		if (name != j.name ||
				enroll_year != j.enroll_year) {
			return false;
		}
		return true;
	}

	std::string name;
	int enroll_year;
};
typedef std::vector<JuniorSchool> JuniorSchoolList;

struct ElementarySchool {
	ElementarySchool(const std::string& ename = "",
									 int eenroll_year = 0,
									 const std::string& tmp = ""):
		name(ename), enroll_year(eenroll_year) { }

	bool operator== (const ElementarySchool& e) const {
		if (name != e.name ||
				enroll_year != e.enroll_year) {
			return false;
		}
		return true;
	}

	std::string name;
	int enroll_year;
};
typedef std::vector<ElementarySchool> ElementarySchoolList;

struct UserNetWork {
	UnivInfoList univ_list_;
	HighSchoolList highschool_list_;
	JuniorSchoolList juniorschool_list_;
	CollegeList college_list_;
	ElementarySchoolList elementaryschool_list_;
};
typedef std::vector<UserNetWork> UserNetWorkSeq;

//------------------------------------------------------------

class NewAutoGroupWithLabel: public AutoGroup {
public:
	NewAutoGroupWithLabel() {
		userid_ = 0;
	}
	virtual ~NewAutoGroupWithLabel() {}

	void GetGroupResult(Items& result);
	//决定程序执行的条件	
	bool hook() {
		if (friendlist_.empty() || 2000 < friendlist_.size()) {
			grouped_stats_.userid_ = userid_;
			grouped_stats_.friendsnum_ = (int)friendlist_.size();
			return false;	
		}
		return true;
	}

	void Display(); 
	GroupStats GetGroupStats();

protected:
	void CreateGroup();
	void CreateGroupTags();
	void CreateStats();

private:
	void CreateClique();
	void SortMembersOfIndexGroup();
	void AddFriendsIntoGroup();
	void DeleteMembersFromGroup(Strategy strategy, const std::map<int, std::vector<int> >& backupmap);
	void UniteSimilarGroup();
	void RefineIndexGroup();
	void CreateUngrouped();
	void CreateRealGroup();

	/*@brief 获取好友的资料信息*/
	void GetFriendsNetwork();
	
	/* @brief 获取组标签
   * @param type 信息类型
   * @param friendlist 组成员
   */
	GroupLabel GetGroupLabel(LabelType type, 
													 const std::vector<int>& friendlist);

	/* @brief 获取小组最适合的标签
   * @param item_list 小组的标签列表
   */
	template <typename T>
	std::pair<T, int> FetchBestItem(const std::vector<T>& item_list);

	/* @brief 合并相同标签的小组 */
	void MergeSameLabel();

	void ShowUserNetwork();

	std::map<int, std::vector<int> > indexgroup_;
	std::map<int, std::vector<int> > group_members_;
	std::map<int, GroupLabel> group_label_;
	std::map<int, UserNetWork> user_network_map_;
	GroupStats grouped_stats_;
};

//------------------------------------------------------------------------------------------

/*@brief 获取用户不用类别资料信息的线程*/
template <typename T>
class LoadUserDataThread : virtual public IceUtil::Thread {
public:
	/* @brief 获取用户不用类别资料信息
   * @param type load哪种资料(UniversityType,HighSchoolType,..., ElementarySchoolType,CollegeType)
   * @param table load哪个表
   * @param terms 哪几个字段
   * @param friendlist 要取哪些id的资料
   * @param data 返回值存储的结构
   */
	LoadUserDataThread(LabelType type, 	
										 const std::string table, 	
										 const std::vector<std::string>& terms, 
										 const std::vector<int>& friendlist,	
										 std::map<int, std::vector<T> >& data): 	
		type_(type), table_(table), terms_(terms), friendlist_(friendlist), data_(data) { }

	virtual void run();
	
private:
	LabelType type_;
	std::string table_;
	std::vector<std::string> terms_;
	const std::vector<int>& friendlist_;
	std::map<int, std::vector<T> >& data_;
};

//------------------------------------------------------------------------------------------
/*统一的取数据类*/
template <typename T>
class InfoHandler: public com::xiaonei::xce::ResultHandler {
public:
	/* @brief 获取用户资料Db类
   * @param type 获取哪类信息
   * @param std::map<int, T>存储进用户的map中
   * @param enroll_term 入学年，默认值为"enroll_year"
   * @param userid_term 用户id项，默认值为"userid"
   */
	InfoHandler(LabelType type, 
							std::map<int, std::vector<T> >& data,
							const std::string enroll_term = "enroll_year", 
							const std::string userid_term = "userid"):
		type_(type), data_(data), enroll_term_(enroll_term), userid_term_(userid_term) { }

	virtual bool handle(mysqlpp::Row& row) const; 

private:
	LabelType type_;
	std::map<int, std::vector<T> >& data_;
	std::string enroll_term_;
	std::string userid_term_;
};

}
}
}

#endif

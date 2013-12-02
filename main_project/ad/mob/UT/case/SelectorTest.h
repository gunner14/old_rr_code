#ifndef __SELECTOR_TEST__
#define __SELECTOR_TEST__

#include "ad/mob/UT/code/Handler.h"
#include "ad/mob/UT/stub/Data.h"
#include "gtest/gtest.h"

using namespace xce::ad;

//Test Selector
class SelectorTestParas {
public:
	SelectorTestParas() {
		for(int i = 0; i < 5; ++i) { cpm_eq.insert(GetAdGroupPtr(10000000010000+i, 20, 1)); }
		for(int i = 0; i < 3; ++i) { cpm_asc.insert(GetAdGroupPtr(10000000010000+i, 20, 1+i)); }
		
		int i = 0;
		for(; i < 2; ++i) {	cpm_cpc_eq.insert(GetAdGroupPtr(10000000010000+i, 20, 3)); }
		for(; i < 4; ++i) {	cpm_cpc_eq.insert(GetAdGroupPtr(10000000010000+i, 10, 1)); }

		i=0;
		for(; i < 2; ++i) {	cpm_cpc_asc.insert(GetAdGroupPtr(10000000010000+i, 20, 1+i));	}
		for(; i < 4; ++i) {	cpm_cpc_asc.insert(GetAdGroupPtr(10000000010000+i, 10, 1+i));	}

		i=0;
		for(; i < 20; ++i) { filter_groups_set.insert(GetAdGroupPtr(10000000010000+i, 20, 1+i)); }
		i=0;
		for(; i < 20; ++i) { filter_groups_seq.push_back(GetAdGroupPtr(10000000010000+i, 20, 1+i)); }
	}

public:
	set<AdGroupPtr> cpm_eq;
	set<AdGroupPtr> cpm_asc;

	set<AdGroupPtr> cpm_cpc_eq;
	set<AdGroupPtr> cpm_cpc_asc;

	set<AdGroupPtr> filter_groups_set;
	vector<AdGroupPtr> filter_groups_seq;
private:
	AdGroupPtr GetAdGroupPtr(Ice::Long id, int trans_type, int max_price) {
		AdGroupPtr group = new AdGroup;
		group->set_group_id(id);
		group->set_trans_type(trans_type);
		group->set_max_price(max_price);
		return group;
	}
};

class Selector_Test_RandomSelect_Cpm_Eq : public ::testing::TestWithParam< set<AdGroupPtr> > {
};

TEST_P(Selector_Test_RandomSelect_Cpm_Eq, TEST) {
	set<AdGroupPtr> para = GetParam();
	map<AdGroupPtr, int> result;
	for(int i=0; i < 10000; i++) {
		AdGroupPtr group;
		Selector selector;
		selector.RandomSelect(para, group);
		if ( result.find(group) == result.end() ) {
			result[group] = 1;
		} else {
			result[group]++;
		}
	}
	int count_new = result.begin()->second;
	int count_old;
	map<AdGroupPtr, int>::const_iterator it = result.begin();
	++it;
	for(; it != result.end(); ++it) {
		count_old = count_new;
		count_new = it->second;
		EXPECT_GT(50,abs(count_old-count_new));
	}
}

class Selector_Test_RandomSelect_Set_Filter : public ::testing::TestWithParam< set<AdGroupPtr> > {
};

TEST_P(Selector_Test_RandomSelect_Set_Filter, TEST) {
	set<AdGroupPtr> para = GetParam();
	set<AdGroupPtr>::const_iterator it = para.begin();
	set<Ice::Long> cand;
	for(; it != para.end(); ++it) {
		cand.insert((*it)->group_id());
	}
	for(int i=0; i<100; ++i) {
		AdGroupPtr group;
		Selector selector;
		selector.RandomSelect(para, group);
		set<AdGroupPtr> result;
		result.insert(group);
		selector.FilterGroups(result, cand);
		EXPECT_EQ(0,result.size());
	}
}

const SelectorTestParas select_test_para; 
INSTANTIATE_TEST_CASE_P(Handler_Test, Selector_Test_RandomSelect_Cpm_Eq, ::testing::Values(select_test_para.cpm_eq));
INSTANTIATE_TEST_CASE_P(Handler_Test, Selector_Test_RandomSelect_Set_Filter, ::testing::Values(select_test_para.filter_groups_set));


#endif

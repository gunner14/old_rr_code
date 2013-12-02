/*
 * Common.cpp
 *
 *  Created on: Mar 27, 2012
 *      Author: rong.tang
 */

#include "Date.h"
#include <algorithm>
#include <limits>

#include "Common.h"
#include "FeedBuilder.h"
#include "UserNameAdapter.h"

using namespace xce::adapter::username;
using namespace xce::feed;
using namespace std;
using namespace MyUtil;

int GetRequest::GetShowedFidsFromHFeed1D(HFeed1D & hfeed1d, SFeed2D & sfeed2d) {
	sfeed2d.push_back(SFeed1D());
	SFeed1D & sfeed1d = *(sfeed2d.rbegin());

	HeavyFeedPtr first_hf = *(hfeed1d.begin());
	int mergetype = first_hf->GetFeedNewsMergeType();
	if (mergetype != FeedMergeReplace && mergetype != FeedMergeAppend) {
		MCE_WARN("GetRequest::GetShowedFidsFromHFeed1D. --> unknown mergetype: " << mergetype
				<< " uid:" << uid_ << first_hf->ToString() );
		mergetype = FeedMergeReplace;
	}

	bool has_comment = false;
	vector<long> useless_fids;
	ostringstream oss;
	BOOST_FOREACH(HeavyFeedPtr hfeed, hfeed1d) {
		
    if(	(hfeed->GetStype()!=8182) && (mergetype == FeedMergeReplace && sfeed1d.size() >= 1 * 2)
				||(mergetype == FeedMergeAppend && (int)sfeed1d.size() >= FeedBuilder::MERGECOUNT )) {
			useless_fids.push_back(hfeed->GetFid());
			continue;
		}

		sfeed1d.push_back(hfeed->GetSharedFeedPtr());
		showed_fids_.insert(hfeed->GetFid());

		if (hfeed->IsMarked() == true) {
			marked_fids_.insert(hfeed->GetFid());
		}

		pair<int,long> p = hfeed->GetStypeMerge();
		if(!MergeConfig::instance().IsMain(p.first)) continue;

		HFeed1D comments ;
		if(!GetComments(p, comments)) continue;

		BOOST_FOREACH(HeavyFeedPtr & hfeed , comments){
			has_comment = true;
			showed_fids_.insert(hfeed->GetFid());
			oss << " :" << hfeed->GetFid();
			showed_comment_map_[p].push_back(hfeed->GetSharedFeedPtr());
		}
	}
	if (has_comment == true)
		MCE_DEBUG("GetRequest::GetShowedFidsFromHFeed1D -->  @@@attention, has_commnet:  " << oss.str());

//	MCE_INFO("@@. GetRequest::GetShowedFidsFromHFeed1D. uid:" << uid_
//				<< " mergetype:" << mergetype
//				<< " first feed " << first_hf->ToString()
//				<< " hfeed1d size:" << hfeed1d.size()
//				<< " useless_fids count:" << useless_fids.size()
//				<< " useless fids:" << PrintHelper::Print(useless_fids)
//				<< " @@@@ check the same:" << oss.str()
//	);
	return useless_fids.size();
}

set<long> & GetRequest::GetShowedFids(){
  if (getshow_) return showed_fids_;

  for(int i3d = begin_; i3d < begin_+limit_ && i3d < (int)hfeed3d_.size(); ++i3d ) {
  	HFeed2D & hfeed2d = hfeed3d_.at(i3d);
  	showed_sfeed3d_.push_back(SFeed2D());
  	SFeed2D & sfeed2d = *(showed_sfeed3d_.rbegin());

    BOOST_FOREACH(HFeed1D & hfeed1d, hfeed2d){
    	useless_count_ += GetShowedFidsFromHFeed1D(hfeed1d, sfeed2d);
    }
  }

  older_count_ =std::max( int(hfeed3d_.size() - (begin_ + limit_)), 0);

  getshow_ = true;
  MCE_INFO("GetRequest::GetShowedFids --> show_sfeed3d_size:" << showed_sfeed3d_.size()
  			<< " uselesssize:" << useless_count_
  			<< " showfidsize:" << showed_fids_.size()
  			<< " markedfeedsize:" << marked_fids_.size());

  return showed_fids_;
}


int GetRequest::GetOlderCount(){
	if(!getshow_)
		GetShowedFids();
	return older_count_;
}

SFeed3D & GetRequest::GetShowedSharedFeed3D(){
	if(!getshow_)
		GetShowedFids();
	return showed_sfeed3d_;
}


bool GetRequest::GetComments(pair<int,long> stypemerge,HFeed1D & comments){
	StypemergeHFeed1DMap::iterator cit= comment_map_.find(stypemerge);

	if(cit == comment_map_.end()) return false;

	comments = cit->second;
	return true;
}

string GetRequest::ToString(){
	ostringstream oss;
	oss << " uid_:" << uid_
			<< " sort_:" << sort_
			<< " comments_:" << comment_map_.size()
			<< " stypemerge_item1d_map_:"  << stypemerge_item1d_map_.size()
			<< " score_iterator_multimap_:" << score_iterator_multimap_.size()
			<< " item3d:" << hfeed3d_.size()
	;
	return oss.str();
}

void GetRequest::PushBackRandom(){
	if(random_){
		HFeed1D hfeed1d;
		hfeed1d.push_back(random_);
		HFeed2D hfeed2d;
		hfeed2d.push_back(hfeed1d);
		hfeed3d_.push_back(hfeed2d);
	}else{
	}
}

bool GetRequest::CheckArgs(){

	if (filter_style_ == kError or uid_ < 0 or begin_ < 0 or limit_ <= 0) {
		MCE_WARN("GetRequest::CheckArgs. arguments invalid. uid:" << uid_
				<< " begin:" << begin_
				<< " limit:" << limit_ << " hot_:" << sort_
				<< " include_types:" << PrintHelper::Print(include_types_)
				<< " exclude_types:" << PrintHelper::Print(exclude_types_)
		);
		return false;
	}
	return true;
}

void GetRequest::DumpComments() {
	BOOST_FOREACH(StypemergeHFeed1DMap::value_type & e, comment_map_) {
		MCE_INFO("GetRequest::DumpComments. stype:" << e.first.first << " merge:" << e.first.second );
		BOOST_FOREACH(HeavyFeedPtr & hfeed,e.second) {
			MCE_INFO("GetRequest::DumpComments. \tfid:" << hfeed->GetFid());
		}
	}
}

bool IsUgc(int stype){
	int btype = (stype & 0xFFFF) / 100 ;
	return btype == 5 or btype == 6 or btype == 7;
}

void GetRequest::RaiseUgc(){
	static const int kRaiseCount = 6;
	if(!MustRaiseUgc())
		return;
	MCE_INFO("GetRequest::RaiseUgc. before raise uid:" << uid_);
	double maxscore = score_iterator_multimap_.rbegin()->first;
	int orig = score_iterator_multimap_.size();

	vector<pair<double,StypemergeHFeed1DMap::iterator> > v;
	typedef pair<double,StypemergeHFeed1DMap::iterator>  vvt;
	BOOST_FOREACH(ScoreIteratorMultiMap::value_type & mvt, score_iterator_multimap_){
		v.push_back(make_pair(mvt.first,mvt.second));
	}
	score_iterator_multimap_.clear();
	int raised = 0;
	set<long> raised_set;
	BOOST_REVERSE_FOREACH(vvt & vt, v){
		if(raised < kRaiseCount and IsUgc(vt.second->first.first)){
			vt.first = vt.first + maxscore;
//			HeavyFeedPtr & hfeed = *(vt.second->second.begin());
			raised_set.insert((*(vt.second->second.begin()))->GetFid());
			++raised;
		}
		score_iterator_multimap_.insert(vt);
	}

	MCE_INFO("GetRequest::RaiseUgc. after raise uid:" << uid_
			<< " maxscore:" << maxscore
			<< " orig:" << orig
			<< " final:" << score_iterator_multimap_.size()
			<< " raised:" << raised
			<< " fids:" << PrintHelper::Print(raised_set)
			 )
}

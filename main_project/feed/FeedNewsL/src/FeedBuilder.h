/*
 * FeedBuilder.h
 *
 *  Created on: Sep 14, 2011
 *      Author: yejingwei
 */
#include "RFeed.h"
#include "FeedItemI.h"
#include "Common.h"
#include <boost/foreach.hpp>
namespace xce {
namespace feed {

class FeedBuilder {
public:
	FeedBuilder(int uid,FeedContentDict & fdict,StypemergeSFeed1DMap & comments,bool show_comment)
		:hidesize_(0), mergesize_(0), uid_(uid),fdict_(fdict),comment_map_(comments), show_comment_(show_comment) {}

	FeedDataResPtr BuildFeedData(GetRequest &);
	const set<long> & GetNullFids(){ return nullfids_;}

  static bool NeedAppendStype(int stype) { return stype == 701 || stype == 601; }
private:
	FeedDataPtr BuildAFeed(SFeed2D &);
	FeedDataPtr BuildAFeed_8182(SFeed2D &);

	bool BuildFrontFeed(SFeed1D & , FeedDataPtr &, FeedReplyPtr&,ostringstream & );
	bool BuildComments(SFeed1D & , FeedDataPtr &, ostringstream & );

	void AppendReplyToXml(FeedReplyPtr & reply, ostringstream & xml);
	void AppendHiddenFidsToXml(set<long> & hiddens, ostringstream & xml);

	bool ShouldAdd(const SharedFeedPtr & sfeed, int merge_count);
	FeedContentPtr GetContent(long fid);

public:
	static const int MERGECOUNT = 5;
  int hidesize_;
  int mergesize_;
private:
	int uid_;
	FeedContentDict & fdict_;
	StypemergeSFeed1DMap & comment_map_;
	bool show_comment_;
	set<long> nullfids_;
};

//---------------------------------------------------
inline FeedDataResPtr FeedBuilder::BuildFeedData(GetRequest & req) {
	FeedDataResPtr res = new FeedDataRes;

//	HItem3D & item3d = req.item3d_;
//	int suc = 0,fail = 0;
//  for(int i3d = req.begin_; i3d < req.begin_+req.limit_ && i3d < (int)item3d.size();++i3d ){
//  	HItem2D & sfeed2d = item3d.at(i3d);
//		FeedDataPtr afeed = BuildAFeed(sfeed2d);
//		if(afeed) {
//			++suc;
//			res->data.push_back(afeed);
//		}else{
//			++fail;
//		}
//  }

	SFeed3D &  sfeed3d = req.GetShowedSharedFeed3D();
	int suc = 0,fail = 0;
	BOOST_FOREACH(SFeed2D & sfeed2d,sfeed3d) {
	  FeedDataPtr afeed;
    if(sfeed2d.begin()->begin()->raw_ptr_->GetStype()==8182) { //stype=8182     
      afeed = BuildAFeed_8182(sfeed2d);
    } else {
      afeed = BuildAFeed(sfeed2d);
    }
		if(afeed) {
			++suc;
			res->data.push_back(afeed);
		}else{
			++fail;
		}
	}

	return res;
}
inline FeedDataPtr FeedBuilder::BuildAFeed_8182(SFeed2D & sfeed2d) {
  FeedDataPtr global_data;
  ostringstream os;

  BOOST_FOREACH(SFeed1D & sfeed1d, sfeed2d) {
      BOOST_FOREACH(SharedFeedPtr & sfeed,sfeed1d) {
        FeedContentPtr content;
        content = GetContent(sfeed->GetFid());
        if(!content) {continue;}
        if(!global_data){
          global_data = content->data;
        }
        os << content->data->xml;
      }
  }
  if (!global_data) return 0;
  global_data->xml=os.str();
  return global_data;
}
inline FeedDataPtr FeedBuilder::BuildAFeed(SFeed2D & sfeed2d) {
	FeedDataPtr global_data;
	FeedReplyPtr global_reply;
	ostringstream os;
	set<long> hiddens;

	BOOST_FOREACH(SFeed1D & sfeed1d, sfeed2d) {
		if(!global_data) {
			BuildFrontFeed(sfeed1d,global_data,global_reply,os);
		} else {
			hiddens.insert(sfeed1d.front()->GetFid());
		}
	}

	if (!global_data) return 0; 

  if (0 && ((uid_ / 1000) % 10 == 5 || uid_ == 241610785 || uid_ == 461193456) && NeedAppendStype(sfeed2d.front().front()->GetStype()) && show_comment_) {
    if (hiddens.size() > 1) {
      int i = 0;
      for(set<long>::const_reverse_iterator it = hiddens.rbegin(); it != hiddens.rend() && i < 4; ) {
        FeedContentPtr content = GetContent(*it);
        if (!content) {
          MCE_DEBUG("FeedBuilder::BuildAFeed " << *it);
          hiddens.erase(*it);
          continue;
        }
        ++it;++i;
        os << content->data->xml;
      }
    }
  }

  int useridx = (uid_ / 1000) % 10;
  if (sfeed2d.size() > 1) {
    mergesize_ += hiddens.size();
  } else if (sfeed2d.size() == 1 && sfeed2d.front().size() > 1 &&
      sfeed2d.front().front()->GetFeedNewsMergeType() == FeedMergeReplace) {
    if (useridx == 6) {
      for(SFeed1D::const_iterator it = sfeed2d.front().begin() + 1; it != sfeed2d.front().end(); ++it) {
        hidesize_++;
        hiddens.insert((*it)->GetFid());
      }
    }
  }

  if(global_reply)
		AppendReplyToXml(global_reply,os);

	if(!hiddens.empty())
		AppendHiddenFidsToXml(hiddens,os);

	global_data->xml = os.str();
	return global_data;
}

inline bool FeedBuilder::BuildFrontFeed(SFeed1D & sfeed1d ,
		FeedDataPtr & global_data ,FeedReplyPtr& global_reply,ostringstream & os){
	if(sfeed1d.empty() || global_data || global_reply){
		MCE_WARN("FeedBuilder::BuildFrontFeed. input data and reply must be null!!");
		return false;//考虑用异常
	}
	bool has_comment = false;
	if(show_comment_ && MergeConfig::instance().IsMain(sfeed1d.at(0)->GetStype()))
	 	has_comment = BuildComments(sfeed1d,global_data,os);
	int added_count = 0;
	BOOST_FOREACH(SharedFeedPtr & sfeed,sfeed1d) {
		FeedContentPtr content = GetContent(sfeed->GetFid());
		if(!content) {continue;}
		//!!!如果有评论新鲜事，返回的FeedData的数据结构必须是comment
		if(!global_data){
			global_data = content->data;
			global_reply = content->reply;
		}
		//如果有评论，最好能取到一条reply
		if(has_comment && !global_reply){
			global_reply = content->reply;
		}
//		if(!has_comment || !global_data){ global_data = content->data;}


		if(!ShouldAdd(sfeed,added_count)) break; // 已经足够了

		os << content->data->xml;//不能用global_data的xml
		++added_count;
	}
	if(added_count == 0) { global_data = 0; global_reply = 0;}
	return global_data;
}

inline bool FeedBuilder::BuildComments(SFeed1D & sfeed1d ,
		FeedDataPtr & global_data ,ostringstream & os){
 	static const int COMMENT_SIZE = 5;

 	SharedFeedPtr & first_sfeed =  sfeed1d.front();
 	pair<int,long> vstype_merge = first_sfeed->GetVersionStypeMerge();
 	StypemergeSFeed1DMap::iterator it = comment_map_.find(vstype_merge);
	if(it == comment_map_.end()) {

		if(uid_ == 220678672 or uid_ == 347348947 or uid_ == 347349486 or uid_ == 347735846){
			int vstype = vstype_merge.first xor 0x10000;
			vstype_merge.first = vstype;
			it = comment_map_.find(vstype_merge);
			if(it != comment_map_.end()){
				SharedFeedPtr first_comment = it->second.front();
				MCE_INFO("FeedBuilder::BuildComments. version mismatch. uid:" << uid_
						<< " first fid:"<< first_sfeed->GetFid()
						<< " version:" << first_sfeed->GetVersion()
						<< " stype:" <<  first_sfeed->GetStype()
						<< " merge:" << first_sfeed->GetMerge()
						<< " first comment fid:" 	<< first_comment->GetFid()
						<< " version:" << first_comment->GetVersion()
						<< " stype:" <<  first_comment->GetStype()
						<< " merge:" << first_comment->GetMerge()
				);
			}
		}

		return false;
	}

	SFeed1D & comment1d = it->second;
	int added_count = 0;
	BOOST_FOREACH(SharedFeedPtr & comment,comment1d) {
		FeedContentPtr content = GetContent(comment->GetFid());
		if (!content) continue;
		global_data = content->data;

		os << global_data->xml;
		++added_count;

		if (added_count > COMMENT_SIZE) break;
	}

	return added_count > 0;
}

inline FeedContentPtr FeedBuilder::GetContent(long fid) {
	FeedContentDict::const_iterator dict_it = fdict_.find(fid);
	if(dict_it == fdict_.end()) {
		return 0;//在memcache中不存在，在db中可能有
	}
	FeedContentPtr content = dict_it->second;
	if(!content || !(content->data)){//防止data为空
		nullfids_.insert(fid);
		return 0;
	}
	return content;
}

inline bool FeedBuilder::ShouldAdd(const SharedFeedPtr & sfeed, int merge_count) {
	int mergetype = sfeed->GetFeedNewsMergeType();
	if(mergetype != FeedMergeReplace && mergetype != FeedMergeAppend){
		MCE_WARN("FeedBuilder::ShouldAdd. unknown mergetype: " << mergetype << " fid:" << sfeed->GetFid());
		return merge_count == 0;//当成replace处理
	}

	bool res = (sfeed->IsMergeReplace() && merge_count == 0 )
					|| (sfeed->IsMergeAppend() && merge_count < MERGECOUNT);
	return res;
}

inline void FeedBuilder::AppendReplyToXml(FeedReplyPtr & reply,ostringstream & xml ) {
	xml << reply->oldReply
			<< reply->newReply
			<< "<reply_count>" << reply->count << "</reply_count>";
}

inline void FeedBuilder::AppendHiddenFidsToXml(set<long> & hiddens,ostringstream & xml) {
	if (hiddens.empty()) { return; }

	xml << "<merge_feed><count>" << hiddens.size() << "</count>";
	BOOST_REVERSE_FOREACH(long fid,hiddens){
		xml << "<fid>" << fid << "</fid>";
	}
	xml << "</merge_feed>";
}
};
};

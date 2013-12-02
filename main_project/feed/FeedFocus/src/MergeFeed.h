/**
 * MergeFeed.h
 * jin.zhang
 * 2012.02.28
 */
#ifndef _MERGEFEED_H_
#define _MERGEFEED_H_

#include "Singleton.h"
#include "RFeed.h"
#include <map>

namespace xce {
namespace feed{
namespace feedfocus{

using namespace MyUtil;
using namespace std;
class MergeFeed: public Singleton<MergeFeed>{
public:
    void merge(FeedItemSeqSeq& iss) {
        FeedItemSeq feeds;
        for (FeedItemSeqSeq::iterator it2 = iss.begin(); 
                it2 != iss.end(); it2++) {
            for (FeedItemSeq::iterator it1 = it2->begin(); 
                    it1 != it2->end(); it1++)
                feeds.push_back(*it1);
        }

        iss.clear();
        map<pair<int, Ice::Long>, int> stypemerge_index;
        for (size_t i = 0; i < feeds.size(); i++) {
            FeedItem& item = feeds.at(i);
            map<pair<int, Ice::Long>, int>::iterator it = 
                stypemerge_index.find(make_pair<int, Ice::Long>(verSmallType(item), item.merge));

            if (it != stypemerge_index.end()) {
                if (miniMergeType(item) == FeedMergeReplace || 
                        miniMergeType(item) == FeedMergeAppend){
                    continue;
                }
                iss[it->second].push_back(item);
            } else {
                FeedItemSeq tmp;
                tmp.push_back(item);
                iss.push_back(tmp);
                stypemerge_index.insert(make_pair<pair<int, Ice::Long>, int>(
                            make_pair<int, Ice::Long>(verSmallType(item), item.merge), 
                            iss.size()-1));
            }
        }
        MCE_DEBUG("MergeFeed::merge, feed size: " << iss.size());
    }
private:
    inline int bigType(const FeedItem& item) const{
        return (int)((item.type & 0xFFFF) / 100) * 100;
    }
    inline int smallType(const FeedItem& item) const {
        return item.type & 0xFFFF;
    }
    inline int ver(const FeedItem& item) const {
        return (item.type & 0xFF0000) >> 16;
    }
    inline int verSmallType(const FeedItem& item) const {
        return item.type & 0xFFFFFF;
    }
    inline int newsMergeType(const FeedItem& item) const {
        return ((unsigned int)item.type) >> 30;
    }
    inline int miniMergeType(const FeedItem& item) const {
        return (item.type & 0x3FFFFFFF) >> 28;
    }
};
}
}
}

#endif  //_MERGEFEED_H_

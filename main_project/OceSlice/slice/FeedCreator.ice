#ifndef __FEED_CREATOR_ICE__
#define __FEED_CREATOR_ICE__

#include <Util.ice>
#include <RFeed.ice>

module xce
{
module feed
{
	exception NotAllowedToSendException extends MyUtil::MceException {};
	exception FeedTypeConfigNotFoundException extends MyUtil::MceException {};
	//exception Exception extends MyUtil::MceException {};

	class FeedCreator extends FeedAssistant{

		bool Create(int stype, int version, MyUtil::Str2StrMap props) throws MyUtil::MceException ;
		bool CreateWithConfig(int stype, int version, MyUtil::Str2StrMap props, MyUtil::Str2StrMap configs) throws MyUtil::MceException ;
		bool CreateWithReply(int stype, int version, MyUtil::Str2StrMap props, FeedReply reply) throws MyUtil::MceException ;
		bool CreateWithReplyAndConfig(int stype, int version, MyUtil::Str2StrMap props, FeedReply reply, 
                        MyUtil::Str2StrMap configs) throws MyUtil::MceException ;
                bool CreateEDM(int stype, int version, MyUtil::Str2StrMap props) throws MyUtil::MceException ;
		bool CreateWithReplace(int stype, int version, MyUtil::Str2StrMap props) throws MyUtil::MceException ;
		bool CreateWithReplaceAndConfig(int stype, int version, MyUtil::Str2StrMap props, MyUtil::Str2StrMap configs) throws MyUtil::MceException ;
                
		
		void ReloadAllConfig();
		void ReloadConfig(int stype);
	};


};
};
#endif

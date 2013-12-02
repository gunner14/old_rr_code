namespace cpp xoa2.demo.twitter
namespace java com.renren.xoa2.demo.twitter

include "../../serverlib/thrift-idl/renren.thrift"

enum TweetType {
    TWEET,
    RETWEET = 2,
    DM = 0xa,
    REPLY
}

struct Location {
    1: required double latitude;
    2: required double longitude;
}

struct Tweet {
    1: required i32 userId;
    2: required string userName;
    3: required string text;
    4: optional Location loc;
    5: optional TweetType tweetType = TweetType.TWEET;
    16: optional string language = "english";
}

typedef list<Tweet> TweetList

struct TweetSearchResult {
    1: TweetList tweets;
}

const i32 MAX_RESULTS = 100;

exception InvalidException {
  1: i32 code;
  2: string msg;
}

service TwitterService extends renren.RenrenService{
    void ping(1:i32 timeout);
    bool postTweet(1:Tweet tweet);
    TweetSearchResult searchTweets(1:string query) throws (1:InvalidException e);
    oneway void zip()
}


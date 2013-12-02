namespace cpp com.renren.xoa2.sns.badoo
namespace java com.renren.xoa2.sns.badoo

struct BadooLikeUserInfo{
    1:required i32 userId;
    2:required i32 badooUserId;
    3:required string name;
}

typedef list<BadooLikeUserInfo> BadooLikeInfoList

struct IdWrapper{
   1:required i32 id;
}

typedef list<IdWrapper> IdWrappers

struct BadooUserInfo{
   1:required string name;
   2:required i32 userId;
   3:required i32 badooUserId;
   4:required string mood;
   5:required i32 charm;
   6:required string tinyUrl;
   7:required i32 sex;
   8:required i64 birthDay;
   9:required i32 age;
   10:required string provinceName;
   11:required string cityName;
}

typedef list<BadooUserInfo> BadooUserInfoList

service BadooService{
   i32 getBadooIdByRId(1:i32 rId)
   i32 getRIdByBId(1:i32 bId)
   i32 sendGossip(1:i32 senderBId,2:i32 receiverBId,3:string message)
   BadooLikeInfoList getLikeUser(1:i32 rId,2:i32 limit)
   BadooLikeInfoList getBeLikeUser(1:i32 rId,2:i32 limit)
   BadooLikeInfoList getMutileLikeUser(1:i32 rId,2:i32 limit)
   BadooUserInfoList getUserBaseInfoByRId(1:IdWrappers rIds)
   BadooUserInfoList getUserBaseInfoByBId(1:IdWrappers bIds)
}

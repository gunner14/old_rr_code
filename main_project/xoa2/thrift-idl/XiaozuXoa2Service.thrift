namespace cpp com.renren.xoa2.sns.xiaozu
namespace java com.renren.xoa2.sns.xiaozu


struct XiaozuThread {
    1: required i64 id;
    2: required i64 forumId;
    3: required string forumName;
    4: required i32 threadType;
    5: required string threadTypeName;
    6: required string subject;
    7: required string body;
    8: required i64 createTime;
    9: required i32 createId;
    10: required string createName;
    11: required string createHead;
    12: required i32 replyCount;
    13: required i32 viewCount;
    14: required i32 shareCount;
    15: required i32 status;
}

struct XiaozuComment {
    1: required i64 id;
    2: required i64 authorId
    3: required string authorName;
    4: required string authorHead;
    5: required string authorFullHead;
    6: required i64 entryId;
    7: required i32 entryOwner;
    8: required string entryName;
    9: required string entryUrl;
    10: required string entrySummary;
    11: required string entryImage;
    12: required string entryVideo;
    13: required string entryIndentify;
    14: required string entryOwnerName;
    15: required string entryOwnerHead;
    16: required string entryOwnerFullHead;
    17: required string content;
    18: required string originalContent;
    19: required i32 delFlag;
    20: required i64 createTimeMillis;
}

typedef list<XiaozuComment> XiaozuCommentList
			
service XiaozuXoa2Service {
  bool postComment(1:i64 userId,2:i64 forumId,3:i64 threadId,4:string body,5:i32 toId)
  XiaozuThread getXiaozuThread(1:i32 forumId, 2:i32 threadId)
  XiaozuCommentList getCommentList(1:i64 forumId, 2:i64 threadId, 3:i32 page)
}

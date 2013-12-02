namespace cpp com.renren.xoa2.ugc.status
namespace java com.renren.xoa2.ugc.status


/**
 * 状态xoa异常
 */
exception XoaStatusException {
	1: i32 code;
 	2: string msg;
}

/**
 * 状态的错误信息，由于thrift对异常处理不太友好，所以单独把异常信息定义为一个数据结构
 */ 
struct Xoa2StatusErrorInfo{
	/**
	 * 错误码，每种错误对应一个值
	 */
	1: required i32 code;
	/**
	 * 错误信息
	 */
	2: optional string msg;
}

/**
 * lbs位置参数  
 */
struct LbsDataParam {
 	 /**
 	  * 用户id
 	  */
 	 1: required i32 userId;
 	 /**
 	  * poi's unique id   
 	  */
 	 2: required string pid;
 	 /**
 	  * 兴趣点 
 	  */
 	 3: required string poiName;
 	 /**
 	  * 地点名称 
 	  */
 	 4: required string locationName;
 	 /**
 	  * poi的经度  
 	  */
 	 5: required double longitudePoi;
 	 /**
 	  * poi的纬度  
 	  */
 	 6: required double latitudePoi;
 	 /**
 	  * gps 经度
 	  */ 
 	 7: required double longitudeGps;
 	 /**
 	  * gps 纬度
 	  */
 	 8: required double latitudeGps
 	 9: required i32 locateType;
 	 10:required i32 sourceType;
 	 /**
 	  * 用来判断，来自什么手机
 	  */
     11: required i32 appId;
     /**
      * 判断是手填POI信息
      */     
     12: required bool handWrite;
}

/**
 * 状态参数 
 */
struct DoingParam {
	/**
	 * 发状态人的id
	 */
	1: required i32 userId;
	/**
	 * 发状态的内容
	 */
	2: required string content;
	/**
	 * 被转发的状态id
	 */
	3: optional i64 fwdDoingId;
	/**
	 * 被转发的状态作者id
	 */
	4: optional i32 fwdUserId;
	/**
	 * ip地址 
	 */
	5: optional string ip;
	/**
	 * lbs信息  
	 */
	6: optional LbsDataParam lbsDataParam;
	/**
	 * 通用标志位  01:地理位置  10:wap位置 
	 */
	7: optional i32 markbit;
	/**
	 * type 来自的类型 如 TYPE_PHONE = 4; 手机   TYPE_LIVE_STREAM = 7; API产品的状态类型 等 
	 */
	8: optional i32 type;
	/**
	 * link 一些遗留的链接 如 某个应用的链接 
	 */
	9: optional string link;
	/**
	 * 表示来源，比如是哪个APP在调用 
	 */
	10: optional i32 source; 
}

/**
 * 状态控制参数 
 */
struct DoingControlParam {
 	 /**
 	 * 发否发新鲜事
 	 */
 	 1: optional bool needDispatchFeed = true;
 	 /**
 	  * 是否过审核
 	  */
 	 2: optional bool needAudit = false;
 	 /**
 	  * 是否过安全过滤
 	  */
 	 3: optional bool needAntispam = true;
 	 /**
 	  * 是否过频率查检
 	  */
 	 4: optional bool needCheckActivity = true;
 	 /**
 	  * 状态新鲜事的小尾巴 对应的链接
 	  */
 	 5: optional string Hlink;
 	 /**
 	  * 状态新鲜事小尾巴，对应的文案
 	  */
 	 6: optional string Htext;
 	 /**
 	  * 是否需要服务端处理转发内容的拼接，默认是true 需要，wap和mt，
 	  *如果转发状态的时候要对内容自己进行截断操作，那就传false
 	  */
 	 7: optional bool dealwithFwdContent=true;
}

/**
 * 发状态返回的结果
 */
struct DoingResult {
	/**
	 * 状态id
	 */
	1: i64 id;
	/**
	 * 状态所有者id
	 */
	2: i32 userId;
	/**
	 * 转换后的状态内容
	 */
	3: string htmlContent;
	/**
	 * 状态创建时间
	 */
	4: i64 creationTime; 
}

/**
 * 状态评论参数
 */
struct ReplyDoingParam {
	/**
	 * 状态的作者
	 */
	1: required i32 ownerId;
	/**
	 * 状态的id
	 */
	2: required i64 doingId; 
	/**
	 * 评论的作者
	 */
	3: required i32 replyerId;
	/**
	 * 评论的内容
	 */
	4: required string replyContent;
	/**
	 * 被回复的作者
	 */
	5: optional i32 replyToUserId;
	/**
	 * 被回复的回复的id
	 */
	6: optional i64 replyToCommentId;
	/**
	 * 评论的类型 区分回复的终端设备;
	 */
	7: optional i32 type; 
}

/**
 * 评论的来源
 * @author li-wei
 */
enum ReplyFromType {
	/**
	 * 网站
	 */
	SITE=0,
	/**
	 * 人人桌面
	 */
    IM=1,
    /**
     * Wap
     */
    WAP=2
}
/**
 * 状态评论控制参数 
 */
struct ReplyDoingControlParam {
 	 /** 
 	 * 发否发新鲜事
 	 */
 	 1: optional bool needDispatchFeed = true;
 	 /**
 	  * 是否发送提醒
 	  */
 	 2: optional bool needNotify = true;
 	 /**
 	  * 是否过安全过滤
 	  */
 	 3: optional bool needAntispam = true;
 	 /**
 	  * 是否过频率查检
 	  */
 	 4: optional bool needCheckActivity = true; 
 	 /**
 	  * ip 
 	  */
 	 5: optional string ip;
 	 /**
 	  * 评论来源，默认是网站
 	  *
 	  */
 	 6: optional ReplyFromType fromType=ReplyFromType.SITE;
}

/**
 * 发状态返回的结果
 */
struct ReplyDoingResult {
	/**
	 *评论的id
	 */
	1:i64 id;
	/**
	 * 状态的作者
	 */
	2: i32 ownerId;
	/**
	 * 状态的id
	 */
	3: i64 doingId; 
	/**
	 * 评论的作者
	 */
	4: i32 replyerId;
	/**
	 * 评论的内容
	 */
	5: string replyContent;  
	/**
	 * 状态作者姓名
	 */
	6: string ownerName;  
	/**
	 * 评论的作者姓名
	 */
	7: string replyerName;  
	/**
	 * 评论创建时间
	 */
	8: i64 creationTime; 
}

/**
 * 取删除的状态
 */
struct Xoa2DoingDel {
	/**
	 * 删除的状态id
	 */
	1: required i64 id;
	/**
	 * 发状态人的id
	 */
	2: required i32 userId;
	/**
	 * 发状态的内容
	 */
	3: required string content;
	/**
	 *状态的发布时间
	 */
	4: required i64 dtime;
	/**
	 * 状态的评论数
	 */
	5: optional i32 commentCount;
	/**
	 * type 来自的类型 如 YPE_NORMAL = 0；普通状态 TYPE_PHONE = 4; 手机   TYPE_LIVE_STREAM = 7; API产品的状态类型 等 
	 */
	6: optional i32 type;
	/**
	 * link 一些遗留的链接 如 某个应用的链接 
	 */
	7: optional string link;
	/**
	 * 表示来源，比如是哪个APP在调用 
	 */
	8: optional i32 source; 
	/**
	 * 删除状态用户的id
	 */
	9: optional i32 delUserid;
	/**
	 * 删除状态的时间
	 */
	10: optional i64 delTime; 
	/**
	 * 下一级转发数
	 */
	11: optional i32 repeatCount;
	/**
	 * 子节点转发的总数
	 */
	12: optional i32 repeatCountTotal;
	/**
	 * 是否是转发
	 */	
	13: optional bool isForward;
}

/**
 * 状态的lbs 位置信息
 * @author li-wei@renren-inc.com
 *
 */
struct Xoa2DoingLbsData{
	/**
 	 * id
 	 */
 	 1: optional i64 id;
 	 /**
 	  * 用户id
 	  */
 	 2: optional i32 userId;
 	 /**
 	  * poi's unique id   
 	  */
 	 3: optional string pid;
 	 /**
 	  * 兴趣点 
 	  */
 	 4: optional string poiName;
 	 /**
 	  * 地点名称 
 	  */
 	 5: optional string locationName;
 	 /**
 	  * poi的经度  
 	  */
 	 6: optional double longitudePoi;
 	 /**
 	  * poi的纬度  
 	  */
 	 7: optional double latitudePoi; 
}


/**
 * xoa2状态对应的model
 * @author li-wei@renren-inc.com
 *
 */
struct Xoa2Doing {
	/**
 	 * id
 	 */
    1: required i64 id;
    /**
     *发状态的人
     */
	2: required i32 userId;
    /**
     *状态的内容,老的格式，对于转发的状态新添加的内容将和根节点状态内容放一起
     */
	3: required string content;
    /**
     *评论数
     */
	4: required i32 commentCount
    /**
     *被直接转发数
     */
	5: required i32 repeatCount;
	/**
	 *当为根节点时，表示被（直接和间接）转发的总数
	 */
	6: required i32 repeatCountTotal;
    /**
     *通用标志位  01:地理位置  10:wap位置   
     */
	7: required i32 markbit;
	/**
	 *状态的发布时间
	 */
	8: required i64 doTime;
	9: required i32 type;
	10: required string link;
    /** 
     *表示来源，比如是哪个APP在调用 23137、 175894、 42 之类的  
     */
	11: required i32 source;
 	/**
     * 状态的lbs信息，默认不会装载这个数据，参考{@link GetDoingControlParam#needLbsData}
     */
	12: optional Xoa2DoingLbsData lbsData;
    /**
     * 转发的根节点
     */
    13: optional i64 rootId;
   /**
     * 状态的内容解析后的html，默认不会装载<br>
     * <li>{@link GetDoingControlParam#needHtmlContent}==true 会加载状态的htmlcontent</li>
     *<li>{@link GetDoingControlParam#need3GHtmlContent}==true 会加载3G用的htmlcontent</li>
     */
    14: optional string htmlContent;
    /**
     *vip 用户的标
     */
    15: optional string vipIconUrl;
    /**
     *  转发状态的跟节点的拥有者
     */
    16: optional i32 rootUserId;
    /**
     * 去掉跟节点内容的文本
     */
    17: optional string newContent;
    18: optional string rootContent;
    19: optional string rootNewContent;
    /**
     * 判断是否是转发
     */
    20: optional bool isForward;
}

/**
 * 状态的html格式
 * @author li-wei
 */
enum DoingHtmlType {
	/**
	 *不需要html
	 */
	NONE=0,
	/**
	 * 正常的网站用的格式
	 */
    HTML_NORMAL=1,
    /**
     * 3G 用的html格式
     */
    HTML_3G=2
}


/**
 * 状态控制参数 
 */
struct GetDoingControlParam {
 	 1: optional bool needLbsData =false;
 	 2: optional bool needRoot =false;
 	 /**
 	  * 默认就是不需要
 	  */
 	 3: optional DoingHtmlType htmlType=DoingHtmlType.NONE;
}

/**
 * 用户状态计数
 */
struct Xoa2DoingCount{
 	1: required i32 userId;
 	2: required i32 doingCount;
 	3: required i32 replyDoingCount; 
}

/**
 * 状态回复
 */
struct Xoa2ReplyDoing{
	
	1: i64 id;
	2: i64 doingId;
	3: i32 ownerId;
	4: string ownerName;
	5: i32 replyerId;
	6: string replyContent;
	7: string replyerName;
	9: string replyerTinyurl;
	10: i64 replyTime;
	/**
	 *区分回复的终端设备; 1：手机-wap，0：pc-web
	 */
	11: i32 type;
}

/**
 * 组装状态评论内容的配置
 */
struct ReplyContentControlParam{
	/**
	 *是否要替换ubb
	 */
	1:bool needReplaceUbb=false;
	/**
	 *是否需要替换@链接
	 */
	2:bool needReplaceMention=false;
	/**
	 * 是否需要替换短链接
	 */
	3:bool needReplaceShortUrl=false;
}

/**
 * uub表情
 */
struct Xoa2Ubb{
	/**
	 *数据库中存储的id
	 */
	1: i32 id;
	/**
	 * 要替换的内容
	 */
	2: string ubb;
	/**
	 *替换后图片的url
	 */
	3: string src;
	/**
	 *替换后图片的alt
	 */
	4: string alt;
	/**
	 * 图片的显示顺序
	 */
	5: i32 position;
	
}

/**
 *取状态的查询参数
 */
struct QueryDoingParam{
	/**
	 * 用户id
	 */
	1:required i32 userId;
	/**
	 * 与mysql的offset一样
	 */
	2:optional i32 offset=0;
	/**
	 * 与mysql的limit意义一样
	 */
	3:optional i32 limit=10; 
}

/**
 * 取状态回复的查询参数
 */
struct QueryReplyParam{
	1:required i32 userId;
	2:required i64 doingId;
	3:optional i32 offset=0;
	4:optional i32 limit=10;
	5:optional bool isAsc=true;
}	

/** 
 * 状态Id 和 用户id的元组
 */
struct Doingtuple{
	1: i64 doingId;
	2: i32 userId;
}

/**
 *  xoa2接口返回bool值的结果
 */
struct BooleanResult{
	/**
	 * 返回的boolean值
	 */
	1:optional bool result;
	/**
	 * 错误信息
	 */
	2: optional Xoa2StatusErrorInfo errorInfo;
}



/**
 * xoa2接口返回long类型值（i64）的结果
 */
struct LongResult{
	/**
	 * 返回值
	 */
	1:optional i64 result;
	/**
	 * 错误信息
	 */
	2: optional Xoa2StatusErrorInfo errorInfo;
}

typedef list<Xoa2ReplyDoing> Xoa2ReplyDoingList
typedef list<string> StringList
typedef list<Xoa2Ubb> Xoa2UbbList
typedef list<Doingtuple> DoingtupleList
typedef map<i64,Xoa2Doing> DoingMap


typedef list<Xoa2Doing> Xoa2DoingList
typedef list<Xoa2DoingDel> Xoa2DoingDelList

/**
 * 当接口需要返回Xoa2Doing时，就用这个作为返回值
 */
struct Xoa2DoingResult{
	1: optional Xoa2Doing xoa2Doing;
	2: optional Xoa2StatusErrorInfo errorInfo;
} 

/**
 * 当接口返回Xoa2ReplyDoingList时，用这个作为返回值
 */
struct Xoa2ReplyDoingListResult{
	1: optional Xoa2ReplyDoingList replyDoingList;
	2: optional Xoa2StatusErrorInfo errorInfo;
}

/**
 * 当接口返回Xoa2UbbList时，用这个作为返回值
 */
struct Xoa2UbbListResult{
	1:optional Xoa2UbbList xoa2UbbList;
	2: optional Xoa2StatusErrorInfo errorInfo;
}
/**
 * 当接口返回Xoa2ReplyDoing时，用这个作为返回值
 */
struct Xoa2ReplyDoingResult{
	1:optional Xoa2ReplyDoing xoa2ReplyDoing;
	2:optional Xoa2StatusErrorInfo errorInfo;
}

/**
 * 当接口返回Xoa2DoingList时，用着这个作为返回值，带上异常信息。
 */
struct Xoa2DoingListResult{
	1:optional Xoa2DoingList xoa2DoingList;
	2:optional Xoa2StatusErrorInfo errorInfo;
}

/**
 * ubb表情的service
 * @author li-wei
 */
service UbbService {
	 
	/**
	 * 获取线上当前显示用的状态表情，已废弃
	 */
	Xoa2UbbList getDoingUbbList() throws (1:XoaStatusException e);
	
	/**
	 * 获取所有状态表情，已废弃
	 */
	Xoa2UbbList getAllDoingUbbList()  throws (1:XoaStatusException e);
	
	/**
	 * 根据分类取状态的ubb表情
	 * @param type 表情所属的分类 
	 *	 		
	 */
	Xoa2UbbListResult getDoingUbbsBytypeNE(1:i32 type);
	
	/**
	 * 获取所有状态表情，包含已经下线的状态ubb表情
	 */
	Xoa2UbbListResult getAllDoingUbbListNE();
}



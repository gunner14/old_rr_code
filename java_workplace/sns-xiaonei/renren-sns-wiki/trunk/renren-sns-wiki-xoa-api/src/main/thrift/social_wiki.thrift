namespace java com.renren.sns.wiki.xoa

/**
* UGC 类型
*/
enum UGCType {
	DOING = 1,
	DOING_COMMENT = 2,
	BLOG = 3,
	BlOG_COMMENT = 4,
	PHOTO = 5,
	PHOTO_COMMENT = 6,
	SHARE = 7,
	SHARE_COMMENT = 8,
	ALBUM = 9,
	ALBUM_COMMENT = 10
}

/**
* wiki状态
*/
enum WikiState {
	OPEN = 0,
	PAUSE = 1,
	CLOSE = 10,
	FORBIDDEN =11,
}

/**
* wiki切词解析参数
*/
struct ParseContext {
	/**
	* 热词存储时的key
	*/
	1: optional string memcacheKey,
	
	/**
	* 需要切词的文本内容
	*/
	2: required list<string> texts,
	
	/**
	* ugcId,如日志则为blogId
	*/
	3: required i64 ugcId,
	
	/**
	* UGC类型
	*/
	4: required UGCType ugcType,
	
	/**
	* ugc owner id
	*/
	5: optional i32 ownerId,
	
	/**
	* 用户ip
	*/
	6: optional string ip
}

/**
* wiki切词解析结果
*/
struct ParseResult {
	/**
	* wiki id 列表
	*/
	1: list<i32> wikiIds,
	/**
	* 匹配上的wiki热词数据
	*/
	2: string wikiWords
}

/**
* 个人主页展示wiki模块
*/
struct WikiProfile {
	/**
	* 是否profile在profile页展示
	*/
	1: required bool profileUser,
	/**
	* 想看电影的数目
	*/
	2: optional i32 wishMovieCount
	/**
	* 看过电影的数目
	*/
	3: optional i32 collectMovieCount
}

/**
* Wiki 解析服务
*/
service WikiParserService {
	
	/**
	* UGC创建时调用该服务对UGC进行切词，返回wiki ids和切词结果
	*/
	ParseResult parseText(1:ParseContext context),

	/**
	* UGC更新时调用该服务
	*/
	void asyncUpdateParseResult(1:ParseContext context, 2:string privacy ),
	
	/**
	* UGC终端页展示时如果读取miss时，则启动该服务异步加载
	*/
	void asyncReloadResult(1:ParseContext context)
	
	/**
	* 批量的reload接口。UGC终端页展示时如果读取miss时，则启动该服务异步加载
	*/
	void asyncBatchReloadResult(1:list<ParseContext> contexts)
}

service WikiXoaService extends WikiParserService {
	
	/**
	* 将简评清空
	*/
	void deleteSimpleComment(1:i32 wikiId, 2:i32 userId)
	
	/**
	* 封禁wiki
	*/
	bool forbiddenWiki(1:i32 wikiId)
	
	/**
	* 暂停wiki
	*/
	bool pauseWiki(1:i32 wikiId)
	
	/**
	* 开启wiki
	*/
	bool reopenWiki(1:i32 wikiId)
	
	/**
	* 获取Wiki状态
	*/
	WikiState getWikiState(1:i32 wikiId)
	
	/**
	* 获取个人主页展示wiki相关信息
	*/
	WikiProfile getWikiProfile(1:i32 userId)
	
}
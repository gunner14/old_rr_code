namespace java com.renren.xoa2.quota.cache

const i32 EXPIRE_SECOND = 1;
const i32 EXPIRE_MINUTE = 60;
const i32 EXPIRE_HOUR = 3600;
const i32 EXPIRE_DAY = 86400;
const i64 ERROR_COUNT = -1;

exception QuotaCacheException {
  1: i32 code;
  2: string msg;
}

service QuotaCache {
	/*删除某个key*/
	bool remove(1:string key);
	
	/*添加key到cache中，并赋值为value，过期时间为expireTime*/
	void setLong(1:string key, 2:i64 value, 3:i32 expireTime)throws (1:QuotaCacheException e);
	
	/*获取某个key对应的计数值*/
	i64 getLong(1:string key) throws (1:QuotaCacheException e);
	
	/*增加某个key对应的计数值增加value*/
	i64 incLong(1:string key, 2:i64 value) throws (1:QuotaCacheException e);
	
	/*取某个key对应的ip集合*/
	set<string> getIps(1:string key);

	/*将ip添加到key对应的键值对中*/
	void putIp(1:string key,2:string ip, 3:i32 expireTime) throws (1:QuotaCacheException e);
	
	/*取某个key对应到single quota使用情况*/
	i64 getSingleQuotaUsed(1:string level, 2:string serviceId, 3:string versionId, 4:string clientId, 5:string ip) throws (1:QuotaCacheException e);
	
	/*取某个key对应到total quota使用情况*/
	i64 getTotalQuotaUsed(1:string level, 2:string serviceId,3:string versionId, 4:string clientId) throws (1:QuotaCacheException e);
}


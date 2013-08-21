package com.xiaonei.platform.core.usercount;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import com.xiaonei.xce.usercount.UserCountAdapter;
import com.xiaonei.xce.usercount.UserCountData;

/**
 * 
 * 用户计数的管理器
 * 
 * @author zhicali.li
 * 
 */
public class UserCountMgr {

	//private static final Logger logger = Logger.getLogger(UserCountMgr.class);

	public static final int ONLINE_FRIENDS = 0;
	public static final int POKE = 1;
	public static final int CLASS_INVITE = 2;
	public static final int GUEST_REQUEST = 3;
	public static final int FRIEND_MEMO_REQUEST = 4;
	public static final int NEW_MESSAGE = 5;
	public static final int IM_LEAVE_WORD = 6;
	public static final int TEAM_INVITE = 7;
	public static final int TRIBE_REQUEST = 8;
	public static final int FRIEND_COUNT = 9;
	public static final int REG_APPLY = 10;
	public static final int RECOMMENDED_FRIEND = 11;
	public static final int NOTIFY = 12;
	public static final int EVENT_INVITE = 13;
	public static final int APP_INVITE = 14;
	public static final int PHOTO_TAG_INVITE = 15;
	public static final int SURVEY_INVITE = 16;
	public static final int GAME_INVITE = 17;
	public static final int TEST_INVITE = 18;
	public static final int CALL_INVITE = 19;
	public static final int NEW_REQUEST = 20;
	public static final int CONTACT_REQUEST = 21;
	public static final int OFFER_MSN_CONTACT = 22;
	public static final int TOTAL_TYPE_SIZE = 23;	
	public static final int PAGE_INVITE = 24;
	public static final int FOLLOW_REQUEST = 25;
	public static final int FOLLOW_TA = 26;
	public static final int TA_FOLLOW = 27;
    public static final int GAME_HALL_INVITE = 29;  //游戏大厅邀请
    public static final int PAGE_ADMIN_INVS = 30;  // 俱乐部管理员邀请(erxi.qian@opi-corp.com)
    public static final int PAGE_CLUB_INVITES = 31;  // 俱乐部成员邀请(erxi.qian@opi-corp.com)
    public static final int VAS_VIP_REQUEST = 32;  // VIP赠送请求(lan.li@opi-corp.com)
    public static final int VAS_PROFILE_TEMPLATE_REQUEST = 33;  // 装扮赠送请求(lan.li@opi-corp.com)
	public static final int PAGE_PHOTO_TAG_INVITE = 34;	//page中的图片圈人请求
    public static final int PAGE_LOVE_ADMIN_INVS = 35;	//邀请XXX成为page情侣空间的管理员
	public static final int AD_NOTIFY_COUNT = 36;	//中间层AD_NOTIFY服务使用的count
    public static final int NUOMI_INVITE = 37;	//人人用户收到糯米邀请请求数
    public static final int FORUM_INVITE = 38;	//论坛邀请
    public static final int PAGE_LOVE_MOODS_INVITE = 39; 	//侣空间管理员向好友发送加人气的请求
    public static final int APP_FREE_GIFT = 40; 	//APP免费礼物，开放平台使用，2010-09-06添加
    
    public static final int XIAOZU_INVITE = 41;	//小组邀请，2010-9-17添加 
    public static final int XIAOZU_MEMBER = 42;	//有新成员申请加入小组的小组数，2010-10-18添加
    
    public static final int ADDRESS_LIST_REQUEST = 43; //通讯录请求，2010-10-10添加

    
    public static final int FRIEND_TYPE_UNREAD = 44;	//请求中心使用，2011-1-13添加
    public static final int REQUEST_TYPE_UNREAD = 45;	//请求中心使用，2011-1-13添加
    public static final int MESSAGE_TYPE_UNREAD = 46;	//请求中心使用，2011-1-13添加
    
    public static final int PAGE_ADMIN_INVITE = 47;	//请求中心使用，2011-08-18添加
    public static final int HOT_AD = 48;	//请求中心使用，热点动态，2011-08-18添加
     
    public static final int XIAOZHAN_INVITE = 49;	//小站推荐请求，点击小站的推荐，就会发送一个请求类型。 2011-10-11添加	2011-10-11添加
    public static final int APP_INVITE_REQ = 50;	//新版请求中心中的应用请求，开放平台推荐应用的时候，会发送一个请求类型。 2011-10-11添加
    
    public static final int UGC_STATUS = 51;//新版Publisher使用,用户状态数
    public static final int UGC_SHARE = 52;//新版Publisher使用,用户分享数
    public static final int UGC_PHOTO = 53;//新版Publisher使用,用户PHOTO相关数据
    public static final int UGC_BLOG = 54;//新版Publisher使用,用户日志数
    
    
    public static final int XIAOZHAN_CREATE_INVITE = 55;	//小站创建邀请，2011-11-11添加
    
    public static final int MSG_UNREAD_SESSION = 56;	//通讯平台，未读的群会话数，2011-12-07添加
    
    /*
     * 从60开始的一段预留给page使用
     */
    public static final int PAGE_CREATE_INVITE = 60;	//Page创建邀请，2011-10-11添加
    
    public static final int APP_RELATE_PAGE = 61;	//和app关联的page，属于配置业务使用的计数类型，2011-12-12添加
    
	/**
	 * 这是一个丑陋的hack，将所有的新版app请求计数加起来，然后用这个type返回给上层调用者，比如首页，因为他们要显示
	 * 一个计数的总和，而不是像facebook那样。
	 * 需求的出入，于是有了这个丑陋的hack，希望有一天，我能亲手将它改掉。。。
	 * 
	 * 千万记住这并不是数据库中真实的一个类型，而且在内存中运算、设置的
	 * 
	 */
	public static final int APP_REQUEST_TOTAL = 28;
	
	/**
	 * app类型下的子类型——app请求
	 */
	public static final int APP_TYPE_REQUEST = 0;
	
	/**
	 * 默认的计数器增加值，一般来说，一次只需增加 1
	 */
	public final static int DEFAULT_INC_STEP = 1;

	/**
	 * 增加指定类型的计数 +1
	 * 
	 * @param uid
	 *            用户的ID
	 * @param appId
	 *            APP的ID
	 * @param type
	 *            需要计数的类型，是业务的类型 type值预留了100个，从0到99，可根据业务的需求酌情添加 目前 0 表示 邀请安装
	 * 
	 */
	public static void incAppCount(int uid, int appId, int type) {
		incAppCount(uid, appId, type, DEFAULT_INC_STEP);
	}

	/**
	 * 计数器增加指定长度
	 * 
	 * @param uid
	 * @param appId
	 * @param type
	 * @param step
	 *            一次增加的步长
	 */
	public static void incAppCount(int uid, int appId, int type, int step) {
		if (step < 0) {
			throw new IllegalArgumentException("step should be positive: " + step);
		}
		// step = step <= 0 ? DEFAULT_INC_STEP : step;
		int ucType = calcAppCountType(appId, type);
		try {
			UserCountAdapter.getInstance().inc(uid, ucType, step);
		} catch (Exception e) {
			e.printStackTrace();
			//logger.error("UserCountMgr==> 调用usercount服务增加计数失败  " + e.getMessage());
		}
	}

	/**
	 * 减少计数
	 * 
	 * @param uid
	 * @param appId
	 * @param type
	 */
	public static void decAppCount(int uid, int appId, int type) {
		decAppCount(uid, appId, type, DEFAULT_INC_STEP);
	}

	/**
	 * @param uid
	 * @param appId
	 * @param type
	 * @param step
	 *            一次增加的步长
	 */
	public static void decAppCount(int uid, int appId, int type, int step) {
		if (step < 0) {
			throw new IllegalArgumentException("step should be positive: " + step);
		}
		int ucType = calcAppCountType(appId, type);
		try {
			UserCountAdapter.getInstance().dec(uid, ucType, step);
		} catch (Exception e) {
			e.printStackTrace();
			//logger.error("UserCountMgr==>  调用usercount服务减少计数失败  " + e.getMessage());
		}
	}

	/**
	 * 设置计数器的值为count
	 * 
	 * @param uid
	 * @param appId
	 * @param type
	 * @param count
	 */
	public static void setAppCount(int uid, int appId, int type, int count) {
		if (count < 0) {
			throw new IllegalArgumentException("count should be positive: " + count);
		}
		int ucType = calcAppCountType(appId, type);
		try {
			UserCountAdapter.getInstance().set(uid, ucType, count);
		} catch (Exception e) {
			e.printStackTrace();
			//logger.error("UserCountMgr==> 调用usercount服务重置计数失败  " + e.getMessage());
		}
	}

	/**
	 * 获取某种类型的数据
	 * 
	 * @param uid
	 * @param appId
	 * @param type
	 * @return
	 */
	public static int getAppCount(int uid, int appId, int type) {
		int ucType = calcAppCountType(appId, type);
		try {
			return UserCountAdapter.getInstance().get(uid, ucType);
		} catch (Exception e) {
			e.printStackTrace();
			//logger.error("UserCountMgr==> 调用usercount服务获取计数失败  " + e.getMessage());
		}
		return -1;
	}

	/**
	 * 获得所有APP的业务的数据
	 * 
	 * @param uid
	 * @return
	 */
	public static List<AppCount> getAppCounts(int uid) {
		UserCountData data = null;
		try {
			data = UserCountAdapter.getInstance().getAll(uid);
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (data == null) {
			return null;
		}
		int types[] = data.types;
		int values[] = data.values;
		List<AppCount> list = new ArrayList<AppCount>();
		for (int i = 0; i < types.length; i++) {
			if (types[i] >= 100) {	//>=100的类型才属于app的类型
				int type = getBusinessType(types[i]);
				int appId = getAppIdFromType(types[i]);
				int count = values[i];
				list.add(new AppCount(uid, appId, type, count));
			}
		}
		return list;
	}

	/**
	 * 把指定用户的指定类型的app请求计数全部清零
	 * @param uid 用户ID
	 * @param type app请求类型
	 * 
	 * @return 清零了多少个不同app的计数
	 */
	public static int clearAppCounts(int uid, int type) {
		List<AppCount> counts = getAppCounts(uid);	//取用户所有的app请求计数
		int clearCount = 0;
		for (AppCount ac : counts) {	//遍历
			if (ac.getType() == type) {	//类型匹配
				setAppCount(uid, ac.getAppId(), type, 0);
				clearCount ++;
			}
		}
		return clearCount;
	}
	
	/**
	 * 得到所有app的类型为"请求"的计数总和
	 * @param uid 用户id
	 * @return
	 */
	public static int getTotalAppReqCount(int uid) {
		return getTotalAppCount(uid, APP_TYPE_REQUEST);
	}
	
	/**
	 * 得到所有app某种类型的计数的总和
	 * 
	 * @param uid
	 * @param type
	 * @return
	 */
	public static int getTotalAppCount(int uid, int type) {
		List<AppCount> counts = getAppCounts(uid);
		int total = 0;
		for (AppCount ac : counts) {
			if (ac.getType() == type) {
				total += ac.getCount();
			}
		}
		return total;
	}
	
	/**
	 * 通过自定义type值获得appId
	 * 
	 * @param ucType
	 *            usercount服务返回的自定义type值
	 * @return
	 */
	public static int getAppIdFromType(int type) {
		if (type >= 100) {
			return (int) type / 100;
		} else {
			throw new IllegalArgumentException("Bad type: " + type);
		}
	}

	/**
	 * 根据usercount服务返回的类型值 获取对应的业务类型
	 * 
	 * @param type
	 * @return
	 */
	public static int getBusinessType(int type) {
		if (type < 0) {
			throw new IllegalArgumentException("Bad type: " + type);
		}
		return type % 100;
	}

	/**
	 * 将指定类型的计数器增加 1
	 * 
	 * @param uid
	 * @param type
	 *            传统业务的类型，比如通知、好友申请等，值为0--99
	 */
	public static void incCount(int uid, int type) {
		incCount(uid, type, DEFAULT_INC_STEP);
	}

	/**
	 * @param uid
	 * @param type
	 * @param step
	 *            增加的步长
	 */
	public static void incCount(int uid, int type, int step) {
		isTypeIllegal(type);
		if (step < 0) {
			throw new IllegalArgumentException("step should be positive: " + step);
		}
		try {
			UserCountAdapter.getInstance().inc(uid, type, step);
		} catch (Exception e) {
			e.printStackTrace();
			//logger.error("UserCountMgr==> 调用usercount服务增加计数失败  " + e.getMessage());
		}
	}

	/**
	 * @param uid
	 * @param type
	 */
	public static void decCount(int uid, int type) {
		decCount(uid, type, DEFAULT_INC_STEP);
	}

	/**
	 * @param uid
	 * @param type
	 * @param step
	 */
	public static void decCount(int uid, int type, int step) {
		isTypeIllegal(type);
		if (step < 0) {
			throw new IllegalArgumentException("step should be positive: " + step);
		}
		try {
			UserCountAdapter.getInstance().dec(uid, type, step);
		} catch (Exception e) {
			e.printStackTrace();
			//logger.error("UserCountMgr==> 调用usercount服务减少计数失败  " + e.getMessage());
		}
	}

	/**
	 * @param uid
	 * @param type
	 * @param count
	 */
	public static void setCount(int uid, int type, int count) {
		isTypeIllegal(type);
		if (count < 0) {
			throw new IllegalArgumentException("step should be positive: " + count);
		}
		try {
			UserCountAdapter.getInstance().set(uid, type, count);
		} catch (Exception e) {
			e.printStackTrace();
			//logger.error("UserCountMgr==> 调用usercount服务重置计数失败  " + e.getMessage());
		}
	}

	public static int getCount(int uid, int type) {
		isTypeIllegal(type);
		try {
			return UserCountAdapter.getInstance().get(uid, type);
		} catch (Exception e) {
			e.printStackTrace();
			//logger.error("UserCountMgr==> 调用usercount服务获取计数失败  " + e.getMessage());
		}
		return 0;
	}

	/**
	 * 取user count，并以List结构返回。
	 * @param uid 用户id
	 * @return
	 */
	public static List<UserCount> getCounts(int uid) {
		
		Map<Integer, Integer> countMap = getCountMap(uid);
		List<UserCount> list = new ArrayList<UserCount>(countMap.size());
		for (Entry<Integer, Integer> entry : countMap.entrySet()) {
			list.add(new UserCount(uid, entry.getKey(), entry.getValue()));
		}
		return list;
	}
	
	/**
	 * 取user count，并以Map结构返回。Map的key是type，value是count数值
	 * @param uid
	 * @return
	 */
	public static Map<Integer, Integer> getCountMap(int uid) {
		UserCountData data = null;
		try {
			data = UserCountAdapter.getInstance().getAll(uid);
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (data == null) {
			return null;
		}
		int types[] = data.types;
		int values[] = data.values;
		Map<Integer, Integer> map = new HashMap<Integer, Integer>();
		int appTotalReqCount = 0;	//app请求计数总和
		for (int i = 0; i < types.length; i++) {
			if (types[i] >= 100 && getBusinessType(types[i]) == APP_TYPE_REQUEST) {//app请求, 首页要显示总数，所以在这里加一下，伪造成一种系统计数类型，这是丑陋的解决方案
				appTotalReqCount += values[i];
			}
			map.put(types[i], values[i]);
		}
		if (appTotalReqCount >= 0) {	//app请求的总数用APP_REQUEST_TOTAL来代表
			map.put(APP_REQUEST_TOTAL, appTotalReqCount);
		}
		return map;
	}
	

	/**
	 * 根据appId和type值获取usercount服务的类型， 计算公式为 appId*100+type eg. appId 为
	 * 19974发出的安装邀请（type=0），其usercount服务的类型值为 1997400
	 * 
	 * @param appId
	 * @param type
	 * @return
	 */
	private static int calcAppCountType(int appId, int type) {
		isTypeIllegal(type);
		if ((Integer.MAX_VALUE - type) / 100 < appId) {
			//logger.error("UserCountMgr==> appId的值过大，appId=" + appId);
			throw new IllegalArgumentException();
		} else {
			return appId * 100 + type;
		}
	}

	/**
	 * 判断type的值是否合法,如果不合法，则抛异常，让使用者自己处理
	 * 
	 * @param type
	 * @return
	 */
	private static boolean isTypeIllegal(int type) {
		if (type < 0 || type > 99) {
			//logger.error("UserCountMgr==> type 的值应该是0--99");
			throw new IllegalArgumentException();
		} else {
			return true;
		}
	}
}

package com.xiaonei.login.client;

import java.util.HashMap;

public class Config {
	// 这个map里存放了需要返回ticket和升级信息的app_id
	// 如果增加一个客户端就需要修改这里的配置，表面上看起来似乎很麻烦，但其实这种方式成本很低，因为增加客户端不是很频繁的
	// 前面那个Integer表示appId，后面那个表示登录类型，没有放到map中的暂时不记录，等有需求时再记录
	public static final HashMap<Integer, Integer> phoneclientMap = new HashMap<Integer, Integer>();

	// J2ME手机客户端1.0
	public static final String J2ME_PHONECLIENT_APIKEY = "7de0de360a6d416b9d4905a996eda574";
	public static final String J2ME_PHONECLIENT_SECRET = "53c722c865bd4ae18c0d1acd4bb0e2c5";
	public static final int J2ME_PHONECLIENT_APP_ID = 23534;
	public static final int J2ME_LOGIN_TYPE = 2;
	// J2ME手机客户端2.0
	public static final String J2ME_PHONECLIENT2_APIKEY = "db1ffe772f504e9790907be9c9a651c7";
	public static final String J2ME_PHONECLIENT2_SECRET = "66c9ef1ef89c4b82b09e7a5d4eb6121b";
	public static final int J2ME_PHONECLIENT2_APP_ID = 38281;
	public static final int J2ME_LOGIN_TYPE2 = J2ME_PHONECLIENT2_APP_ID;

	// iPhone客户端
	public static final String IPHONE_PHONECLIENT_APIKEY = "1a731696a6f64ddda8ca21340bd2678d";
	public static final String IPHONE_PHONECLIENT_SECRET = "6d5ead3983554fcca270b866f9d17cd4";
	public static final int IPHONE_PHONECLIENT_APP_ID = 29384;
	// public static final int IPHONE_LOGIN_TYPE = 5;
	public static final int IPHONE_LOGIN_TYPE = IPHONE_PHONECLIENT_APP_ID;
	// S60客户端
	public static final String S60_PHONECLIENT_APIKEY = "2d7b775acba54c7e8a47d00c9304abf3";
	public static final String S60_PHONECLIENT_SECRET = "ab292a7b0ccb4eabb4259607eac29f00";
	public static final int S60_PHONECLIENT_APP_ID = 29385;
	// public static final int S60_LOGIN_TYPE = 6;
	public static final int S60_LOGIN_TYPE = S60_PHONECLIENT_APP_ID;
	// Android
	public static final String ANDROID_PHONECLIENT_APIKEY = "d0cc1f915bf5410bbde201504ed49ee2";
	public static final String ANDROID_PHONECLIENT_SECRET = "42d252a5991847dd99e98b88d9d9f10d";
	public static final int ANDROID_PHONECLIENT_APP_ID = 41301;
	// public static final int ANDROID_LOGIN_TYPE = 6;
	public static final int ANDROID_LOGIN_TYPE = ANDROID_PHONECLIENT_APP_ID;

	/** 邀请小工具，目前此工具只调用登录和uploadContact两个接口,为便于区别和统计，所以将它与客户端其他接口分开 */
	public static final String INVITE_TOOL_APIKEY = "f3178536dfaf408795fef54809896896";
	public static final String INVITE_TOOL_SECRET = "f169fc44a592467f81e2337ecb24f03f";
	public static final int INVITE_TOOL_APP_ID = 21985;
	public static final int INVITE_TOOL_LOGIN_TYPE = 4;

	// public static final List<Integer> list = new ArrayList<Integer>();

	static {
		phoneclientMap.put(J2ME_PHONECLIENT_APP_ID, J2ME_LOGIN_TYPE);
		phoneclientMap.put(J2ME_PHONECLIENT2_APP_ID, J2ME_LOGIN_TYPE2);
		phoneclientMap.put(INVITE_TOOL_APP_ID, INVITE_TOOL_LOGIN_TYPE);
		phoneclientMap.put(IPHONE_PHONECLIENT_APP_ID, IPHONE_LOGIN_TYPE);
		phoneclientMap.put(S60_PHONECLIENT_APP_ID, S60_LOGIN_TYPE);
		// list.add(J2ME_PHONECLIENT_APP_ID);
		// list.add(IPHONE_PHONECLIENT_APP_ID);
		// list.add(S60_PHONECLIENT_APP_ID);
		// list.add(INVITE_TOOL_APP_ID);
	}

	public static void main(String[] args) {
		// 性能测试结果：用map比list性能要搞
		int key = 1231654;
		// long time1=System.currentTimeMillis();
		// for(int i=0;i<10000000;i++){
		try {
			key = phoneclientMap.get(key);
		} catch (Exception e) {
		}
		System.err.println(key);
		// }
		// long time2=System.currentTimeMillis();
		// for(int i=0;i<10000000;i++){
		// list.contains(key);
		// }
		// long time3=System.currentTimeMillis();
		// System.err.println("map : "+(time2-time1)+" list : "+(time3-time2));
	}

}

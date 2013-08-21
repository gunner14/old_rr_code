package com.xiaonei.reg.guide.util;

import java.util.Date;

import com.xiaonei.xce.log.XceLogger;
import com.xiaonei.xce.log.XceLoggerAdapter;

/**
 * @author xin.yin@opi-corp.com
 * @since 2010-10-25
 */
public class GuideRemoteLogger {
	private final static String[][] CATEGORIES = 
		new String[][] { 
		{"ActiveTrackXN", "action", "bucket_0"},
		{"ActiveTrackXN", "action", "bucket_1"},
		{"ActiveTrackXN", "action", "bucket_2"},
		{"ActiveTrackXN", "action", "bucket_3"},
		{"ActiveTrackXN", "action", "bucket_4"},
		{"ActiveTrackXN", "action", "bucket_5"},
		{"ActiveTrackXN", "action", "bucket_6"},
		{"ActiveTrackXN", "action", "bucket_7"},
		{"ActiveTrackXN", "action", "bucket_8"},
		{"ActiveTrackXN", "action", "bucket_9"},
		};
	private final static String MSG_SEPARATOR = " ";
	private final static String KEY = "GUIDEZIP";
	
	public static final String TYPE_REQUESTMULTFIRENDS = "REQFRDS_";
	public static final String TYPE_REQUESTFIREND = "REQAFRD_";
	public static final String TYPE_GETOFFERFIREND = "OFFEFRD_";
	public static final String TYPE_GETPROFILEFIREND = "PROFFRD_";
	
	public static GuideRemoteLogger obj = new GuideRemoteLogger();

	private GuideRemoteLogger() {

	}

	public static GuideRemoteLogger getInstance() {
		return obj;
	}


	public void log(int owner, String type,String action) {
		try {
			XceLogger myLogger = XceLoggerAdapter.getLogger("M@XceLogger0");
			//myLogger.addLog(CATEGORIES, new StringBuilder(KEY).append(MSG_SEPARATOR).append(owner).append(MSG_SEPARATOR).append(type).append(MSG_SEPARATOR).append(appId).append(MSG_SEPARATOR).append(action).toString());
			long TIME = new Date().getTime();
			String msg = new StringBuilder().append(TIME).append(MSG_SEPARATOR).append(owner).append(MSG_SEPARATOR).append(KEY).append(MSG_SEPARATOR).append(type).append(action).toString();
			myLogger.addLog(CATEGORIES[owner % 10], msg);
		} catch (Exception e) {
			System.err.print("GuideRemoteLogger::log() e:"+e.toString());
		}
	}
	
	public static void main(String[] args) {
		//System.out.println(GuideRemoteLogger.getInstance().contructorStr(1, 1, 1, "test"));
		long TIME = new Date().getTime();
		int owner = 81120;
		String type = TYPE_REQUESTMULTFIRENDS;
		String reasonflag = "just_a_test";
		String [] ids = {};
		String action = "["+reasonflag+"]_"+ids.length+"";
		String msg = new StringBuilder().append(TIME).append(MSG_SEPARATOR).append(owner).append(MSG_SEPARATOR).append(KEY).append(MSG_SEPARATOR).append(type).append(action).toString();
		System.out.println(msg);
		GuideRemoteLogger.getInstance().log(81120, type, action);
		//String u = "p=[{\\x22t\\x22:\\x22univ\\x22,\\x22id\\x22:\\x22-1\\x22,\\x22name\\x22:\\x22%E5%8C%97%E4%BA%AC%E7%94%B5%E5%BD%B1%E5%AD%A6%E9%99%A2\\x22,\\x22year\\x22:\\x220\\x22}]";
		//System.out.println(URLDecoder.decode(u).replace("\\x22", "\""));
		
	}
}

package com.renren.xoa2.quota.cache;


/**
 * Quota Cache Service的XOA2 client
 * @author Xue Wenxin Nov 12, 2012 6:33:16 PM
 */
public class QuotaCacheConstants {
	
	/**	 * Expiration, a second	 */
	public static final int EXPIRE_SECOND = 1;
	
	/**	 * Expiration, a minute	 */
	public static final int EXPIRE_MINUTE = 60;
	
	/**	 * Expiration, a hour	 */
	public static final int EXPIRE_HOUR = 60 * EXPIRE_MINUTE;
	
	/**	 * Expiration, a day	 */
	public static final int EXPIRE_DAY = 24 * EXPIRE_HOUR;
	
	/**	 * Never expire	 */
	public static final int EXPIRE_NEVER = -0xabcd;
	
	public static final long ERROR_COUNT = -1;
	
}

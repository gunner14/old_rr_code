/**
 *
 */
package com.xiaonei.reg.guide.logic;

/**
 * <ul>
 * <li>
 * 
 * @author skllb </li> <li>2008-12-10 下午02:20:15</li> <li></li>
 *         </ul>
 */
public class NetworkUtils {
	public static boolean isCollegeNetwork(int networkid) {
		return (networkid > 80000000 && networkid < 90000000);
	}

	public static boolean isHighSchoolNetwork(int networkid) {
		return (networkid > 10000000 && networkid < 20000000);
	}

	public static boolean isJuniorNetwork(int networkid) {
		return (networkid > 40000000 && networkid < 50000000);
	}

	public static boolean isMiddleSchoolNetwork(int networkid) {
		return isCollegeNetwork(networkid) || isHighSchoolNetwork(networkid)
				|| isJuniorNetwork(networkid);
	}

	private NetworkUtils() {

	}
}

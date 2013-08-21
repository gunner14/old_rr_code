package com.xiaonei.v.home;


/**
 *
 * 提供不需要注入的接口
 *
 */
public class VHomeUtil {

	public static boolean inRange(int id) {
		return id >= Constants.VID_START && id < Constants.VID_END;
	}
	
}

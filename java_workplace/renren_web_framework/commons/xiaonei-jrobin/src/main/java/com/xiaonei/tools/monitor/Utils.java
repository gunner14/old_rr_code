package com.xiaonei.tools.monitor;

public class Utils {
	/**
	 * 去掉多余的空格
	 * @param str
	 * @return
	 */
	public static String getOutOfSpace (String  str){
		str=str.trim().replaceAll(" {2,}", " ");
		return str;
	}

}

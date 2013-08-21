package com.xiaonei.reg.guide.util;

public class Utils {

    static class SingletonHolder {
        static Utils instance = new Utils();
    }

    public static Utils getInstance() {
        return SingletonHolder.instance;
    }

    /**
     * 判断字符串是否为空
     *
     * @param str 待判断的字符串
     * @return 判断结果，布尔值
     */
    public boolean validateForString(String str) {
        return (str == null || "".equalsIgnoreCase(str)) ? true : false;
	}
}

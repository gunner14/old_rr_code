package com.xiaonei.login.util;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * 日期处理的工具包
 * 
 * @author wen.he1
 *
 */
public class LoginDateTimeUtil {


    // 默认日期格式化格式:"yyyy-MM-dd HH:mm:ss"
    private static final String DEFAULT_FORMAT     = "yyyy-MM-dd HH:mm:ss";

    /***
     * 将参数currDate字符串格式化成日期型，格式化的格式为:"yyyy-MM-dd HH:mm:ss"
     * 
     * @param currDate
     * @return 返回一个格式化的日期
     */
    public static Date formatStrToDate(String currDate) {
        return getFormatDate(currDate, DEFAULT_FORMAT);
    }

    
    /**
     * 当前的时间是否在startTime和endTime两者时间之中，如果是就返回true，否则返回false
     * 
     * @param startTime
     * @param endTime
     * @return
     */
    public static boolean isBetweenDate(String startTime,String endTime) {
    	return LoginDateTimeUtil.formatStrToDate(startTime).before(LoginDateTimeUtil.formatStrToDate(endTime)) 
		        && LoginDateTimeUtil.getCurrDate().after(LoginDateTimeUtil.formatStrToDate(startTime))
		        && LoginDateTimeUtil.getCurrDate().before(LoginDateTimeUtil.formatStrToDate(endTime));
    }

    /**
     * 将参数currDate字符串格式化的成日期型，格式化的格式：参数format决定
     * 
     * @param currDate
     *            要格式化的日期字符串
     * @param format
     *            格式化的格式如："yyyy-MM-dd"，"yyyy-MM-dd HH:mm:ss"
     * 
     * @return 格式化的日期Date
     */
    public static Date getFormatDate(String currDate, String format) {
        if (currDate == null)
            return null;
        SimpleDateFormat dtFormatdB = null;
        Date date;
        try {
            dtFormatdB = new SimpleDateFormat(format);
            date = dtFormatdB.parse(currDate);
        } catch (Exception e) {
            dtFormatdB = new SimpleDateFormat("yyyy-MM-dd");
            Date date_2_;
            try {
                date_2_ = dtFormatdB.parse(currDate);
            } catch (Exception exception) {
                return null;
            }
            return date_2_;
        }
        return date;
    }

    public static Date getCurrDate() {
        return new Date();
    }
    
    /**
     * 将参数currDate日期型格式化成字符串，格式化的格式为："yyyy-MM-dd"
     * 
     * @param currDate
     * @return 返回一个格式化的日期字符串
     */
    public static String formatDateToStr(Date currDate) {
        return getFormatDate(currDate, "yyyy-MM-dd");
    }
    
    /**
     * 改类适合时间格式为Y-M-D的字符串格式
     * 
     * @param datestring
     * @return
     */
    public static String addDateEndfix(String datestring) {
        if (datestring == null || datestring.equals(""))
            return null;
        return new StringBuilder().append(datestring).append(" 23:59:59")
                .toString();
    }
    
    /***
     * 将参数currDate日期型格式化的成日期字符串，格式化的格式：参数format决定
     * 
     * @param currDate
     *            要格式化的日期
     * @param format
     *            格式化的格式如："yyyy-MM-dd"，"yyyy-MM-dd HH:mm:ss"
     * 
     * @return 返回格式化的日期字符串
     */
    public static String getFormatDate(Date currDate, String format) {
        if (currDate == null)
            return "";
        SimpleDateFormat dtFormatdB = null;
        String string;
        try {
            dtFormatdB = new SimpleDateFormat(format);
            string = dtFormatdB.format(currDate);
        } catch (Exception e) {
            dtFormatdB = new SimpleDateFormat("yyyy-MM-dd");
            String string_0_;
            try {
                string_0_ = dtFormatdB.format(currDate);
            } catch (Exception exception) {
                return null;
            }
            return string_0_;
        }
        return string;
    }
    
    public static void main(String[] args) {
    	
    }

}

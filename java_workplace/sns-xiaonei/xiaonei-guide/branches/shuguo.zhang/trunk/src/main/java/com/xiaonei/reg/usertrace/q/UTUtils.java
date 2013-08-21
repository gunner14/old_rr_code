package com.xiaonei.reg.usertrace.q;

/**
 * batch insert data into tables
 * @author wei.cheng@opi-corp.com
 * @date 2009.11.05
 */
public class UTUtils {
    
    /**
     * `act1` queue default max size
     */
    public static final int VISIT_MaxSize           = 100;
    
    /**
     * `act2` queue default max size
     */
    public static final int OPR_MaxSize             = 100;
    
    /**
     * `act_subs` queue default max size
     *  notice: act_subs always less than act2, 
     *  so default queue size should be less 'OPR_MaxSize' too.
     */
    public static final int OPRSub_MaxSize          = 10;
    
    /**
     * `act1_cookie` queue default max size
     */
    public static final int COOKIE_MaxSize          = 100;
    /**
     * default author email. use for log
     */
    public static final String authorEmail = "[wei.cheng@opi-corp.com]";
}

/**
 *
 */
package com.xiaonei.reg.guide;

/**
 * GuideRouteDef
 * <p/>
 * 定义guide状态分发，标志定义
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-3-16 - 下午06:43:35
 */
public class GuideRouteDef {

    public static final int STATUS_GUIDE = 0;
    public static final int STATUS_GUIDE_ERR = -1;
    public static final int STATUS_FILLINFO = 1;
    public static final int STATUS_PORTRAIT = 2;
    public static final int STATUS_REGIMPORTMSN = 3;
    public static final int STATUS_PREVIEW = 4;
    public static final int STATUS_MOBILE_BASE_INFO = 5;
    public static final int STATUS_MOBILE_FILLINFO = 6;
    public static final int STATUS_MOBILE_VERIFYEMAIL = 7;
    public static final int STATUS_QUICK_FILLINFO = 8;

}

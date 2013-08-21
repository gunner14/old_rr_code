package com.xiaonei.reg.common.constants;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.platform.core.opt.OpiConstants;


/**
 * Globals.java
 *
 */
public class Globals {
	
	public static final boolean DEBUG_ENV = System.getProperty("DEBUG_ENV") != null ;
	public static final boolean NO_ANTISPAM_ENV = System.getProperty("NO_ANTISPAM_ENV") != null ;
    /**
     * The system account to send email to user, it should be changed
     */
    public static final String systemEmail = DodoyoResource.getString("sys.email.dodoyo");

    public static final String userMenMainUrl = OpiConstants.userMenMainUrl;
    public static final String userWomenMainUrl = OpiConstants.userWomenMainUrl;
    public static final String userMenHeadUrl = OpiConstants.userMenHeadUrl;
    public static final String userWomenHeadUrl = OpiConstants.userWomenHeadUrl;
    public static final String userMenTinyUrl = OpiConstants.userMenTinyUrl;
    public static final String userWomenTinyUrl = OpiConstants.userWomenTinyUrl;
    public static final String userMainUrl = OpiConstants.userMainUrl;
    public static final String userHeadUrl = OpiConstants.userHeadUrl;
    public static final String userTinyUrl = OpiConstants.userTinyUrl;
    public static final String userLargeUrl = OpiConstants.userLargeUrl;

    public static final Object domain = OpiConstants.domain;//"kaixin.com";
    public static final Object urlWww = OpiConstants.urlWww;
    public static final Object urlHome = OpiConstants.urlHome;
    public static final Object urlMain = OpiConstants.urlMain;
    public static final Object urlFriend = OpiConstants.urlFriend;
    public static final Object urlReg = OpiConstants.urlReg;
    public static final Object urlRegJump = OpiConstants.urlRegJump;
    public static final Object urlGuide = OpiConstants.urlGuide;
    public static final Object urlLogin = OpiConstants.urlLogin;
    public static final Object urlAdmin = OpiConstants.urlAdmin;
    public static final Object domainMobile = OpiConstants.domainMobile;
    public static final Object urlMobile = OpiConstants.urlMobile;
    public static final Object urlInvite = OpiConstants.urlInvite;
    public static final Object urlPage = OpiConstants.urlPage;
    public static final Object urlSafeCenter = OpiConstants.urlSafeCenter;
    public static final Object urlBrowse = OpiConstants.urlBrowse;
    public static String contextRoot = "";
    public static final int defaultLimit = 20;
    public static final int firstPage5Count = 5;


    public static final String tempPath = "/data/temp";
    public static final String tempTmpPath = tempPath + "/" + "tmp";

    public static final String MEM_CACHE_REG_INVITE_MAIL = "mem_cache_reg_inv_mail_";
}

package com.kaixin;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.platform.core.opt.OpiConstants;


/**
 * 
 * Globals.java
 * @author weige(wei.cheng@opi-corp.com) 2009-10-11 0:00:49
 */
public class Globals {
    
    public static boolean DEBUG_ENV = System.getProperty("DEBUG_ENV") != null ;
    public static boolean NO_ANTISPAM_ENV = System.getProperty("NO_ANTISPAM_ENV") != null ;
	public static final String errUnknown = DodoyoResource.getString("errors.action.unknown");
	public static final String errUrl = DodoyoResource.getString("errors.action.url");
	    
    public static final  String systemEmail = DodoyoResource.getString("sys.email.dodoyo.kaixin");    
    public static final  String userMenMainUrl = OpiConstants.userMenMainUrl;
    public static final  String userWomenMainUrl = OpiConstants.userWomenMainUrl;
    public static final  String userMenHeadUrl = OpiConstants.userMenHeadUrl;    
    public static final  String userWomenHeadUrl = OpiConstants.userWomenHeadUrl;
    public static final  String userMenTinyUrl = OpiConstants.userMenTinyUrl;
    public static final  String userWomenTinyUrl = OpiConstants.userWomenTinyUrl;
    public static final  String userMainUrl = OpiConstants.userMainUrl;
    public static final  String userHeadUrl = OpiConstants.userHeadUrl;
    public static final  String userTinyUrl = OpiConstants.userTinyUrl;    
    public static final  String userLargeUrl = OpiConstants.userLargeUrl;
    
    public static final  Object domain = OpiConstants.domain;
    public static final  Object domainMain = OpiConstants.domainMain;
    public static final  Object urlStatic = OpiConstants.urlStatic;
    public static final  Object urlImg =OpiConstants.urlImg;
    public static final  Object urlSource =OpiConstants.urlSource;
    public static final  Object urlMain = OpiConstants.urlMain;
    public static final  Object urlBrowse = OpiConstants.urlBrowse;
    public static final  Object urlFriend = OpiConstants.urlFriend;
    public static final  Object urlProfile = OpiConstants.urlProfile;
    public static final  Object urlHead = OpiConstants.urlHead;
    public static final  Object urlHeadUpload = OpiConstants.urlHeadUpload;
    public static final  Object urlClass = OpiConstants.urlClass;
    public static final  Object urlReg = OpiConstants.urlReg;
    public static final  Object urlInvite = OpiConstants.urlInvite;
    public static final  Object urlWww = OpiConstants.urlWww;
    public static final  Object urlGuide = OpiConstants.urlGuide;
    public static final  Object urlLogin = OpiConstants.urlLogin;
    public static final  Object urlIcode = OpiConstants.urlIcode;
    public static final  Object urlAdmin = OpiConstants.urlAdmin;
    public static final  Object domainMobile= OpiConstants.domainMobile;
    public static final  Object urlMobile = OpiConstants.urlMobile;
    public static final  Object urlXyx = OpiConstants.urlXyx;
    public static final  Object urlPassport = OpiConstants.urlPassport;
    public static final  Object urlSupport = OpiConstants.urlSupport;
    public static String contextRoot = "" ;
    public static final int defaultLimit = 20;
    public static final int firstPage5Count = 5;
   
    
    public static final String toError = "error";
    public static final String loginKey_session =OpiConstants.loginKey_session;
    public static final String loginKey_cookie="societyid" ;
    public static final String loginKey_ticket = "societyguester" ;
    public static final boolean isnewPassport=true ;
    public static final boolean isnewDataSource=false ;
    public static final String tempPath = "/data/temp" ;
    public static final String tempTmpPath=tempPath+"/"+"tmp" ;
    
    public static final String checkEmailDomainRules = "check-rules";
    public static final String regLogName = "regAction";
}

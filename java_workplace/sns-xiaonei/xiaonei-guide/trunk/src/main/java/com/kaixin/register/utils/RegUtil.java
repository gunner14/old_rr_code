package com.kaixin.register.utils;

import java.net.URLDecoder;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.TimeZone;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;

import com.kaixin.Globals;
import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.platform.core.utility.MathUtils;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.tribe.dao.TribeDAO;
import com.xiaonei.tribe.dao.TribeUserDAO;
import com.xiaonei.tribe.model.Tribe;
import com.xiaonei.tribe.model.TribeUser;

public class RegUtil {
	
	public static final int exceptInviterId = 700001678;

	public static final String KX_REG_BASEURL       = "/views/register/kaixin/";
	public static final String publicreg            = KX_REG_BASEURL + "kx8201.jsp";
    public static final String commoninit           = KX_REG_BASEURL + "common_regist_no.jsp";
    public static final String regok                = KX_REG_BASEURL + "reg_ok_afterreg.jsp";
    public static final String regLogin             = KX_REG_BASEURL + "regAutoLogin.jsp";
    public static final String regReactive          = KX_REG_BASEURL + "reactivate.jsp";
    public static final String baiduReg             = KX_REG_BASEURL + "baidu_syshome.jsp";
    public static final String moFillInfo           = KX_REG_BASEURL + "mobile_fillinfo.jsp";
    
    public static final String reg7101              = KX_REG_BASEURL + "kx7101.jsp";
    public static final String reg7103              = KX_REG_BASEURL + "kx7103.jsp"; 
    public static final String reg8101              = KX_REG_BASEURL + "kx8101.jsp";
    public static final String reg8102              = KX_REG_BASEURL + "kx8102.jsp";
    public static final String reg8103              = KX_REG_BASEURL + "kx8103.jsp";
    public static final String reg8401              = KX_REG_BASEURL + "kx8401.jsp";
    public static final String reg7301              = KX_REG_BASEURL + "kx7301.jsp";
    public static final String reg8204              = KX_REG_BASEURL + "kx8204.jsp";
    public static final String reg8205              = KX_REG_BASEURL + "kx8205.jsp";
    
    public static final String LOGIN_FAIL_REG       = "linreg.do";

	public static int randomNum(int param) {
		return MathUtils.randomInt(param);
	}

	/**
	 * 判断是否为公司邮箱
	 * 
	 * @param email
	 * @return
	 */
	public static boolean isWorkplaceEmail(String email) {
		boolean isWokrplaceEmail = false;
		if(null != getWorkplace(email)){
			isWokrplaceEmail = true;
		}
		return isWokrplaceEmail;
	}

	/**
	 * 判断是否为公司邮箱
	 * 
	 * @param email
	 * @return
	 */	
	public static Workplace getWorkplace(String email) {
		boolean isWokrplaceEmail = false;
		Workplace wp = null;
		List<Workplace> workplaceList = NetworkManager.getInstance().getWps();
		if (workplaceList != null) {
			for (int i = 0; i < workplaceList.size(); i++) {
				String emailList = workplaceList.get(i).getEmail();
				if (emailList != null) {
					String[] emails = emailList.split(";");
					for (int n = 0; n < emails.length; n++) {
						if (emails[n].trim().length() > 0 && email.endsWith("@".concat(emails[n].trim()))) {
							isWokrplaceEmail = true;
							wp = workplaceList.get(i);
							break;
						}
					}
					if (isWokrplaceEmail) {
						break;
					}
				}
			}
		}
		return wp;
	}
	
	/**
	 * 判断request 中的某个对象是否存在
	 * 
	 * @param request
	 * @param para
	 * @return true 存在且不为空
	 */
	public static boolean hasParameter(HttpServletRequest request, String para) {
		return (null != request.getParameter(para) && !"".equals(request.getParameter(para)));
	}

	public static void joinGroup(int userId, int groupId) {
		if (groupId == 0) {
			return;
		}
		try {
			Tribe tr = TribeDAO.getInstance().get(groupId);
			if (tr != null) {
				TribeUser tu = TribeUserDAO.getInstance().get(groupId, userId);
				if (tu == null) {
					com.xiaonei.tribe.home.TribeHome.addTribeMember(userId, groupId);
					TribeDAO.getInstance().incMemberCount(tr.getId());
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 判断是否为msn邮箱
	 * 
	 * @param email
	 * @return
	 */
	public static boolean isMSNEmail(String email) {
		if (email != null
				&& (email.endsWith("hotmail.com") || email.endsWith("msn.com") || email.endsWith("xnmsn.com") || email.endsWith("live.com")
						|| email.endsWith("live.cn") || email.endsWith("passport.com"))) {
			return true;
		}
		return false;
	}
	
	public static String emailLoginUrl(String email) {
		if (email == null || email.length() == 0) {
			return "";
		}

		String emailPortal = null;
		HashMap<String, String> emailPortals = new HashMap<String, String>();
		emailPortals.put("163.com", "http://mail.163.com");
		emailPortals.put("126.com", "http://mail.126.com");
		emailPortals.put("qq.com", "http://mail.qq.com");
		emailPortals.put("yahoo.com.cn", "http://cn.mail.yahoo.com");
		emailPortals.put("sina.com", "http://mail.sina.com.cn");
		emailPortals.put("hotmail.com", "http://mail.live.com");
		emailPortals.put("yahoo.com", "http://mail.yahoo.com");
		emailPortals.put("tom.com", "http://mail.tom.com");
		emailPortals.put("yahoo.cn", "http://cn.mail.yahoo.com");
		emailPortals.put("gmail.com", "http://mail.google.com");
		emailPortals.put("sohu.com", "http://mail.sohu.com");
		emailPortals.put("eyou.com", "http://freemail.eyou.com");
		emailPortals.put("21cn.com", "http://passport.21cn.com");
		emailPortals.put("sina.com.cn", "http://mail.sina.com.cn");
		emailPortals.put("163.com.cn", "http://163.com.cn");
		emailPortals.put("yeah.net", "http://yeah.net");
		emailPortals.put("msn.com", "http://mail.live.com");
		emailPortals.put("msn.cn", "http://mail.live.com");
		emailPortals.put("126.com.cn", "http://126.com.cn/search.html?q=%D3%CA%CF%E4");
		emailPortals.put("ncu.edu.cn", "http://ncu.edu.cn");
		emailPortals.put("mail.china.com", "http://mail.china.com");
		emailPortals.put("live.cn", "http://mail.live.com");

		String[] emailArr = email.split("@");
		String domain = emailArr[1].toLowerCase();
		emailPortal = emailPortals.get(domain);

		return ((emailPortal == null) ? "" : emailPortal);
	}
	
	/**
	 * 判断邮箱的后缀
	 * @param email
	 * @return
	 */
	public static boolean checkEmailDomain(String email){
		if (email == null || email.length() == 0) {
			return false;
		}
		String[] emailArr = email.split("@");
		String domain = emailArr[1].toLowerCase();
		if("hotmail.com".equals(domain) || "163.com".equals(domain)
				|| "126.com".equals(domain) || "sina.com".equals(domain)
				|| "yahoo.com.cn".equals(domain) || "qq.com".equals(domain)
				|| "sohu.com".equals(domain) || "gmail.com".equals(domain)
				|| "yahoo.cn".equals(domain) || "msn.com".equals(domain)
				|| "live.cn".equals(domain) || "tom.com".equals(domain)
				|| "yahoo.com".equals(domain) || "sina.com.cn".equals(domain)
				|| "21cn.com".equals(domain) || "yeah.net".equals(domain)
				|| "hotmail.com.cn".equals(domain) || "139.com".equals(domain)
				|| "citiz.com".equals(domain) || "live.com".equals(domain)){
			return true;
		}
		return false;
	}
	
	/**
	 * set cookie for foot*.inc
	 * @param response
	 */
	public static void setCookieForFootInc(HttpServletResponse response)
	{
		Cookie ck = new Cookie("ck_for_foot_inc", "ck_for_foot_inc_value");
        ck.setDomain("." + Globals.domain.toString());
        ck.setMaxAge(60 * 60 * 24);
        response.addCookie(ck);
        
        return ;
	}
	
	/**
	 * 注册的时候，注册成功以后，给写一个Cookie值……add by inshion...╮(╯^╰)╭
	 * @param response
	 */
	private static final String CHANNEL_REG_SUCC_COOKIE_NAME = "rgsu";
	public static void setCookieForAntiCheat(HttpServletRequest request,HttpServletResponse response)
	{
		String v = request.getParameter("ss");
		if(v==null){
			v = "DN";
		}
		Cookie ck = new Cookie(CHANNEL_REG_SUCC_COOKIE_NAME, v);
        ck.setDomain("." + Globals.domain.toString());
        ck.setMaxAge(60 * 60 * 24);
        response.addCookie(ck);
        
        return ;
	}
	
	/**
	 * from class name get controller path
	 * @param obj , init controller path
	 * @return
	 */
	public static String getControllerURI(String obj){
        String result = publicreg;
        if(StringUtils.isNotEmpty(obj)){
            if(obj.length() >6){
                String tmp = obj.substring(2, obj.indexOf("Controller"));
                result = KX_REG_BASEURL + "kx" + tmp + ".jsp";;
            }
        }
        return result;
    }
	
	public static boolean isPubReg(String regFlag){
	    if("public_reg7301".equals(regFlag) || "public_reg".equals(regFlag)
                || "public_reg8204".equals(regFlag) || "public_reg8205".equals(regFlag)){
            return true;
        }
	    return false;
	}
	
	public static String getPubRegPage(String regFlag){
	    regFlag = StringUtils.trimToEmpty(regFlag);
	    if("public_reg7301".equals(regFlag)){
	        return reg7301;
	    }
	    if("public_reg8204".equals(regFlag)){
            return reg8204;
        }
        if("public_reg8205".equals(regFlag)){
            return reg8205;
        }
	    return publicreg;
	}
	
	/**
	 * deal with action id, and change to right URI
	 * @param actionId
	 * @return
	 */
    public static String changeActionId2URI(String actionId){
        String result = publicreg;
        if(actionId != null && !"".equals(actionId) && actionId.length()>2){
            result = KX_REG_BASEURL + "kx" + actionId.substring(2) + ".jsp";
        }
        return result;
    }
    
    public static String changeInitPath2URI(HttpServletRequest request){
        //default
        String result = publicreg;
        String urlp = "^/kx[0-9]{4}\\.do$";
        String requestURI = request.getRequestURI();
        if (Pattern.compile(urlp).matcher(requestURI).matches() == false) {
            return String.valueOf(RegOprDefine.NORMAL_VISIT_XN_REG);
        }
        String[] pa = requestURI.split("/");
        String path = pa[pa.length - 1].split("\\.")[0];
        String actionId = path.replaceAll("kx", "10");
        result = changeActionId2URI(actionId);
        return result;
    }
	
	public static String toMyString(int num, int value) {
        String result = String.valueOf(value);
        while (num > result.length()) {
            result = "0" + result;
        }
        return result;
    }
	
	public static boolean checkId(String xId){
	    String nickPatten = "^\\w{6,10}|\\d{6,10}$";
        Pattern p = Pattern.compile(nickPatten);
        Matcher m = p.matcher(xId);
        if (!m.matches()) {
            return false;
        }
        return true;
	}
	
	public static boolean isValidMain(String email) {
		String pattern3 = "\\w*@{1}[\\w&&[^_]][\\w\\.]*\\w";
		String pattern4 = ".*\\.\\..*";
		return Pattern.matches(pattern3, email)
				&& !Pattern.matches(pattern4, email);
	} 
	
    public static void main(String []args){
        //System.out.println( RegUtil.getControllerURI(Kx8301Controller.class.getSimpleName()));
        System.out.println( "ERROR(RegUtil.java):"+"RegUtil.getControllerURI(Kx8301Controller.class.getSimpleName())");
        System.out.println(RegUtil.changeActionId2URI("201345"));
        System.out.println(toMyString(4,1212));
        System.out.println(Integer.parseInt("97", 16));
        
        
        System.out.println(checkId("1234568978900222"));
        System.out.println(checkId("1297"));
        System.out.println(checkId("1234568"));
        System.out.println(checkId("12345678ab"));
        System.out.println(checkId("1234568978900222abc"));
        System.out.println(checkId("1234abc"));
        
        String test = "faf&\\!'\"";
        for(char t : test.toCharArray()){
            System.out.println(t);
        }
        System.out.println(StringUtils.contains("胡强\\", "\\"));
        try{
            System.out.println(URLEncoder.encode("王泰", "UTF-8"));
            System.out.println(URLEncoder.encode("男生", "UTF-8"));
            System.out.println(URLDecoder.decode("zjw0358@msn.com%22%3E%3Cmarquee%3Eaaa", "UTF-8"));
            System.out.println(isValidMain("zjw0358@msn.com"));
            System.out.println("-------------------我是分割线------------------");
            
            String str="I am a evil, I am Hello ok,\nnew \rline\t test!";
            System.out.println("before:"+str);
            String after = trimLine(str);
            System.out.println("after:"+after);
            
            System.out.println("-------------------我是分割线--22----------------");
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");  
            sdf.setTimeZone(TimeZone.getTimeZone("UTC"));  
            System.out.println(sdf.format(new Date()));
            
            System.out.println(Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTime().getTime());
            
            
            String abc = "string123,|,string456";
            String []cc = abc.split(",\\|,");
            for(String c : cc)
            	System.out.println(c);
        }catch(Exception ex){
            ex.printStackTrace();
        }
    }

    private static String trimLine(String str) {
        Pattern p = Pattern.compile("\t|\r|\n");
        Matcher m = p.matcher(str);
        String after = m.replaceAll("");
        return after;
    }
}

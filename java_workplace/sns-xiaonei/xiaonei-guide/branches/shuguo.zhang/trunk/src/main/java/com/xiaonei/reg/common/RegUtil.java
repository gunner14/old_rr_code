package com.xiaonei.reg.common;

import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.platform.core.utility.MathUtils;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.platform.core.utility.VerifyCode;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.dao.RegAuditDAO;
import com.xiaonei.reg.register.dao.UserInviteCodeViewDAO;
import com.xiaonei.reg.register.dao.UserPersonalInfoDAO;
import com.xiaonei.reg.register.model.Invite;
import com.xiaonei.reg.register.model.UserInviteCode;
import com.xiaonei.tribe.dao.TribeDAO;
import com.xiaonei.tribe.dao.TribeUserDAO;
import com.xiaonei.tribe.model.Tribe;
import com.xiaonei.tribe.model.TribeUser;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;

public class RegUtil {

	/**
	 * 审核的表，测试的时候用_tmp，外网用空的
	 */
	public final static String audit_table = "";

	/**
	 * 判断是否为msn邮箱
	 * 
	 * @param email
	 * @return
	 */
	public static boolean isMSNEmail(String email) {
		if (email != null
				&& (email.endsWith("hotmail.com") || email.endsWith("msn.com")
						|| email.endsWith("xnmsn.com")
						|| email.endsWith("live.com")
						|| email.endsWith("live.cn") || email
						.endsWith("passport.com"))) {
			return true;
		}
		return false;
	}

	/**
	 * 判断是否为公司邮箱
	 * 
	 * @param email
	 * @return
	 */
	public static boolean isWorkplaceEmail(String email) {
		boolean isWokrplaceEmail = false;
		if (null != getWorkplace(email)) {
			isWokrplaceEmail = true;
		}
		return isWokrplaceEmail;
	}

	/**
	 * 使用公司邮箱获得对应的公司信息
	 * 
	 * @param email
	 * @return
	 */
	public static Workplace getWorkplace(String email) {
		boolean isWokrplaceEmail = false;
		Workplace wp = null;
		List<Workplace> workplaceList = RegUtil.getWps();
		if (workplaceList != null) {
			for (int i = 0; i < workplaceList.size(); i++) {
				String emailList = workplaceList.get(i).getEmail();
				if (emailList != null) {
					String[] emails = emailList.split(";");
					for (int n = 0; n < emails.length; n++) {
						if (emails[n].trim().length() > 0
								&& email.endsWith("@".concat(emails[n].trim()))) {
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

	private static List<Workplace> getWps() {
		return NetworkManager.getInstance().getWps();
	}

	/**
	 * 判断request 中的某个对象存在不？
	 * 
	 * @param request
	 * @param para
	 * @return true 存在且不为空 不为0
	 */
	public static boolean hasParameter(HttpServletRequest request, String para) {
		return (null != request.getParameter(para)
				&& !"".equals(request.getParameter(para)) && !"0"
				.equals(request.getParameter(para)));
	}

	/**
	 * @param inviterId
	 * @return
	 * @throws SQLException
	 */
	public static String getUserInviteCode(int inviterId) throws SQLException {
		UserInviteCode uic = UserInviteCodeViewDAO.getInstance().getByUser(inviterId);
		if (uic == null) {
			uic = new UserInviteCode();
			uic.setId(inviterId);
			do {
				uic.setInviteCode(VerifyCode.getVerifyCode(10));
			} while (UserInviteCodeViewDAO.getInstance().getByCode(uic.getInviteCode()) != null);

			UserPersonalInfoDAO.getInstance().insertInviteCode(uic.getId(),
					uic.getInviteCode());
		} else {
			if (uic.getInviteCode() == null) {
				do {
					uic.setInviteCode(VerifyCode.getVerifyCode(10));
				} while (UserInviteCodeViewDAO.getInstance().getByCode(uic.getInviteCode()) != null);
				UserPersonalInfoDAO.getInstance().updateInviteCode(uic.getId(),
						uic.getInviteCode());
			}
		}
		return uic.getInviteCode();
	}

	public static boolean isValidInvite(int userId, String code) {
		boolean isValid = false;
		try {
			if (userId > 0 && code != null) {
				Invite invite = InviteDAO.getInstance().get(userId);
				if (invite != null && code.equals(invite.getInvitecode())) {
					isValid = true;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return isValid;
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
					com.xiaonei.tribe.home.TribeHome.addTribeMember(userId,
							groupId);
					TribeDAO.getInstance().incMemberCount(tr.getId());
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static int randomNum(int param) {
		return MathUtils.randomInt(param);
	}

	/**
	 * 得到散表表名
	 * 
	 * @param id
	 * @return
	 */
	public static String getTableName(int id) {
		String table = "relation_";
		try {
			table += (int) (id % 100);
		} catch (Exception e) {
			LogTimeUtils.logger.error(e);
		}
		return table;
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
		emailPortals.put("msn.cn", "http://cn.msn.com");
		emailPortals.put("xnmsn.cn", "http://cn.msn.com");
		emailPortals.put("126.com.cn",
				"http://126.com.cn/search.html?q=%D3%CA%CF%E4");
		emailPortals.put("ncu.edu.cn", "http://ncu.edu.cn");
		emailPortals.put("mail.china.com", "http://mail.china.com");
		emailPortals.put("live.cn", "http://mail.live.com");
		emailPortals.put("live.com", "http://mail.live.com");

		String[] emailArr = email.split("@");
		String domain;
		try {
			domain = emailArr[1].toLowerCase();
		} catch (Exception e) {
			return "";
		}
		emailPortal = emailPortals.get(domain);

		return ((emailPortal == null) ? "" : emailPortal);
	}

	/**
	 * 根据学校得到高中的网络id
	 * 
	 * @param highSchoolCode
	 * @param schoolType
	 * @return
	 */
	public static int getHsNetWorkId(int highSchoolCode, int schoolType) {
		if (schoolType == 1) {
			int jhsid;
			try {
				jhsid = RegAuditDAO.getInstance()
						.getJuniorSchoolIdByHighSchoolId(
								Integer.valueOf(highSchoolCode));
			} catch (SQLException e) {
				e.printStackTrace();
				jhsid = 0;
			}
			if (jhsid > 0)
				highSchoolCode = jhsid;
		}
		return highSchoolCode;
	}

	/**
	 * 是否为手机注册
	 * 
	 * @param regForm
	 * @return
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-11-10 下午01:38:47
	 */
	public static boolean isMobileReg(String phoneNum) {
		return StringFormat.isValidMobile(phoneNum);
	}

	public static String getEncryptString(String str){
		int len1 = encryptKey.length();
		int len2 = str.length();
		int len = len1 > len2 ? len2: len1;
		StringBuilder sb = new StringBuilder();
		for(int i=0; i< len; i ++){
			sb.append(str.charAt(i));
			sb.append(encryptKey.charAt(i));
		}
		if(len1 > len) sb.append(encryptKey.substring(len));
		if(len2 > len) sb.append(str.substring(len));
		return sb.toString();
	}
	
	/**
	 * 字符串变数字
	 * 
	 * @param str
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-27 - 下午04:13:28
	 */
	public static int strToInt(String str) {
		try {
			return Integer.valueOf(str);
		} catch (NumberFormatException e) {
			return 0;
		}
	}

   public static String getDomain(final HttpServletRequest request){
        String regUrl = request != null && request.getRequestURL() != null ? request
                .getRequestURL().toString()
                : null;
        if (regUrl != null && regUrl.indexOf("http://") > -1) {
            String temp = regUrl.substring(7);
            if (temp != null && temp.indexOf("/") > -1) {
                regUrl = temp.substring(0, temp.indexOf("/"));
            }
        }
        return regUrl;
    }
   
   
   //register monitor  
   public static final Logger rlog = Logger.getLogger("regMonitorLog");
   
   public static final String bizIcode   = "Icode";
   public static final String bizRegForm = "RegForm";
   public static final String bizRegSpam = "RegSpammer";
   public static final String bizAccount = "CreateAccount";
   public static final int chkBegin = 0;
   public static final int chkFail  = 1;
   public static final int chkEnd   = 2;
   
   public static final String encryptKey = "renren3924!$abux&^!";
   
   public static void main(String []args){
	    System.out.println(StringUtils.isNumeric(""));
	    System.out.println(MD5Util.md5(getEncryptString("wangqiangwei8901.vip@msn.com.cn")));
	}
}

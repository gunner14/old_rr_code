/**
 * xiaonei-reg
 * com.xiaonei.reg.common
 * UserInfo.java
 * Sep 9, 2009 - 11:17:31 AM
 */
package com.xiaonei.reg.common.users;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.model.UserTime;
import com.xiaonei.platform.core.model.UserUrl;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.register.dao.UserPassportExtDAO;
import com.xiaonei.reg.register.model.UserPassportExt;
import com.xiaonei.xce.userpassport.NoSuchUserByEmailException;
import com.xiaonei.xce.userpassport.UserPassportInfo;
import com.xiaonei.xce.usertime.UserTimeAdapter;
import com.xiaonei.xce.userurl.UserUrlAdapter;

/**
 * UserAdapter
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 9, 2009 - 11:17:31 AM
 * 
 */
public class UserAdapter {

	private static class UserInfoQM {
		private Map<Integer, UserInfo> userInfoMap = new HashMap<Integer, UserInfo>();
		private UserInfoIdQueue idQueue = new UserInfoIdQueue(500);
	}

	private static UserInfoQM idQM = new UserInfoQM();

	public static UserInfo get(User user) {
		return get(user.getId());
	}

	public static UserInfo get(Integer id) {
		synchronized (idQM) {
			if (idQM.userInfoMap.get(id) == null) {
				idQM.userInfoMap.put(id, new UserInfo(id));
				idQM.idQueue.add(id);
				int popUserInfoId = idQM.idQueue.getPopUserInfoId();
				if (popUserInfoId != UserInfoIdQueue.NO_POP_ID) {
					idQM.userInfoMap.remove(popUserInfoId);
				}
				RegLogger.getLoggerWithType(UserAdapter.class).debug(
						"new UserInfo:" + id + " idQueue.size():"
								+ idQM.idQueue.size() + " pop id:"
								+ popUserInfoId);
			}
			RegLogger.getLoggerWithType(UserAdapter.class).debug("get id:" + id);
			return idQM.userInfoMap.get(id);
		}
	}

	/**
	 * 
	 * @param id
	 * @param newEmail
	 * @param emailVerify
	 * @author wangtai(tai.wang@opi-corp.com) Sep 9, 2009 - 12:28:17 PM
	 */
	public static void updateVerifyNewEmail(int id, String newEmail,
			String emailVerify) {
		UserPassportExt upe = new UserPassportExt();
		upe.setId(id);
		upe.setEmail_verify(emailVerify);
		upe.setNew_email(newEmail);
		try {
			UserPassportExtDAO.getInstance().updateVerifyNewEmail(upe);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

	/**
	 * getUser
	 * 
	 * @param email
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 10, 2009 - 3:33:00 PM
	 */
	public static User getUser(String account, String domain) {
		User user;
		try {
			user = WUserAdapter.getInstance().getByEmail(account);
		} catch (RuntimeException e) {
			e.printStackTrace();
			return null;
		}
		return user;
		// AccountInfo[] accounInfoArr = LoginAdapter.getInstance().getAccount(
		// account);
		// for (AccountInfo accountInfo : accounInfoArr) {
		// if (accountInfo.domain.equals(domain.toString())) {
		// return getUser(accountInfo.userId);
		// }
		// }
		// return null;
	}

	public static User getUser(int id) {
		return UserAdapter.get(id).getUser();
	}

	public static User getUser(User user) {
		return UserAdapter.get(user.getId()).getUser();
	}

	@SuppressWarnings("unused")
	private static void setBasic(UserBasic oceBasic) {
		//2010-01-20 拆分UserBasic
		//UserAdapter.get(oceBasic.getId()).setUserBasic(oceBasic.getId(), oceBasic);
	}	

	/**
	 * hasUser
	 * 
	 * @param mobile
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 10, 2009 - 3:57:11 PM
	 */
	public static boolean hasUser(String account) {
		/*List<Type> list = new ArrayList<Type>();
		list.add(Type.Account);
		Account a;
		try {
			a = PassportAdapterFactory.getWebLogin().login(list, account,
					"", Globals.domain.toString(), -1, true);
			WUserAdapter.getInstance().getPassportAdapter().getUserIdByEmail(account);
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		if (null == a) {
			return false;
		}
		return a.getSuccess() != -1;*/
	    //使用新的验证方法
	    int uid = 0;
	    try {
            uid = WUserAdapter.getInstance().getPassportAdapter().getUserIdByEmail(account);
        }catch(NoSuchUserByEmailException e){
            return false;
        } catch (Exception e) {
            System.err.print(ToStringBuilder.reflectionToString(account, ToStringStyle.SIMPLE_STYLE));
            return false;
        }
        return uid > 0;
    }

	public static void updateEmail(int userId, String email, String newEmail) {
		final UserPassportInfo userPassportInfo = new UserPassportInfo();
		userPassportInfo.setId(userId);
		userPassportInfo.setAccount(StringUtils.trimToEmpty(newEmail)
				.toLowerCase());

		try {
			WUserAdapter.getInstance().getPassportAdapter().setUserPassport(
					userId, userPassportInfo);
		} catch (final Ice.TimeoutException e) {
			e.printStackTrace();
		}
	}

	public static void incLoginCount(int id) {
		UserAdapter.get(id).incLoginCount();
	}

	public static UserBasic getBasic(int id) {
		return UserAdapter.get(id).getUserBasic();
	}

	public static void updateSelected(int id, int selected) {
		UserAdapter.get(id).updateSelected(selected);
	}

	public static UserDesc getDesc(int id) {
		return UserAdapter.get(id).getUserDesc();
	}

	public static void updateHeadUrl(int id, String head, String main,
			String tiny, String large) {
		UserAdapter.get(id).updateHeadUrl(head, main, tiny, large);
	}

	

	public static UserUrlAdapter<UserUrl> getUrlAdapter() {
		return WUserAdapter.getInstance().getUrlAdapter();
	}

	public static UserTimeAdapter<UserTime> getTimeAdapter() {
		return WUserDescAdapter.getInstance().getTimeAdapter();
	}

}

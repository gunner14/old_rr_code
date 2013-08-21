/**
 *
 */
package com.xiaonei.reg.register.logic.additional;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.reg.register.constants.IRegCookieKeyDef;

/**
 * RegChannelActivityLogic
 * <p/>
 * 渠道活动
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-27 - 下午03:42:11
 */
public class RegChannelActivityLogic implements IRegCookieKeyDef {

	private static final Map<Integer, String> ACTIVITY_MAP;
	private static final Integer STOP_USE = Integer.MAX_VALUE;

	static {
		ACTIVITY_MAP = new HashMap<Integer, String>();
		// ACTIVITY_MAP.put(id | STOP_USE, "说明");
		ACTIVITY_MAP.put(601 | STOP_USE, "砸金蛋");
		ACTIVITY_MAP.put(602, "百色情人节");
		ACTIVITY_MAP.put(603, "girl");
		ACTIVITY_MAP.put(604, "");
		ACTIVITY_MAP.put(610, "09新生");
		ACTIVITY_MAP.put(614, "网吧送话费");
		ACTIVITY_MAP.put(615, "寒假活动");
		ACTIVITY_MAP.put(616, "阿凡达活动");
	}

	/**
	 * 渠道用，from 表明从哪条渠道过来的注册<br>
	 * 激活时读出，返回给相应渠道信息
	 * 
	 * @param from
	 * @param ayid
	 * @param ss
	 * @param response
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-27 - 下午03:46:24
	 */
	public static void saveChannelInfoToCookie(String inviterId, String from,
			String ayid, String ss, HttpServletResponse response) {
		// 渠道推广
		try {
			if (StringUtils.isNotEmpty(from)) {
				// 这个from 在砸金蛋活动中也使用
				CookieManager.getInstance().saveCookie(response, FROM, from,
						60 * 60, "/");
			}
			if (ayid != null) {
				CookieManager.getInstance().saveCookie(response, AYID, ayid,
						60 * 60, "/");
			}
		} catch (Exception e) {
			System.err.println("save \"from\" err");
			e.printStackTrace();
		}
		Integer ifrom;
		try {
			ifrom = Integer.valueOf(from);
		} catch (NumberFormatException e1) {
			ifrom = STOP_USE;
		}
		// 活动
		if (ACTIVITY_MAP.get(ifrom) != null && !ifrom.equals(STOP_USE)) {
			try {
				if (StringUtils.isNotEmpty(inviterId)
						&& StringUtils.isNotEmpty(from)) {
					CookieManager.getInstance().saveCookie(response,
							INVITER_ID, inviterId, 60 * 60, "/");
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	}

	/**
	 * 回调活动方法
	 * 
	 * @param request
	 * @param user
	 * @param response
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-27 - 下午04:31:52
	 *//*
	public static void callChannlFun(HttpServletRequest request, User user,
			HttpServletResponse response) {
		try {
			String sType = StringUtils.trim(CookieManager.getInstance()
					.getCookie(request, FROM));
			String sInviterId = StringUtils.trim(CookieManager.getInstance()
					.getCookie(request, INVITER_ID));
			// 清出cookie
			CookieManager.getInstance().clearCookie(response, FROM, 0, "/");
			CookieManager.getInstance().clearCookie(response, INVITER_ID, 0,
					"/");
			if (StringUtils.isNotEmpty(sType)
					&& StringUtils.isNotEmpty(sInviterId)) {
				Integer ifrom;
				try {
					ifrom = Integer.valueOf(sType);
				} catch (NumberFormatException e1) {
					ifrom = STOP_USE;
				}
				// 活动
				if (ACTIVITY_MAP.get(ifrom) != null && !ifrom.equals(STOP_USE)
						&& other(ifrom, user.getId())) {
					final int accepterId = user.getId();
					final String other1 = String.valueOf(UserAdapter.get(user).getStage());
					final int type = Integer.valueOf(sType);
					final int inviterId = Integer.valueOf(sInviterId);
					Runnable task =  new Runnable() {
						@Override
						public void run() {
							try {
								SpecialeventLogic.getInstance().addRelRecord(inviterId,
										accepterId, type, other1);
							} catch (SQLException e) {
								e.printStackTrace();
							}
						}
					};
					RunnableTaskExecutor.getInstance().runTask(task);
				}
			}
		} catch (NumberFormatException e) {
			e.printStackTrace();
		}
	}*/

	/*private static boolean other(Integer ifrom, int id) {
		if (610 == ifrom && !BirthdayLogic.getInstance().compareYear(id, 1990, 1995)) {
			return false;
		}
		return true;
	}*/
}

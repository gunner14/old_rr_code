/**
 *
 */
package com.xiaonei.reg.register.logic.pretreatment;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.guide.dao.InviteUrlParasDAO;
import com.xiaonei.reg.register.model.InviteUrlParas;

/**
 * InviteUrlParasLogic
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-22 - 上午11:21:23
 */
public class InviteUrlParasLogic {

	private static int UUID_LEN = "a902ce44-72d8-4de8-a262-84ef07854660"
			.length();

	private static InviteUrlParasLogic _instance = new InviteUrlParasLogic();

	public static InviteUrlParasLogic getInstance() {
		return _instance;
	}

	/**
	 * 封装参数列表
	 * 
	 * @param request
	 * @return
	 * @throws Exception
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-22 - 上午11:39:41
	 */
	public Map<String, String> getParas(HttpServletRequest request)
			throws Exception {
		String uuid = StringUtils.isNotEmpty(request.getParameter("iid")) ? request
				.getParameter("iid")
				: request.getParameter("uuid");
		InviteUrlParas iup = null;
		if (StringUtils.isNotEmpty(uuid)) {
			uuid = StringUtils.substring(uuid, 0, UUID_LEN);
			final String fuuid = uuid;
			iup = InviteUrlParasDAO.getInstance().getParas(fuuid); 
			/*final TimeLimitConfiguration tl = TimeLimitController.getTimeLimit(
					25, 5l);
			tl.setSingleConfig("getuuid", new TimeLimitThread(tl) {
				@Override
				public Object work() {
					try {
						return InviteUrlParasDAO.getInstance().getParas(fuuid);
					} catch (final SQLException e) {
						e.printStackTrace();
					}
					return new InviteUrlParas();
				}
			});
			try {
				iup = (InviteUrlParas) tl.getSingleReturn();
			} catch (final Throwable e) {
				e.printStackTrace();
			}*/
		}
		// 显式传参数
		else {
			iup = setVParams(request);
		}
		if (null == iup) {
			RegLogger.getLoggerWithType("NullUUid").error(
					"NullUUidException,uuid = " + uuid);
			return new ConcurrentHashMap<String, String>();
		}
		return iup.getParas();
	}

	private InviteUrlParas setVParams(HttpServletRequest request) {
		InviteUrlParas iup;
		iup = new InviteUrlParas();
		String appIdStr = request.getParameter("appId");
		if (appIdStr != null && !appIdStr.equals("")) {
			iup.setApp_id(Integer.valueOf(appIdStr));
		}
		String inviterId = request.getParameter("i");
		if (inviterId != null && !inviterId.equals("")) {
			iup.setUser_id(Integer.valueOf(inviterId));
		}
		String inviteeEmail = request.getParameter("e");
		if (inviteeEmail != null && !inviteeEmail.equals("")) {
			iup.setEmail_to(inviteeEmail);
		}
		String appToken = request.getParameter("appToken");
		if (appToken != null && !appToken.equals("")) {
			iup.setApp_token(appToken);
		}

		String inviteType = request.getParameter("inviteType");
		if (inviteType != null && !inviteType.equals("")) {
			// do nothing
		}

		String from = request.getParameter("from");
		if (from != null && !from.equals("")) {
			// do nothing
		}

		String groupName = request.getParameter("buddygroup");
		if (groupName != null && !groupName.equals("")) {
			iup.setBuddy_group(groupName);
		}
		String code = request.getParameter("code");
		if (groupName != null && !groupName.equals("")) {
			iup.setCode(code);
		}
		String ss = request.getParameter("ss");
		if (groupName != null && !groupName.equals("")) {
			iup.setSs(ss);
		}
		return iup;
	}
}

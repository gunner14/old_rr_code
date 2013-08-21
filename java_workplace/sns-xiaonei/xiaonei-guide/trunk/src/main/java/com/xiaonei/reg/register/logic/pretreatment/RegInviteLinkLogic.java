package com.xiaonei.reg.register.logic.pretreatment;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.dao.InviteUrlParasDAO;
import com.xiaonei.reg.register.dao.InviteDAO;

import com.xiaonei.reg.register.dao.UserInviteCodeViewDAO;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.reg.register.model.InviteUrlParas;
import com.xiaonei.reg.register.model.UserInviteCode;
import com.xiaonei.reg.thread.timelimit.TimeLimitConfiguration;
import com.xiaonei.reg.thread.timelimit.TimeLimitController;
import com.xiaonei.reg.thread.timelimit.TimeLimitThread;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

import java.sql.SQLException;
import java.util.Map;

public class RegInviteLinkLogic {

	public static final int LINK_NOT_INVITE = -1;
	public static final int LINK_INVITE_APP_COPY = 10;
	public static final int LINK_INVITE_APP_MAIL = 20;
	// public static final int LINK_INVITE_APP_MAIL_USED = 25;
	public static final int LINK_INVITE_COMMON_COPY = 30;
	public static final int LINK_INVITE_COMMON_MAIL = 40;
	public static final int LINK_INVITE_HS_COPY = 50;

	/* 不同注册页面地址 */
	public static final String REG_COMMON = "register";

	public static final String REG_INVITE_HS = "invite_hs";
	public static final String REG_INVITE_COMMON_COPY = "invite_common_copy";
	public static final String REG_INVITE_COMMON_MAIL = "invite_common_mail";
	public static final String REG_INVITE_APP_COPY = "invite_app_copy";
	public static final String REG_INVITE_APP_MAIL = "invite_app_mail";

	public static final String REG_CHANNEL_BAIDU = "reg_adv";
	public static final String REG_CHANNEL_MAOYOU = "reg_maoyou";
	public static final String REG_CHANNEL_SANGUO = "reg_sanguo";
	public static final String REG_CHANNEL_MSNTAB = "reg_msntab";
	public static final String REG_CHANNEL_EDU = "reg_edu";
	public static final String REG_ADS_HSDOG = "reg_hsdog";

	/* 各种邀请链接的参数 */
	private static final String[] LINK_PARA_INVITE_APP_COPY = { "i", "code",
			"appId" };
	private static final String[] LINK_PARA_INVITE_APP_MAIL = { "i", "e",
			"appId", "appToken" };
	private static final String[] LINK_PARA_INVITE_COMMON_COPY = { "i", "code" };
	private static final String[] LINK_PARA_INVITE_COMMON_MAIL = { "i", "e" };
	private static final String[] LINK_PARA_INVITE_HS_COPY = { "i", "code" };

	private static RegInviteLinkLogic _instance = new RegInviteLinkLogic();

	public static RegInviteLinkLogic getInstance() {
		return _instance;
	}

	private RegInviteLinkLogic() {
	}

	/**
	 * 得到跳转页面地址
	 * 
	 * @param req
	 * @param spot
	 * @return
	 */
	public String getSpot(HttpServletRequest req) {
		String ispot = REG_COMMON;
		switch (RegInviteLinkLogic.getInstance().whichInviteLink(req, null)) {
		case RegInviteLinkLogic.LINK_INVITE_HS_COPY:
			ispot = REG_INVITE_HS;
			break;
		case RegInviteLinkLogic.LINK_INVITE_COMMON_COPY:
			ispot = REG_INVITE_COMMON_COPY;
			break;
		case RegInviteLinkLogic.LINK_INVITE_COMMON_MAIL:
			ispot = REG_INVITE_COMMON_MAIL;
			break;
		case RegInviteLinkLogic.LINK_INVITE_APP_COPY:
			ispot = REG_INVITE_APP_COPY;
			break;
		case RegInviteLinkLogic.LINK_INVITE_APP_MAIL:
			ispot = REG_INVITE_APP_MAIL;
			break;
		case RegInviteLinkLogic.LINK_NOT_INVITE:
			String from = req.getParameter("from");
			if ("44".equals(from)) {
				ispot = REG_CHANNEL_BAIDU;
			} else if ("reg_sanguo".equals(from)) {
				ispot = REG_CHANNEL_SANGUO;
			} else if ("reg_maoyou".equals(from)) {
				ispot = REG_CHANNEL_MAOYOU;
			} else if (REG_ADS_HSDOG.equals(from)) {
				ispot = REG_ADS_HSDOG;
			} else if ("msntab".equals(from)) {
				ispot = REG_CHANNEL_MSNTAB;
			} else if ("edu".equals(from)) {
				ispot = REG_CHANNEL_EDU;
			} else {
				ispot = REG_COMMON;
			}
			break;
		default:
			ispot = REG_COMMON;
		}
		return ispot;
	}

	/**
	 * 通过参数判断是哪一种邀请类型
	 * 
	 * @param request
	 * @param paras
	 * @return
	 */
	public int whichInviteLink(final HttpServletRequest request,
			Map<String, String> paras) {
		int type = LINK_NOT_INVITE;
		try {
			InviteUrlParas paras2;
			if (null != paras) {
				paras2 = new InviteUrlParas();
				paras2.setParas(paras);
			} else {
				paras2 = this.isUuidInviteLink(request);
			}
			// app 拷贝链接
			if (this.isAppCopyLink(request) || isAppCopyUuidLink(paras2)) {
				type = LINK_INVITE_APP_COPY;
			}
			// app 邮件
			else if (this.isAppMailLink(request) || isAppMailUuidLink(paras2)) {
				type = LINK_INVITE_APP_MAIL;
			}
			// 普通拷贝链接
			else if (this.isCommonCopyLink(request)
					|| isCommonCopyUuidLink(paras2)) {
				type = LINK_INVITE_COMMON_COPY;
			}
			// 普通邮件链接
			else if (this.isCommonMailLink(request)
					|| this.isCommonMailUuidLink(paras2)) {
				type = LINK_INVITE_COMMON_MAIL;
			}
			// 高中链接
			else if (this.isHSCopyLink(request) || isHSCopyUuidLink(paras2)) {
				type = LINK_INVITE_HS_COPY;
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return type;
	}

	/**
	 * 是否为高中拷贝链接
	 * 
	 * @param req
	 * @return
	 * @throws SQLException
	 */
	private boolean isHSCopyLink(final HttpServletRequest req)
			throws SQLException {
		boolean yes = false;
		if (false == this.isMyLink(req,
				RegInviteLinkLogic.LINK_PARA_INVITE_HS_COPY)) {
			return false;
		}
		int inviterId = Integer.parseInt(StringUtils.isNotEmpty(req.getParameter("i")) 
                && StringUtils.isNumeric(req.getParameter("i")) 
                ? req.getParameter("i") : "-1");
		String inviteCode = req.getParameter("code");
		UserInviteCode uic = UserInviteCodeViewDAO.getInstance().getByUser(inviterId);
		// code 符合
		if (null != uic) {
			if (null != uic.getInviteCode()) {
				if (inviteCode.equals(uic.getInviteCode())) {
					yes = true;
				}
			}
		}

		return yes;
	}

	/**
	 * 是否为高中拷贝链接 uuid
	 * 
	 * @param req
	 * @return
	 * @throws SQLException
	 */
	private boolean isHSCopyUuidLink(final InviteUrlParas paras)
			throws SQLException {
		boolean yes = false;
		if (false == this.isMyLinkUuid(paras,
				RegInviteLinkLogic.LINK_PARA_INVITE_HS_COPY)) {
			return false;
		}
		yes = true;

		return yes;
	}

	/**
	 * 是否为uuid link
	 * 
	 * @param req
	 * @return null 不是 InviteUrlParas 是
	 * @author tai.wang@opi-corp.com (2008-10-24 - 下午05:35:12)
	 */
	private InviteUrlParas isUuidInviteLink(final HttpServletRequest req) {
		if (false == RegUtil.hasParameter(req, "iid")) {
			return null;
		} else {
			String uuid = (req.getParameter("iid"));
			InviteUrlParas paras = null;
			try {
				paras = InviteUrlParasDAO.getInstance().getParas(uuid);
			} catch (SQLException e) {
				paras = null;
				e.printStackTrace();
			}
			return paras;
		}
	}

	private boolean isCommonMailUuidLink(final InviteUrlParas paras) {
		if (null == paras) {
			return false;
		}
		if (true == isMyLinkUuid(paras, LINK_PARA_INVITE_COMMON_MAIL)) {
			boolean yes = false;
			// 邀请人id
			int inviteId = 0; // 邀请renID
			try {
				inviteId = Integer.parseInt(StringUtils.isNotEmpty(paras.getParas().get("i")) 
                        && StringUtils.isNumeric(paras.getParas().get("i")) 
                        ? paras.getParas().get("i") : "-1");
			} catch (Exception e) {
				e.printStackTrace();
				// do nothing ...
			}
			// 邀请人id 没问题
			if (0 != inviteId) {
				// 被邀请的邮件，及验证格式
				String invitedEmail = Escape.trim(paras.getParas().get("e"));
				if (inviteId == 0 || invitedEmail == null
						|| invitedEmail.length() == 0
						|| invitedEmail.length() > 50
						|| !StringFormat.isValidEmailAddr(invitedEmail)) {
					// do nonthing ...
				} else {
					// 检查邀请信息，通过id 和邮件
					// InviteInfo inviteInfo = null;
					User inviter = UserAdapter.getUser(inviteId);
					if (null != inviter) {
						InviteInfo inviteInfo = null;
						try {
							inviteInfo = getInviteInfo(inviteId, invitedEmail);
						} catch (Exception e) {
							e.printStackTrace();
							// do nothing
						}
						if (null != inviteInfo) {
							yes = true;
						}
					}
				}
			}
			return yes;
		} else {
			return false;
		}
	}

	private InviteInfo getInviteInfo(final int inviteId, final String invitedEmail){
		TimeLimitConfiguration tl =  TimeLimitController.getTimeLimit(40);
		tl.setSingleConfig("getInviteInfo", new TimeLimitThread(tl) {
			@Override
			public Object work() {
				try {
					return InviteDAO.getInstance().getInviteInfo(
							inviteId, invitedEmail);
				} catch (SQLException e) {
					e.printStackTrace();
					return null;
				}
			}
		});
		try {
			return (InviteInfo) tl.getSingleReturn();
		} catch (Throwable e) {
			return null;
		}
	}

	/**
	 * 是否为一般邮件连接
	 * 
	 * @param req
	 * @return
	 */
	private boolean isCommonMailLink(final HttpServletRequest req) {
		boolean yes = false;
		if (false == this.isMyLink(req,
				RegInviteLinkLogic.LINK_PARA_INVITE_COMMON_MAIL)) {
			return false;
		}
		// 邀请人id
		int inviteId = 0; // 邀请renID
		try {
			inviteId = Integer.parseInt(StringUtils.isNotEmpty(req.getParameter("i")) 
                    && StringUtils.isNumeric(req.getParameter("i")) 
                    ? req.getParameter("i") : "-1");
		} catch (Exception e) {
			e.printStackTrace();
			// do nothing ...
		}
		// 邀请人id 没问题
		if (0 != inviteId) {
			// 被邀请的邮件，及验证格式
			String invitedEmail = Escape.trim(req.getParameter("e"));
			if (inviteId == 0 || invitedEmail == null
					|| invitedEmail.length() == 0 || invitedEmail.length() > 50
					|| !StringFormat.isValidEmailAddr(invitedEmail)) {
				// do nonthing ...
			} else {
				// 检查邀请信息，通过id 和邮件
				// InviteInfo inviteInfo = null;
				User inviter = UserAdapter.getUser(inviteId);
				if (null != inviter) {
					InviteInfo inviteInfo = null;
					try {
						inviteInfo = getInviteInfo(inviteId, invitedEmail);
					} catch (Exception e) {
						e.printStackTrace();
						// do nothing
					}
					if (null != inviteInfo) {
						yes = true;
					}
				}
			}
		}
		return yes;
	}

	/**
	 * 是否为普通 拷贝链接
	 * 
	 * @param req
	 * @return
	 * @throws SQLException
	 */
	private boolean isCommonCopyLink(final HttpServletRequest req)
			throws SQLException {
		boolean yes = false;
		if (!this
				.isMyLink(req, RegInviteLinkLogic.LINK_PARA_INVITE_COMMON_COPY)) {
			return false;
		}
		// inviteType 符合
		int inviterId = Integer.parseInt(StringUtils.isNotEmpty(req.getParameter("i")) 
                && StringUtils.isNumeric(req.getParameter("i")) 
                ? req.getParameter("i") : "-1");
		String inviteCode = req.getParameter("code");
		UserInviteCode uic = UserInviteCodeViewDAO.getInstance().getByUser(inviterId);
		// code 符合
		if (null != uic) {
			if (null != uic.getInviteCode()) {
				if (inviteCode.equals(uic.getInviteCode())) {
					yes = true;
				}
			}
		}
		return yes;
	}

	/**
	 * 是否为普通 拷贝链接 uuid
	 * 
	 * @param req
	 * @return
	 * @throws SQLException
	 */
	private boolean isCommonCopyUuidLink(final InviteUrlParas paras)
			throws SQLException {
		boolean yes = false;
		if (!this.isMyLinkUuid(paras,
				RegInviteLinkLogic.LINK_PARA_INVITE_COMMON_COPY)) {
			return false;
		}
		yes = true;
		return yes;
	}

	/**
	 * 是否为 app 邮件邀请
	 * 
	 * @param req
	 * @return
	 */
	private boolean isAppMailLink(final HttpServletRequest req) {
		boolean yes = false;
		if (!this.isMyLink(req, RegInviteLinkLogic.LINK_PARA_INVITE_APP_MAIL)) {
			return false;
		}
		// 邀请人id
		int inviteId = 0; // 邀请renID
		try {
			inviteId = Integer.parseInt(StringUtils.isNotEmpty(req.getParameter("i")) 
                    && StringUtils.isNumeric(req.getParameter("i")) 
                    ? req.getParameter("i") : "-1");
		} catch (Exception e) {
			e.printStackTrace();
			// do nothing ...
		}
		// 邀请人id 没问题
		if (0 != inviteId) {
			// 被邀请的邮件，及验证格式
			String invitedEmail = Escape.trim(req.getParameter("e"));
			if (inviteId == 0 || invitedEmail == null
					|| invitedEmail.length() == 0 || invitedEmail.length() > 50
					|| !StringFormat.isValidEmailAddr(invitedEmail)) {
				// do nonthing ...
			} else {
				// 检查邀请信息，通过id 和邮件
				// InviteInfo inviteInfo = null;
				User inviter = UserAdapter.getUser(inviteId);
				if (null != inviter) {
					InviteInfo inviteInfo = null;
					try {
						inviteInfo = getInviteInfo(inviteId, invitedEmail);
					} catch (Exception e) {
						e.printStackTrace();
						// do nothing
					}
					if (null != inviteInfo) {
						yes = true;
					}
				}
			}
		}
		return yes;
	}

	private boolean isAppMailUuidLink(final InviteUrlParas paras) {
		boolean yes = false;
		if (null == paras) {
			return false;
		}
		if (false == this.isMyLinkUuid(paras, LINK_PARA_INVITE_APP_MAIL)) {
			return false;
		}
		// 邀请人id
		int inviteId = 0; // 邀请renID
		try {
			inviteId = Integer.parseInt(StringUtils.isNotEmpty(paras.getParas().get("i")) 
                    && StringUtils.isNumeric(paras.getParas().get("i")) 
                    ? paras.getParas().get("i") : "-1");
		} catch (Exception e) {
			e.printStackTrace();
			// do nothing ...
		}
		// 邀请人id 没问题
		if (0 != inviteId) {
			// 被邀请的邮件，及验证格式
			String invitedEmail = Escape.trim(paras.getParas().get("e"));
			if (inviteId == 0 || invitedEmail == null
					|| invitedEmail.length() == 0 || invitedEmail.length() > 50
					|| !StringFormat.isValidEmailAddr(invitedEmail)) {
				// do nonthing ...
			} else {
				// 检查邀请信息，通过id 和邮件
				// InviteInfo inviteInfo = null;
				User inviter = UserAdapter.getUser(inviteId);
				if (null != inviter) {
					InviteInfo inviteInfo = null;
					try {
						inviteInfo = getInviteInfo(inviteId, invitedEmail);
					} catch (Exception e) {
						e.printStackTrace();
						// do nothing
					}
					if (null != inviteInfo) {
						yes = true;
					}
				}
			}
		}
		return yes;
	}

	/**
	 * 是否为 app 拷贝链接
	 * 
	 * @param req
	 * @return
	 * @throws SQLException
	 */
	private boolean isAppCopyLink(final HttpServletRequest req)
			throws SQLException {
		boolean yes = false;
		if (!this.isMyLink(req, RegInviteLinkLogic.LINK_PARA_INVITE_APP_COPY)) {
			return false;
		}
		// inviteType 符合
		int inviterId = Integer.parseInt(StringUtils.isNotEmpty(req.getParameter("i")) 
                                && StringUtils.isNumeric(req.getParameter("i")) 
                                ? req.getParameter("i") : "-1");
		String inviteCode = req.getParameter("code");
		UserInviteCode uic = UserInviteCodeViewDAO.getInstance().getByUser(inviterId);
		// code 符合
		if (null != uic) {
			if (null != uic.getInviteCode()) {
				if (inviteCode.equals(uic.getInviteCode())) {
					yes = true;
				}
			}
		}
		return yes;
	}

	/**
	 * 是否为 app 拷贝链接
	 * 
	 * @param req
	 * @return
	 * @throws SQLException
	 */
	private boolean isAppCopyUuidLink(final InviteUrlParas req)
			throws SQLException {
		boolean yes = false;
		if (!this.isMyLinkUuid(req,
				RegInviteLinkLogic.LINK_PARA_INVITE_APP_COPY)) {
			return false;
		}
		yes = true;
		return yes;
	}

	/**
	 * 是否为对应的链接
	 * 
	 * @param paras
	 * @return
	 */
	public boolean isMyLink(HttpServletRequest req, final String[] paras) {
		boolean yes = false;
		for (int i = 0; i < paras.length; i++) {
			if (RegUtil.hasParameter(req, paras[i])) {
				yes = true;
			} else {
				yes = false;
				break;
			}
		}
		return yes;
	}

	private boolean isMyLinkUuid(InviteUrlParas req, final String[] paras) {
		boolean yes = false;
		if (null == req) {
			return false;
		}
		for (int i = 0; i < paras.length; i++) {
			Map<String, String> map = req.getParas();
			if (null != map.get(paras[i])) {
				yes = true;
			} else {
				yes = false;
				break;
			}
		}
		return yes;
	}

}

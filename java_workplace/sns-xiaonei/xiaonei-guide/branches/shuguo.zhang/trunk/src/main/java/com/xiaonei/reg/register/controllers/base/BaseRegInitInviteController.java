/**
 *
 */
package com.xiaonei.reg.register.controllers.base;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.sql.SQLException;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.AsSuperController;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegTools;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.common.users.UserInfo;
import com.xiaonei.reg.register.dao.InviteDAO;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.form.base.userganerate.IInvite;
import com.xiaonei.reg.register.logic.pretreatment.InviteUrlParasLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegInviteLinkLogic;
import com.xiaonei.reg.register.model.InviteInfo;
import com.xiaonei.reg.thread.timelimit.TimeLimitConfiguration;
import com.xiaonei.reg.thread.timelimit.TimeLimitController;
import com.xiaonei.reg.thread.timelimit.TimeLimitThread;

/**
 * InitInviteRegisterAction.java 继续完善///weige
 * 
 * @author tai.wang@opi-corp.com (2008-10-13 - 上午11:12:05)
 */
@AsSuperController
public abstract class BaseRegInitInviteController extends BaseRegInitController {

	private static final String errForward = "initcommonreg";
	private static final String succForward = "success";

	@Override
	final public RegForward executeRIt(RegMapping mapping, RegForm form,
			HttpServletRequest request, HttpServletResponse response) {
		IInvite regForm = (IInvite) form;
		Map<String, String> paras = null;
		try {
			paras = InviteUrlParasLogic.getInstance().getParas(request);
		} catch (Exception e) {
			return mapping.findForward(errForward);
		}

		// 判断连接的合法性
		boolean isValidLink = isInviteLink(request, regForm, paras);
		if (isValidLink == false) {
			return mapping.findForward(errForward);
		}

		// 邀请初始化页面

		RegForward rt = initPage(request, (BaseRegForm) form, mapping, paras);
		if (rt != null) {
			return rt;
		}
		
		// 兼容iframe的页面
		initIframe(request);
		
		request.setAttribute(getRegFormName(), regForm);
		return mapping.findForward(succForward);

	}

    private void initIframe(HttpServletRequest request) {
        String sUrl = StringUtils.trimToEmpty(request.getParameter("surl"));
        if(StringUtils.isNotEmpty(sUrl)){
            try {
                sUrl = URLDecoder.decode(sUrl, "utf-8");
                if(internalURL(sUrl)){
                    request.setAttribute("iframeUrl", sUrl);
                }
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        }
    }

	/**
	 * 初始化页面保存一些参数在jsp上
	 * 
	 * @param request
	 * @param regForm
	 * @param mapping
	 * @param paras
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-22 - 上午11:12:36
	 */
	private RegForward initPage(HttpServletRequest request,
			BaseRegForm regForm, RegMapping mapping, Map<String, String> paras) {
		// 解析邀请链接参数
		// uuid方式保存参数
		int invId = 0;
		String appIdStr = null;
		String uuid = request.getParameter("iid");
		regForm.setUuid(uuid);
		IInvite ii = (IInvite) regForm;
		IAccType ie = (IAccType) regForm;
		appIdStr = paras.get("appId");
		if (!StringUtils.isEmpty(appIdStr)) {
		    //get app Name
		    AppMenuItem appMenuItem = AppMenuItemCacheMgr.getInstance().get(Integer.parseInt(appIdStr));
		    if(appMenuItem != null){
		    String appName = appMenuItem.getAppName();
		        ii.setAppName(appName);
		        request.setAttribute("appName", appName);
		    }
			ii.setAppId(Integer.parseInt(appIdStr));
			ii.setAppToken(paras.get("appToken"));
		}
		ii.setInviteeEmail(paras.get("e"));
		if (StringUtils.isEmpty(ie.getRegEmail())) {
			ie.setRegEmail(paras.get("e"));
		}
		invId = Integer.valueOf(paras.get("i"));
		ii.setInviterId(Integer.valueOf(paras.get("i")));

		String inviteType = paras.get("inviteType");
		if (inviteType != null && !inviteType.equals("")) {
			ii.setInviteType(Integer.parseInt(inviteType));
		}
		appIdStr = request.getParameter("appId");
		if (appIdStr != null && !appIdStr.equals("")) {
			ii.setAppId(Integer.parseInt(appIdStr));
		}
		String inviterId = paras.get("i");
		if (inviterId != null && !inviterId.equals("")) {
			invId = Integer.parseInt(inviterId);
			request.setAttribute("inviterId", inviterId);
			ii.setInviterId(invId);
		}
		String inviteeEmail = paras.get("e");
		if (inviteeEmail != null && !inviteeEmail.equals("")) {
			ii.setInviteeEmail(inviteeEmail);
			if (StringUtils.isEmpty(ie.getRegEmail())) {
				ie.setRegEmail(inviteeEmail);
			}
			request.setAttribute("inviteeEmail", inviteeEmail);

		} else if (ie.getRegEmail() != null) {
			request.setAttribute("e", ie.getRegEmail());
		}
		String appToken = paras.get("appToken");
		if (appToken != null && !appToken.equals("")) {
			ii.setAppToken(appToken);
		}

		String from = paras.get("from");
		if (from != null && !from.equals("")) {
			regForm.setFrom(from);
		}

		String groupName = paras.get("groupName");
		if (groupName != null && !groupName.equals("")) {
			ii.setGroupName(groupName);
		}
		try {
			UserInfo inviter = UserAdapter.get(invId);
			if (inviter != null) {
				request.setAttribute("inviter", inviter);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * 判断是否点击邀请链接
	 * 
	 * @param request
	 * @param regForm
	 * @return
	 */
	private boolean isInviteLink(final HttpServletRequest request,
			final IInvite regForm, Map<String, String> paras) {

		InviteInfo inviteInfo = null;
		int inviteId = 0; // 邀请renID
		User inviter = null;
		/* 区分不同的邀请类型邀请 */
		switch (RegInviteLinkLogic.getInstance()
				.whichInviteLink(request, paras)) {
		case RegInviteLinkLogic.LINK_INVITE_HS_COPY:
		case RegInviteLinkLogic.LINK_INVITE_COMMON_COPY:
		case RegInviteLinkLogic.LINK_INVITE_APP_COPY:
			/* app拷贝链接的邀请注册 */
			// 邀请人id
			try {
				inviteId = Integer.parseInt(paras.get("i"));
			} catch (Exception e) {
			}
			if (inviteId == 0)
				return false;
			inviter = getUser(inviteId);
			if (null == inviter)
				return false;
			inviteInfo = new InviteInfo();
			inviteInfo.setOwner_id(inviter.getId());
			inviteInfo.setOwner_name(inviter.getName());
			try {
				inviteInfo.setHeadurl(UserAdapter.get(inviter).getMain_Url());
			} catch (Throwable e) {
				inviteInfo.setHeadurl(UserAdapter.get(inviter).getLarge_Url());
			}
			// return true;
			break;
		case RegInviteLinkLogic.LINK_INVITE_APP_MAIL:
		case RegInviteLinkLogic.LINK_INVITE_COMMON_MAIL:
		default:
			// 邀请人id
			try {
				inviteId = Integer.parseInt(paras.get("i"));
			} catch (Exception e) {
			}

			// 被邀请的邮件，及验证格式
			String invitedEmail = Escape.trim(paras.get("e"));
			if (inviteId == 0 || invitedEmail == null
					|| invitedEmail.length() == 0 || invitedEmail.length() > 50
					|| !StringFormat.isValidEmailAddr(invitedEmail))
				return false;

			// 检查邀请信息，通过id 和邮件
			// InviteInfo inviteInfo = null;
			inviter = getUser(inviteId);
			if (null == inviter)
				return false;
			try {
				inviteInfo = getInviteInfo(inviteId, invitedEmail);
				if (inviteInfo == null)
					return false;
				try {
					inviteInfo.setHeadurl(UserAdapter.get(inviter)
							.getMain_Url());
				} catch (Throwable e) {
					inviteInfo.setHeadurl(UserAdapter.get(inviter)
							.getLarge_Url());
				}
			} catch (Throwable e) {
			}

			request.setAttribute("e", invitedEmail);
		}
		request.setAttribute("inviteInfo", inviteInfo);

		return true;

	}

	private InviteInfo getInviteInfo(final int inviteId,
			final String invitedEmail) throws Throwable {
		TimeLimitConfiguration tl = TimeLimitController.getTimeLimit(50, 5l);
		tl.setSingleConfig("get_invite_info", new TimeLimitThread(tl) {
			@Override
			public Object work() {
				try {
					return InviteDAO.getInstance().getInviteInfo(inviteId,
							invitedEmail);
				} catch (SQLException e) {
					e.printStackTrace();
					return null;
				}
			}
		});
		return (InviteInfo) tl.getSingleReturn();
	}

	private User getUser(final int id) {
		TimeLimitConfiguration tl = TimeLimitController.getTimeLimit(15);
		tl.setSingleConfig("invite_getUser", new TimeLimitThread(tl) {
			@Override
			public Object work() {
				return UserAdapter.getUser(id);
			}
		});
		try {
			return (User) tl.getSingleReturn();
		} catch (Throwable e) {
			return null;
		}
	}

	public boolean internalURL(String url) {
        String s = "(.*\\.)?(renren|xiaonei|kaixin)\\.(com|cn)";
        String host = RegTools.getHost(url);
        boolean r = host.matches(s);
        return r;
    }
}

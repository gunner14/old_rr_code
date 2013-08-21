package com.xiaonei.reg.guide.action;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.datum.config.BuildSchedule;
import com.xiaonei.platform.component.tools.UserUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.usercount.UserCount;
import com.xiaonei.platform.core.usercount.UserCountMgr;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.additional.RegInviterInviteeLogic;
import com.xiaonei.reg.register.model.RegInviteMap;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * 新注册的用户引导的最后一步，用户登录少于6次时所要到的页面，大学阶段
 * 
 * @author seraph
 */
public class DXGuideAction extends ProtectAction {

	public ActionForward executePt(ActionMapping mapping, ActionForm form,	HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		User host = BaseThreadUtil.currentHost(request);
		GuideLogger.printLog(" host:" + host.getId() + " NOHERE! ",	GuideLogger.ERROR);
		// 防止直接访问将要分发的action.do
		String vid = (String) request.getAttribute("G" + String.valueOf(host.getId()));
		if (vid == null || !String.valueOf(host.getId()).equals(vid)) {
			return null;
		}
		String forward = "ok";
		try {
			UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(host.getId());
			request.setAttribute("userBasic", ub);
			RegInviteMap inviter = RegInviterInviteeLogic.getInstance().getRegInviteMapByInvitee(host.getId());
			int inviterId = 0;
			if (inviter != null) {
				inviterId = inviter.getInviterId();
			}
			request.setAttribute("inviter", UserAdapter.getUser(inviterId));
			request.setAttribute("loginCount", host.getLoginCount());
			initUserCounts(request, host);			
			Map<String, Object> starMap = UserUtil.getPeopleStar(host, 9);
			request.setAttribute("starList", starMap.get("starList"));
			request.setAttribute("starUniv", starMap.get("starUniv"));

		} catch (Exception e) {
			e.printStackTrace();

		} finally {

		}
		BuildSchedule bu = new BuildSchedule(host);
		request.setAttribute("buildSchedule", bu);
		return mapping.findForward(forward);
	}

	private void initUserCounts(HttpServletRequest request, User host) {
		try {
			List<UserCount> counters = UserCountMgr.getCounts(host.getId());
			if (counters != null) {
				for (UserCount uc : counters) {
					if (uc != null) {
						switch (uc.getType()) {
						case UserCountMgr.POKE:
							request.setAttribute("pokeCount", uc.getCount());
							break;
						case UserCountMgr.GUEST_REQUEST:
							request.setAttribute("buddyApplyCount", uc.getCount());
							break;
						case UserCountMgr.REG_APPLY:
							request.setAttribute("highSchoolApplyCount", uc.getCount());
							break;
						case UserCountMgr.APP_REQUEST_TOTAL:
							request.setAttribute("appApplyCount", uc.getCount());
							break;
						case UserCountMgr.RECOMMENDED_FRIEND:
							request.setAttribute("recommendedCount", uc.getCount());
							break;
						}
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}

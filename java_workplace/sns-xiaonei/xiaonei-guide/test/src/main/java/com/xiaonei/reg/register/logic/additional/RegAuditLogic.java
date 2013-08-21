/**
 *
 */
package com.xiaonei.reg.register.logic.additional;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.logic.pretreatment.RegInviteLinkLogic;

/**
 * RegAuditLogic
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-16 - 下午03:55:22
 */
public class RegAuditLogic {
	private static final String MEM_CACHE_AUDIT_WAIT_KEY = "mem_cache_audit_wait_key_";

	/**
	 * 提交审核信息去等待页面
	 * 
	 * @param mapping
	 * @param stage
	 * @param waitPage
	 * @param auditCode
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-19 - 下午04:28:51
	 */
	public static RegForward goWait(RegMapping mapping, int stage,
			String waitPage, int auditCode) {
		String path = mapping.findForward(waitPage).getPath();
		RegForward forward = new RegForward(path);
		forward.setRedirect(true);
		return forward;
	}


	/**
	 * 判断等待审核成功传入的key是否存在
	 * 
	 * @param key
	 * @return exist true
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-25 - 下午03:18:56
	 */
	public static boolean isMemKeyExist(final Long key) {
		MemCacheManager mem = MemCacheFactory.getInstance()
				.getManagerByPoolName(MemCacheKeys.pool_guide);
		String memKey = MEM_CACHE_AUDIT_WAIT_KEY + key;
		Object obj = mem.get(memKey);

		if (obj != null) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * 白领注册流程<br>
	 * 条件：app邀请 true
	 * 
	 * @param request
	 * @return boolean
	 */
	public boolean isWhiteCollerAuditFlow(HttpServletRequest request) {
		int inviteType = RegInviteLinkLogic.getInstance().whichInviteLink(
				request, null);
		try {
			if (Stage.STAGE_MAJOR == Integer.valueOf(request
					.getParameter("stage"))
					|| Stage.STAGE_NONE == Integer.valueOf(request
							.getParameter("stage"))) {
				if (RegInviteLinkLogic.LINK_INVITE_APP_COPY == inviteType
						|| RegInviteLinkLogic.LINK_INVITE_APP_MAIL == inviteType) {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} catch (Exception e) {
			return false;
		}
	}
}

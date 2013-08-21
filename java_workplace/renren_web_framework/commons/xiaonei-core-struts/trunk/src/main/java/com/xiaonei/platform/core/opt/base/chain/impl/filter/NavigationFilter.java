package com.xiaonei.platform.core.opt.base.chain.impl.filter;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.AbstractBizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;
import com.xiaonei.platform.core.opt.ice.WUserConfigInfoAdapter;
import com.xiaonei.platform.core.usercount.UserCountMgr;
import com.xiaonei.platform.core.menu.UserMenuManager;

/**
 * 
 * 取与导航相关的数据
 * 
 * @author Li Weibo
 * @since 2008-12-28 下午10:18:05
 */
public class NavigationFilter extends AbstractBizFilter {

	public static final String HOST_USER_CONFIG_INFO = "HOST_USER_CONFIG_INFO";

	/**
	 * 是否屏蔽掉取UserConfig的过程
	 */
	private boolean disableUserConfig = false;

	/**
	 * 是否屏蔽掉取导航菜单的过程
	 */
	private boolean disableMenu = false;

	/**
	 * 锁定标志
	 */
	private boolean frozen = false;

	@Override
	public boolean executeMe(WebContext webContext) throws BizFilterException {
		frozen = true;
		HttpServletRequest request = webContext.getRequest();
		User host = BaseThreadUtil.currentHost(request);
		if (host == null) {
			return false;
		}
		try {
			if (!disableUserConfig) {
				// 左侧的“导航”配置
				UserConfigInfo config = WUserConfigInfoAdapter.getInstance()
						.getUserConfig(host.getId());
				// 将登陆者的UserConfigInfo对象放到request中，不用每次都取，子类可以直接从request中获得
				request.setAttribute(HOST_USER_CONFIG_INFO, config);
			} else {
				if (logger.isDebugEnabled()) {
					logger.debug("skip the process of getting UserConfig");
				}
			}
			
			if (!disableMenu) {
				request.setAttribute("userMenu", UserMenuManager.getInstance()
						.get(host.getId()));
				logger.warn("Deprecated web framework. The logic about menu is not supported any more.");
				
			} else {
				if (logger.isDebugEnabled()) {
					logger.debug("skip the process of getting menu");
				}
			}

			Map<Integer, Integer> userCountMap = UserCountMgr.getCountMap(host.getId());
			if (userCountMap != null) { // 如果ICETimeout，这里就可能是null了
				request.setAttribute("userCountMap", userCountMap);
				request.setAttribute("newMessageCount", userCountMap.get(UserCountMgr.NEW_MESSAGE));
				request.setAttribute("newNotifyCount", userCountMap.get(UserCountMgr.NOTIFY));
			}
		} catch (Throwable e) {
			e.printStackTrace();
		}
		return false;
	}

	/**
	 * 屏蔽掉取UserConfig的过程
	 */
	public void disableUserConfig() {
		if (frozen) {
			throw new IllegalStateException();
		}
		disableUserConfig = true;
		if (logger.isInfoEnabled()) {
			logger.info("the process of getting UserConfig is disabled");
		}
	}

	/**
	 * 屏蔽掉取导航菜单的过程
	 */
	public void disableMenu() {
		if (frozen) {
			throw new IllegalStateException();
		}
		disableMenu = true;
		if (logger.isInfoEnabled()) {
			logger.info("the process of getting menu is disabled");
		}
	}
}

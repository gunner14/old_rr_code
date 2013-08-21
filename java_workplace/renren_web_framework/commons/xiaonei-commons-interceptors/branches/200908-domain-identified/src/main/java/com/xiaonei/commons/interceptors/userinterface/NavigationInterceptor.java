package com.xiaonei.commons.interceptors.userinterface;

import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.annotation.Interceptor;
import net.paoding.rose.web.var.Model;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.menu.UserMenuManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.ice.WUserConfigInfoAdapter;
import com.xiaonei.platform.core.usercount.UserCountMgr;

/**
 * {@link NavigationInterceptor} 拦截对Web控制器的调用，取与导航相关的数据
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class NavigationInterceptor extends ControllerInterceptorAdapter {

    public static final String HOST_USER_CONFIG_INFO = "HOST_USER_CONFIG_INFO";

    @Autowired
    private HostHolder hostHolder;

    /**
     * 是否屏蔽掉取UserConfig的过程
     */
    private boolean userConfigDisabled = false;

    /**
     * 是否屏蔽掉取导航菜单的过程
     */
    private boolean menuDisabled = false;

    public boolean isUserConfigDisabled() {
        return userConfigDisabled;
    }

    public void setUserConfigDisabled(boolean userConfigDisabled) {
        this.userConfigDisabled = userConfigDisabled;
        if (logger.isInfoEnabled()) {
            logger.info("userConfigDisabled=" + userConfigDisabled);
        }
    }

    public boolean isMenuDisabled() {
        return menuDisabled;
    }

    public void setMenuDisabled(boolean menuDisabled) {
        this.menuDisabled = menuDisabled;
        if (logger.isInfoEnabled()) {
            logger.info("menuDisabled=" + menuDisabled);
        }
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        User host = hostHolder.getUser();
        if (host == null) {
            return true;
        }
        Model model = inv.getModel();
        if (!userConfigDisabled) {
            // 左侧的“导航”配置
            UserConfigInfo config = WUserConfigInfoAdapter.getInstance()
                    .getUserConfig(host.getId());
            // 将登陆者的UserConfigInfo对象放到request中，不用每次都取，子类可以直接从request中获得
            model.add(HOST_USER_CONFIG_INFO, config);
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("skip the process of getting UserConfig");
            }
        }

        if (!menuDisabled) {
            model.add("userMenu", UserMenuManager.getInstance().get(host.getId()));
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("skip the process of getting menu");
            }
        }

        Map<Integer, Integer> userCountMap = UserCountMgr.getCountMap(host.getId());
        if (userCountMap != null) { // 如果ICETimeout，这里就可能是null了
            model.add("userCountMap", userCountMap);
            model.add("newMessageCount", userCountMap.get(UserCountMgr.NEW_MESSAGE));
            model.add("newNotifyCount", userCountMap.get(UserCountMgr.NOTIFY));
        }

        return true;
    }
}

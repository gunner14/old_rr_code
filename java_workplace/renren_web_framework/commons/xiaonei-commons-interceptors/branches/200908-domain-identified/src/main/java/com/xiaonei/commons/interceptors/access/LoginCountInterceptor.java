package com.xiaonei.commons.interceptors.access;

import java.util.Date;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.platform.core.utility.sched.impl.IpLoginLogic;

/**
 * {@link AutoLoginInterceptor}
 * 拦截对Web控制器的调用，如果该请求是由于用户访问，按一定的时间间隔，更新该用户的登录次数
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class LoginCountInterceptor extends ControllerInterceptorAdapter {

    private long loginCountForceTime = 24 * 60 * 60 * 1000;

    private HostHolder hostHolder;

    public LoginCountInterceptor() {
        setPriority(29700);
    }

    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }

    public void setLoginCountForceTime(long loginCountForceTime) {
        this.loginCountForceTime = loginCountForceTime;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        try {
            HttpServletRequest request = inv.getRequest();
            HttpServletResponse response = inv.getResponse();

            // 之前如果没有设置访问用户(通过ticket或自动登录等等各种方式，此拦截器不发生实际作用
            User guester = hostHolder.getUser();
            if (guester == null) {
                return true;
            }
            //
            String guesterIdInCookie = CookieManager.getInstance().getCookie(request,
                    PassportManager.loginKey_hostId);
            if (guesterIdInCookie == null || guesterIdInCookie.length() == 0
                    || guesterIdInCookie.equals("null")) {

                CookieManager.getInstance().saveCookie(response, PassportManager.loginKey_hostId,
                        Integer.toString(guester.getId()), "/");

                UserDesc ud = WUserDescAdapter.getInstance().get(guester.getId());
                if (ud.getLastLoginTime() == null) {
                    ud.setLastLoginTime(new Date(1));
                }
                if (ud.getLastLoginTime().getTime() + loginCountForceTime < System
                        .currentTimeMillis()
                        || guester.getLoginCount() == 0) {
                    guester.setLoginCount(guester.getLoginCount() + 1);
                    try {
                        guester.setLastIp(BizFilterUtil.getClientIP(request));
                        IpLoginLogic.saveIpLogin(guester.getId(), guester.getLastIp(), 0);
                    } catch (Exception e) {
                        logger.error(e.getMessage(), e);
                    }
                    WUserAdapter.getInstance().incLoginCount(guester.getId());
                    ud.setId(guester.getId());
                    ud.setLastLoginTime(new Date());
                    WUserDescAdapter.getInstance().set(ud);
                }
            }
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
        }
        return true;
    }
}

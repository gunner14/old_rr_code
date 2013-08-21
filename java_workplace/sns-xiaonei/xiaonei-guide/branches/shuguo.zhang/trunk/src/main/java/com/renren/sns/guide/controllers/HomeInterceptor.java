package com.renren.sns.guide.controllers;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.NotForSubModules;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
/**
 * 对于直接进入Home页的用户判断正确的去向，这个Interceptor是看要不要去Home页面的
 * 
 * @author wei.xiang(wei.xiang@opi-corp.com)
 *
 */
@NotForSubModules
public class HomeInterceptor extends ControllerInterceptorAdapter {    
    
    @Override
    public Object before(Invocation inv) throws Exception {
        User host = BaseThreadUtil.currentHost(inv.getRequest());
        boolean wantstay = false;
        try {
            String stay = inv.getParameter("stay");
            wantstay = MyStringUtils.equals("true", stay);
        } catch (Exception e) {
            
        }
        if (host.getUserStateInfo().isGuideDone() && !wantstay) {
            return String.format("r:%s/home", OpiConstants.urlWww);
        }
        return true;
    }
}

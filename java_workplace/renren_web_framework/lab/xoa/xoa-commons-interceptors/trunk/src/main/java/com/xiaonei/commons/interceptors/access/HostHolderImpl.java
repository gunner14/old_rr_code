package com.xiaonei.commons.interceptors.access;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.InvocationLocal;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;

/**
 * 
 */
@Component("hostHolder")
public class HostHolderImpl implements HostHolder {

    @Autowired
    InvocationLocal inv;

    @Override
    public User getUser() {
        Invocation inv = this.inv.getCurrent(false);
        if (inv != null) {
            return BaseThreadUtil.currentHost(inv.getRequest());
        } else {
            return null;
        }
    }

    public void setUser(User user) {
        BaseThreadUtil.setHost(inv.getRequest(), user);
    }

}

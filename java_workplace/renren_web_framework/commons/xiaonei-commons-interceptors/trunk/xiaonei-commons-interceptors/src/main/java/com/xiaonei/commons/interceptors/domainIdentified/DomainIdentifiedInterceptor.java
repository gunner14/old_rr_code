package com.xiaonei.commons.interceptors.domainIdentified;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 在Rose下配置com.xiaonei.platform.core.domainIdentified.
 * DomainIdentifiedFilter后，可不配置filter-mapping(此时filter相当于Listener)
 * <p>
 * Paoding-Rose
 * Portal框架采用并发forward技术，所以需要此类，特别是在DomainIdentifiedFilter没有配置FORWARD的情况下
 * 
 * @author zhiliang.wang
 * 
 */
public class DomainIdentifiedInterceptor extends ControllerInterceptorAdapter {

    @Override
    public Object before(Invocation inv) throws Exception {
        String domain = OpiConstants.getCurrentDomain();
        if (domain == null) {
            domain = OpiConstants.findDomain(inv.getRequest());
            OpiConstants.setCurrentDomain(domain);
            inv.addModel("domainIdentified", OpiConstants.getCurrentDomainIdentifiedProperties());
        }
        return true;
    }

    @Override
    public void afterCompletion(Invocation inv, Throwable ex) throws Exception {
        OpiConstants.clearCurrentDomain();
    }
}

package com.xiaonei.commons.interceptors.access;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * {@link SafeCenterInterceptor}
 * 拦截用户请求，通过user的safeStatus判断用户是否需要到安全中心做验证，如果需要的话把流程交给安全中心
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
public class SafeCenterInterceptor extends ControllerInterceptorAdapter {

    boolean disabled;

    long refreshTime;

    private HostHolder hostHolder;

    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        User host = hostHolder.getUser();
        if (host != null && host.getSafeStatus() > 0 && !isSafeCenterDisabled()) {
            return "r:" + OpiConstants.urlSafeCenter;
        }
        return true;
    }

    protected boolean isSafeCenterDisabled() {
        if (refreshTime + 1000 < System.currentTimeMillis()) {
            MemCacheManager memcache = MemCacheFactory.getInstance().getManagerByPoolName(
                    MemCacheKeys.pool_default);
            Object safeDisabled = memcache.get(OpiConstants.keySafeCenterDisabled.toString());
            if (safeDisabled == null || "true".equals(safeDisabled.toString())) {
                disabled = true;
            } else {
                disabled = false;
            }
            refreshTime = System.currentTimeMillis();
        }
        return disabled;
    }
}

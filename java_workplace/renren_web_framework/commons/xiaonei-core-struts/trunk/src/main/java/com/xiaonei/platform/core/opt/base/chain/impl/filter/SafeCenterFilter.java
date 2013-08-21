package com.xiaonei.platform.core.opt.base.chain.impl.filter;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.AbstractBizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 
 * 通过user的safeStatus判断用户是否需要到安全中心做验证
 * 
 * @author 王志亮 [zhiliang.wang@opi-corp.com]
 * 
 */
public class SafeCenterFilter extends AbstractBizFilter {

    static boolean disabled;

    static long refreshTime;

    @Override
    public boolean executeMe(WebContext context) throws BizFilterException {
        HttpServletRequest request = context.getRequest();
        User host = BaseThreadUtil.currentHost(request);
        if (host != null && (host.getSafeStatus()&(31<<1)) > 0 && !isSafeCenterDisabled()) {
            throw new BizFilterException(BizFilterException.CODE_SAFE_CENTER);
        }
        return false;
    }

    public static boolean isSafeCenterDisabled() {
        if (refreshTime + 5000 < System.currentTimeMillis()) {
            MemCacheManager memcache = MemCacheFactory.getInstance().getManagerByPoolName(
                    MemCacheKeys.pool_default);
            Object safeDisabled = memcache.get(OpiConstants.keySafeCenterDisabled.toString());
            if (safeDisabled != null && "true".equals(safeDisabled.toString())) {
                disabled = true;
            } else {
                disabled = false;
            }
            refreshTime = System.currentTimeMillis();
        }
        return disabled;
    }

}

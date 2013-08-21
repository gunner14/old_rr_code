package com.renren.sns.wiki.interceptor;

import java.util.regex.Pattern;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.adapter.WikiProfileAdapter;
import com.renren.sns.wiki.utils.RemoteAddrUtil;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.sns.platform.core.rose.wiki.WikiUserIdsManager;

/**
 * 在开放之前用来做访问控制，上线之后去掉
 * 
 * @author yi.li@renren-inc.com since 2012-6-20
 * 
 */
public class AcessControlInterceptor extends ControllerInterceptorAdapter {

    private static Pattern ipPattern = Pattern
            .compile("(^10\\.2\\..*)|(127\\.0\\.0\\.1)|(^10\\.3\\..*)");

    private static Log logger = LogFactory.getLog(AcessControlInterceptor.class);

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiUserIdsManager wikiUserIdManager;

    private WikiProfileAdapter wikiProfileAdapter = WikiProfileAdapter.getInstance();

    @Override
    public int getPriority() {
        return 195;
    }

    @Override
    protected Object before(Invocation inv) throws Exception {
        if (isInternalIP(inv) || isWhiteList(inv) || isCacheUser(inv)) {
            return super.before(inv);
        }
        return "e:404";
    }

    private boolean isCacheUser(Invocation inv) {
        try {
            if (null == hostHolder.getUser()) {
                return false;
            }

            if (wikiProfileAdapter.isCacheUser(hostHolder.getUser().getId())) {
                logger.debug("is in cache user : " + hostHolder.getUser().getId());
                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    private boolean isWhiteList(Invocation inv) {
        try {
            if (null == hostHolder.getUser()) {
                return false;
            }

            if (wikiUserIdManager.isWikiUser(hostHolder.getUser().getId())) {
                logger.debug("is in white list : " + hostHolder.getUser().getId());
                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return false;
    }

    private boolean isInternalIP(Invocation inv) {
        String ip = RemoteAddrUtil.getClientIP(inv.getRequest());
        logger.debug("client ip : " + ip);
        if (ipPattern.matcher(ip.trim()).matches()) {
            logger.debug("is internal ip: " + ip);
            return true;
        }

        return false;
    }

}

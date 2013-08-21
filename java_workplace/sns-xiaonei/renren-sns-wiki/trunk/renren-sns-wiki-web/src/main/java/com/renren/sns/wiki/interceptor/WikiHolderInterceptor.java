package com.renren.sns.wiki.interceptor;

import org.apache.commons.lang.math.NumberUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.enums.WikiStateType;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.service.WikiService;
import com.renren.sns.wiki.service.admin.AdminService;
import com.renren.sns.wiki.utils.JSONResultUtil;
import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

/**
 * 拦截器中用来获取wiki信息并保存下来供之后的流程使用
 * 
 * @author yi.li@renren-inc.com since 2012-5-21
 * 
 */
@Interceptor(oncePerRequest = true)
public class WikiHolderInterceptor extends ControllerInterceptorAdapter {

    private Log logger = LogFactory.getLog(WikiHolderInterceptor.class);

    @Autowired
    private WikiHolder wikiHolder;

    @Autowired
    private WikiService wikiService;

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private AdminService adminService;

    @Override
    public int getPriority() {
        return 190;
    }

    @Override
    protected Object before(Invocation inv) throws Exception {
        Wiki wiki = wikiHolder.getWiki();
        if (null != wiki) {
            return true;
        }

        String wikiIdStr = inv.getParameter("wikiId");// 固定使用wikiId为wiki ID参数名

        if (null == wikiIdStr || "".equals(wikiIdStr.trim())) {
            return true;
        }

        // TODO 添加别名支持
        if (!NumberUtils.isDigits(wikiIdStr)) {
            logger.warn("Wiki alias not support right now! input string: " + wikiIdStr); // 别名暂时不支持
            return getNotFoundReturn(inv);
        }

        int wikiId;
        try {
            wikiId = Integer.parseInt(wikiIdStr);
        } catch (NumberFormatException e) {
            logger.warn("Wiki ID not integer, something may wrong, input string: " + wikiIdStr);
            return getNotFoundReturn(inv);
        }

        wiki = wikiService.getWikiById(wikiId);
        if (wiki == null) {
            return getNotFoundReturn(inv);
        }

        if (!hasAuthority(wiki)) {
            return getCloseReturn(inv, wiki);
        }

        if (logger.isDebugEnabled()) {
            logger.debug("Wiki found wikiId: " + wikiId + " wikiName: " + wiki.getName());
        }
        wikiHolder.setWiki(wiki);

        return super.before(inv);
    }

    private boolean hasAuthority(Wiki wiki) {
        User user = hostHolder.getUser();
        if (null == user) {
            return false;
        }

        if (wiki.getState() >= WikiStateType.CLOSE.getState()
                && !adminService.isSuperAdminInFly(user.getId())) {
            logger.warn("don't have access authority wikiId " + wiki.getId() + " userId "
                    + user.getId());

            return false;
        }
        return true;
    }

    private String getCloseReturn(Invocation inv, Wiki wiki) {
        Ajax annotation = inv.getAnnotation(Ajax.class);
        if (null != annotation) {
            int code = 0;
            String msg = "";
            if (wiki.getState() == WikiStateType.CLOSE.getState()) {
                code = WikiExceptionType.WIKI_CLOSE.getCode();
                msg = WikiExceptionType.WIKI_CLOSE.getDisplayMsg();
            } else if (wiki.getState() == WikiStateType.FORBIDDEN.getState()) {
                code = WikiExceptionType.WIKI_FORBIDDEN.getCode();
                msg = WikiExceptionType.WIKI_FORBIDDEN.getDisplayMsg();
            }
            return "@" + JSONResultUtil.buildJSONResult(code, msg, null);
        }

        return "e:404";
    }

    private String getNotFoundReturn(Invocation inv) {
        Ajax annotation = inv.getAnnotation(Ajax.class);
        if (null != annotation) {
            return "@"
                    + JSONResultUtil.buildJSONResult(WikiExceptionType.WIKI_NOT_EXISTS.getCode(),
                            WikiExceptionType.WIKI_NOT_EXISTS.getDisplayMsg(), null);
        }

        return "e:404";
    }

}

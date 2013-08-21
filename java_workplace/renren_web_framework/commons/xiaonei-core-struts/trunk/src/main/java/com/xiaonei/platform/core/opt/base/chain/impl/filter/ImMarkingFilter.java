package com.xiaonei.platform.core.opt.base.chain.impl.filter;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.MD5;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.AbstractBizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 */
public class ImMarkingFilter extends AbstractBizFilter {

    @Override
    public boolean executeMe(WebContext webContext) throws BizFilterException {
        HttpServletRequest request = webContext.getRequest();
        User host = BaseThreadUtil.currentHost(request);
        if (host == null) {
            return false;
        }

        // for 黄欢
        if (host != null) {
            String xeferer = request.getHeader("Xeferer");
            if (logger.isDebugEnabled()) {
                logger.debug("httpHeader.Xeferer=" + xeferer + "; hostId=" + host.getId());
            }
            if (xeferer != null && xeferer.length() == 16) {
                // "1e4548100fa84a18b070b4b628e323e1"此常量来自黄欢
                String result = MD5.digest("1e4548100fa84a18b070b4b628e323e1" + host.getId())
                        .substring(0, 16);
                if (logger.isDebugEnabled()) {
                    logger.debug("expected Xeferer result: " + result + "   "
                            + (xeferer.equals(result) ? "ok" : "fail"));
                }
                request.setAttribute("isFromIM", xeferer.equals(result) ? Boolean.TRUE
                        : Boolean.FALSE);
            } else {
                request.setAttribute("isFromIM", Boolean.FALSE);
                if (logger.isDebugEnabled()) {
                    logger.debug("xeferer is null or illegel: xeferer=" + xeferer);
                }
            }
        }

        return false;
    }
}

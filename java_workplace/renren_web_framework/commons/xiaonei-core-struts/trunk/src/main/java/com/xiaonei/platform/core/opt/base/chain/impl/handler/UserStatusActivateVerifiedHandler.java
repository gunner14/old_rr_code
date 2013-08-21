package com.xiaonei.platform.core.opt.base.chain.impl.handler;

import java.io.IOException;

import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 处理异常BizFilterException.CODE_USER_STATUS_ACTIVATE_VERIFIED。
 * 
 * 
 * @author Li Weibo
 * @since 2009-1-3 下午08:51:02
 */
public class UserStatusActivateVerifiedHandler extends ExceptionHandlerBase {

    @Override
    public int getHandledExceptionCode() {
        return BizFilterException.CODE_USER_STATUS_ACTIVATE_VERIFIED;
    }

    @Override
    public void handleIt(BizFilterException e, WebContext context) {
        HttpServletResponse response = context.getResponse();

        try {
            response.sendRedirect(OpiConstants.urlMain + "/Newbie.do");
        } catch (IOException e1) {
            e1.printStackTrace();
        }
    }
}

package com.xiaonei.platform.core.opt.base.chain.impl.filter;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;
import com.xiaonei.platform.core.opt.base.chain.AbstractBizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;

/**
 * 检查用户的状态是不是User.statusActivateVerified。
 * 
 * 这貌似是一个历史遗留问题，现在是否还需要对状态为User.statusActivateVerified的用户进行特殊处理还有待商榷，产品
 * 同学还没有给一个明确的答复。
 * 
 * @author Li Weibo
 * @since 2008-12-31
 */
public class ActivatedFilter extends AbstractBizFilter {

    @Override
    public boolean executeMe(WebContext webContext) throws BizFilterException {
        if (OpiConstants.isXiaonei()) { //校内才判断，开心不判断
            HttpServletRequest request = webContext.getRequest();
            User host = BaseThreadUtil.currentHost(request);
            UserConfirmLogic logic = UserConfirmLogic.getInstance();
            if (!logic.iNeedYou(host) && host.getStatus() == User.statusActivateVerified) {
                throw new BizFilterException(BizFilterException.CODE_USER_STATUS_ACTIVATE_VERIFIED);
            }
        }
        return false;
    }
}

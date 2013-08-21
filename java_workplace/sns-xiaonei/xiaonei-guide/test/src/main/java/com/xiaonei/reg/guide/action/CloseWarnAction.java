package com.xiaonei.reg.guide.action;

import com.xiaonei.antispam.AntiSpamAdapter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.guide.util.GuideLogger;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;

public class CloseWarnAction extends ProtectAction {

    public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws SQLException {
        User user = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+user.getId()+" NOHERE! ",GuideLogger.ERROR);
        AntiSpamAdapter.getInstance().getCheckLoginClient().closeSecurityNotify(user.getId());
        return null;
    }
}

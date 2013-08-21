package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.util.GuideLogger;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;

public class ShowImportMsnAction extends ProtectAction {

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response)
            throws SQLException {
        String now = "dx";
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        if (UserAdapter.get(host).getStage() == Stage.STAGE_MAJOR || UserAdapter.get(host).getStage() == Stage.STAGE_NONE) {//白领阶段
            now = "bl";
        }
        return mapping.findForward(now);
    }

}

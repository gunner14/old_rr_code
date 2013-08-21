package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserDesc;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.msn.CommonImportMsnLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.register.logic.TimeCost;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;

public class BLRegImportMsnAction extends ProtectAction {

    private static final String succ = "succ";

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response)
            throws SQLException {
        TimeCost cost = TimeCost.begin();
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        StepStatusLogic.updateStepStatus(host.getId(),
                StepStatusLogic.STEP_UPLOAD_HEAD);
        cost.endStep();
        String toUrl = StepStatusLogic.goWhere(host);
        cost.endStep();
        if (!Utils.getInstance().validateForString(toUrl)
                && !toUrl
                .equalsIgnoreCase(Globals.urlGuide+"/regimportmsnbl.do")) {
            return new ActionForward(toUrl, true);
        }
        try {
            UserDesc ud = WUserDescAdapter.getInstance().get(host.getId());
            cost.endStep();
            if (null != ud) {
                request.setAttribute("msn", ud.getMsn());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            request.setAttribute("isMsnUser", CommonImportMsnLogic
                    .notCompanyUser(host));
            cost.endStep();
        } catch (Exception e) {
            e.printStackTrace();
        }

        cost.endFinally();

        return mapping.findForward(succ);
    }
}

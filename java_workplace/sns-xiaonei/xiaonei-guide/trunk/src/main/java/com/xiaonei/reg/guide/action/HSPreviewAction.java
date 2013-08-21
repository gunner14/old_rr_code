package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.preview.DXPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.guide.view.NewGuideView;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.search2.model.client.SearchView;
import mop.hi.oce.domain.network.HighSchoolInfo;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;
import java.util.List;

public class HSPreviewAction extends ProtectAction {

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws SQLException {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        String succ = "succ";
        String toUrl = StepStatusLogic.goWhere(host);
        if (!Utils.getInstance().validateForString(toUrl) && !toUrl.equalsIgnoreCase(Globals.urlGuide+"/previewhs.do")) {
            return new ActionForward(toUrl, true);
        }
        List<SearchView> hsList = null;
        NewGuideView n = DXPreviewLogic.getInstance().getHSGuideView(host, 40);
        if (n != null) {
            hsList = n.getResult();
        }
        NewGuideView ngv = DXPreviewLogic.getInstance().getHSGuideView(host, 40);
        request.setAttribute("highschoolList", ngv);
        List<HighSchoolInfo> hsTempList = DXPreviewLogic.getInstance().getHSInfoList(host);
        if (hsTempList != null && hsTempList.size() > 0) {
            request.setAttribute("noHighSchool", "1");
        }
        if (hsList == null || hsList.size() == 0) {
            try {
                // 用户行为跟踪
                UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(), String.valueOf(RegOprDefine.PREVIEW_AUTO_SKIP), UserAdapter.get(host).getStage(), null, null,
                        null, null, 0);
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
            StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_PREVIEW | StepStatusLogic.STEP_COMPLETE_GUIDE);
            String to = StepStatusLogic.goWhere(host);
            request.setAttribute("to", to);
            if (!Utils.getInstance().validateForString(to) && !to.equalsIgnoreCase(Globals.urlGuide+"/previewhs.do")) {
                return new ActionForward(to, true);
            } else {
                return new ActionForward(Globals.urlGuide + "/guide.do", true);
            }
        }
        try {
            // 用户行为跟踪
            UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(), String.valueOf(RegOprDefine.PREVIEW), UserAdapter.get(host).getStage(), null, null, null,
                    null, 0);
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        request.setAttribute("mixList", hsList);
        return mapping.findForward(succ);
    }

}

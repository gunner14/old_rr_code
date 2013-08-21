package com.xiaonei.reg.guide.action;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.domain.network.HighSchoolInfo;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.action.BasePageAction;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.preview.CommonPreviewLogic;
import com.xiaonei.reg.guide.logic.preview.DXPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.view.NewGuideView;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.search2.model.client.SearchView;

public class ShortFormHSPreviewAction extends BasePageAction {

    private static final String SUCC_PAGE = "succ";

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        String toUrl = StepStatusLogic.goWhere(host);
        if (!StringUtils
                .equals(toUrl, Globals.urlGuide+"/preview-sf.do")) {
            return new ActionForward(toUrl, true);
        }
        List<SearchView> hsList = null;
        NewGuideView n = CommonPreviewLogic.getInstance().getHSGuideView(host, 40);
        if (n != null) {
            hsList = n.getResult();
        }
        NewGuideView ngv = DXPreviewLogic.getInstance()
                .getHSGuideView(host, 40);
        request.setAttribute("highschoolList", ngv);
        List<HighSchoolInfo> hsTempList = DXPreviewLogic.getInstance()
                .getHSInfoList(host);
        if (hsTempList != null && hsTempList.size() > 0) {
            request.setAttribute("noHighSchool", "1");
        }
        if (hsList == null || hsList.size() == 0) {
            try {
                // 用户行为跟踪
                UserTraceLogic.getInstance().insertRegHis(request, host,
                        host.getAccount(),
                        String.valueOf(RegOprDefine.PREVIEW_AUTO_SKIP),
                        UserAdapter.get(host).getStage(), null, null, null, null, 0);
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
            StepStatusLogic.updateStepStatus(host.getId(),
                    StepStatusLogic.STEP_PREVIEW
                            | StepStatusLogic.STEP_COMPLETE_GUIDE);
            String to = StepStatusLogic.goWhere(host);
            request.setAttribute("to", to);
            if (!StringUtils.equals(to,
            		Globals.urlGuide+"/preview-sf.do")) {
                return new ActionForward(to, true);
            } else {
            	RegLogger.getLoggerWithType(this.getClass()).info(host.getId() + " preview no friend ");
                return new ActionForward(Globals.urlGuide + "/guide.do", true);
            }
        }
        try {
            // 用户行为跟踪
            UserTraceLogic.getInstance().insertRegHis(request, host,
                    host.getAccount(), String.valueOf(RegOprDefine.PREVIEW),
                    UserAdapter.get(host).getStage(), null, null, null, null, 0);
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        request.setAttribute("mixList", hsList);
        return mapping.findForward(SUCC_PAGE);
    }

}

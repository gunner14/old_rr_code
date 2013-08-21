package com.xiaonei.reg.guide.action;

import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.preview.CommonPreviewLogic;
import com.xiaonei.reg.guide.logic.preview.DXPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.guide.view.DXPreviewView;
import com.xiaonei.reg.guide.view.NewGuideView;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.search2.model.client.SearchView;
import com.xiaonei.xce.log.TimeCost;
import mop.hi.oce.domain.network.HighSchoolInfo;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;
import java.util.Date;
import java.util.List;

public class DXPreviewAction extends ProtectAction {

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response)
            throws SQLException {
        TimeCost cost = TimeCost.begin("DXPreviewAction");

        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        String succ = "succ";
        try {
            try {
                // 用户行为跟踪
                UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(), String.valueOf(RegOprDefine.PREVIEW), UserAdapter.get(host).getStage(), null, null, null, null, 0);
            } catch (Exception e) {
            	LogTimeUtils.logger.error(e);
            }
            cost.endStep("1");
            int skip = 0;
            try {
                skip = Integer.parseInt(request.getParameter("skip"));
            } catch (Exception e) {
            }
            if (skip == 1) {
                StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO);
            }
            cost.endStep("2");
            // 以前是20，现在是30
            List<SearchView> mixList = null;
            // 混排大学高中
            DXPreviewView d = DXPreviewLogic.getInstance().searchUnivResult(host);
            request.setAttribute("dxPojo", d);
            cost.endStep("3");
            //为了性能优化要改成批量处理。
            NewGuideView n = DXPreviewLogic.getInstance().getGuideView(d, host, 40);
            cost.endStep("4");
            List<SearchView> univList = null;
            List<SearchView> hsList = null;
            if (n != null) {
                univList = n.getResult();
            }
            cost.endStep("5");
            //为了性能优化要改成批量处理。
            NewGuideView ng = DXPreviewLogic.getInstance().getHSGuideView(host, 40);
            if (ng != null) {
                hsList = ng.getResult();
            }
            cost.endStep("6");
            //为了性能优化要改成批量处理。
            NewGuideView ngv = DXPreviewLogic.getInstance().getHSGuideView(host, 40);
            request.setAttribute("highschoolList", ngv);
            cost.endStep("7");
            List<HighSchoolInfo> hsTempList = DXPreviewLogic.getInstance().getHSInfoList(host);
            if (hsTempList != null && hsTempList.size() > 0) {
                request.setAttribute("noHighSchool", "1");
            }
            cost.endStep("8");

            mixList = CommonPreviewLogic.getInstance().mixList(univList, hsList);
            cost.endStep("9");
            if (mixList == null || mixList.size() == 0) {
                StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_PREVIEW);
                cost.endStep("10");
                String toUrl = StepStatusLogic.goWhere(host);
                cost.endStep("11");
                if (!Utils.getInstance().validateForString(toUrl) && !toUrl.equalsIgnoreCase(Globals.urlGuide+"/previewdx.do")) {
                    return new ActionForward(toUrl, true);
                }
                cost.endStep("12");
                return new ActionForward(Globals.urlGuide + "/guide.do", true);
            }
            request.setAttribute("mixList", mixList);
        } catch (Exception e) {
            cost.endException(e);
            LogTimeUtils.logger.error(e);
        } finally {
            cost.endFinally();
        }
        return mapping.findForward(succ);
    }

    public long timingEnd(long startTime, String funName) {
//		long offTime = new Date().getTime() - startTime;
        return new Date().getTime();
    }
}
package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.fillinfo.CommonFillinfoLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.UniversityInfo;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.sql.SQLException;

public class BLFillinfoAction extends ProtectAction {

    private final static int NEWUSERID = 0;

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response)
            throws SQLException {
        String succ = "succ";
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        UserTraceLogic.getInstance().insertRegHis(request, host,
                host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO),
                UserAdapter.get(host).getStage(), null, null, null, null, 0);
        // 如果发现老用户(此步骤未增加到引导前注册的用户)进入则
        // 记录状态并跳转至home页
        // (状态判断一定不能出错,否则会出现循环重定向)
        if (host.getId() < NEWUSERID) {
            StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO);
            try {
                response.sendRedirect(Globals.urlMain + "/home.do");
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }
        //读取大学院系控件时要用到的
        request.setAttribute("university", CommonFillinfoLogic.getInstance().getUniv(host));
        if (!"post".equalsIgnoreCase(request.getMethod())) {
            return mapping.findForward(succ);
        }
        //设置大学资料

        UniversityInfo newUnviInfo = checkUniversityForm(mapping, form, request, response, host.getId());
        if (newUnviInfo != null) {
            // 增加一个大学资料
            // 前提是用户的确是没有大学资料
            // 由于是在引导页显示所以基本可以保证
            UserTraceLogic.getInstance().insertRegHis(request, host,
                    host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO_SUBMIT),
                    UserAdapter.get(host).getStage(), null, null, null, null, 0);
            AdapterFactory.getNetworkAdapter().addUniversityInfo(newUnviInfo);
        }
        StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO);
        try {
            response.sendRedirect(Globals.urlGuide + "/guide.do");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 组装保存时的大学信息对象
     *
     * @param mapping
     * @param form
     * @param request
     * @param response
     * @param uiList
     * @return
     */
    private final UniversityInfo checkUniversityForm(ActionMapping mapping, ActionForm form,
                                                     HttpServletRequest request, HttpServletResponse response, int userId) {
        int univId = 0;
        try {
            univId = Integer.parseInt(request.getParameter("universityid"));    //大学id
        } catch (Exception e) {
        }
        if (univId == 0) {
            return null;
        }
        UniversityInfo ui = new UniversityInfo();
        ui.setUserId(userId);
        ui.setUniversityId(univId);
        String univName = Escape.stringToHTMLString(Escape.trim(request.getParameter("universityname")));//大学名字
        ui.setUniversityName(univName);

        int enrollYear = 0;
        try {
            enrollYear = Integer.parseInt(request.getParameter("universityyear"));//入学年univYear
        } catch (Exception e) {
        }
        ui.setEnrollYear(enrollYear);
        //
//		String univDept = Escape.stringToHTMLString(Escape.trim(request.getParameter("dept_name_"+index)));//department
        String univDept = Escape.stringToHTMLString(Escape.trim(request.getParameter("department")));//department
        ui.setDepartment(univDept);

		return ui;
	}
}

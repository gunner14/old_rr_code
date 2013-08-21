/**
 *
 */
package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.form.Fillinfo4399UnForm;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.mobile.MobileFillinfoLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.additional.RegNetworkLogic;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;

/**
 * AD4399FillinfoUnAction
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午07:28:52
 */
public class AD4399FillinfoUnAction extends ProtectAction {

    /**
     * 检查正确
     */
    public static final String CHECK_OK = null;

    private static final String CHECK_TYPEOFCOURSE = "请选择大学类型";

    private static final String CHECK_UNIV = "请填写大学";

    private static final String CHECK_UNIV_NOT_EXIST = "没有这个大学";

    private static final String ACT_INIT = "init";
    private static final String ACT_SUBMIT = "submit";
    private static final String PAR_ACT = "action";
    private static final String FORWORD_INIT = "init";
    private static final String FORWARD_SUCC = "succ";

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
                                   HttpServletResponse response) throws SQLException {
        Fillinfo4399UnForm fillForm = (Fillinfo4399UnForm) form;
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        if (false == isPost(request)) {
            UserTraceLogic.getInstance().insertRegHis(request, host,
                    host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO),
                    UserAdapter.get(host).getStage(), null, null, null, null, 0);
            return this.init(mapping, fillForm, request);
        }
        String act = request.getParameter(PAR_ACT);
        if (StringUtils.equals(act, ACT_INIT)) {
            UserTraceLogic.getInstance().insertRegHis(request, host,
                    host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO),
                    UserAdapter.get(host).getStage(), null, null, null, null, 0);
            return this.init(mapping, fillForm, request);

        } else if (StringUtils.equals(act, ACT_SUBMIT) || StringUtils.isEmpty(act)) {
            UserTraceLogic.getInstance().insertRegHis(request, host,
                    host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO_SUBMIT),
                    UserAdapter.get(host).getStage(), null, null, null, null, 0);
            StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_4399_FILLINFO
                    | StepStatusLogic.STEP_COMPLETE_GUIDE);
            return this.submit(mapping, fillForm, request);

        } else {
            return this.err(mapping, fillForm, request, "系统错误，请稍后重试");

        }
    }

    private ActionForward err(ActionMapping mapping, Fillinfo4399UnForm fillForm, HttpServletRequest request,
                              String errMsg) {
        addErr(request, errMsg);
        return this.init(mapping, fillForm, request);
    }

    private ActionForward submit(ActionMapping mapping, Fillinfo4399UnForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        RegNetworkLogic ml = RegNetworkLogic.getInstance();
        String rt = checkDXForm(fillForm);
        if (rt != MobileFillinfoLogic.CHECK_OK) {
            return this.err(mapping, fillForm, request, rt);
        }
        // 大学
        ml.saveUniv(host, 0, fillForm.getUnivId(), fillForm.getUnivName(), fillForm.getTypeOfCourse(), "", NetworkStatus.CURRENT_NETWORK);

        return succ(mapping, fillForm, request);
    }

    private String checkDXForm(Fillinfo4399UnForm fillForm) {
        if (StringUtils.isEmpty(fillForm.getTypeOfCourse())) {
            return CHECK_TYPEOFCOURSE;
        }
        if (StringUtils.isEmpty(fillForm.getUnivName()) || fillForm.getUnivId() == 0) {
            return CHECK_UNIV;
        }
        University university = NetworkManager.getInstance().getUnivById(fillForm.getUnivId());
        if (university == null) {
            return CHECK_UNIV_NOT_EXIST;
        }
        return CHECK_OK;
    }

    private ActionForward succ(ActionMapping mapping, Fillinfo4399UnForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        // 标志走完此步骤
        StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO_MOBILE);
        return mapping.findForward(FORWARD_SUCC);
    }

    private ActionForward init(ActionMapping mapping, Fillinfo4399UnForm fillForm, HttpServletRequest request) {
        return mapping.findForward(FORWORD_INIT);
    }

}

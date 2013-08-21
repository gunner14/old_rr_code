/**
 *
 */
package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.form.FillInfo4399BLForm;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
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
 * AD4399FillinfoBlAction
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午07:27:49
 */
public class AD4399FillinfoBlAction extends ProtectAction {

    /**
     * 检查正确
     */
    public static final String CHECK_OK = null;
    private static final String CHECK_LOCATION = "请填写居住地";

    private static final String PAR_ACT = "act";
    private static final String ACT_INIT = "init";
    private static final String ACT_SUBMIT = "submit";
    private static final String FORWARD_INIT = "init";
    private static final String FORWARD_SUCC = "succ";

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
                                   HttpServletResponse response) throws SQLException {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        FillInfo4399BLForm fillForm = (FillInfo4399BLForm) form;
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
            return err(mapping, fillForm, request, "系统错误，请稍后重试");
        }
    }

    private ActionForward err(ActionMapping mapping, FillInfo4399BLForm fillForm, HttpServletRequest request,
                              String errMsg) {
        addErr(request, errMsg);
        return this.init(mapping, fillForm, request);
    }

    private ActionForward submit(ActionMapping mapping, FillInfo4399BLForm fillForm, HttpServletRequest request) {
        String rt = checkBLForm(fillForm);
        if (rt != CHECK_OK) {
            return this.err(mapping, fillForm, request, rt);
        }
        User host = BaseThreadUtil.currentHost(request);
        RegNetworkLogic.getInstance().saveLocation(host, fillForm.getLocationProvince(),
                fillForm.getLocationCity(), fillForm.getLocationCode());
        return succ(mapping, fillForm, request);
    }

    private ActionForward succ(ActionMapping mapping, FillInfo4399BLForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        // 标志走完此步骤
        StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_4399_FILLINFO);
        return mapping.findForward(FORWARD_SUCC);
    }

    private String checkBLForm(FillInfo4399BLForm fillForm) {
        if (StringUtils.isEmpty(fillForm.getLocationCity()) || StringUtils.isEmpty(fillForm.getLocationProvince())) {
            return CHECK_LOCATION;
        }
        return CHECK_OK;
    }

    private ActionForward init(ActionMapping mapping, FillInfo4399BLForm fillForm, HttpServletRequest request) {
        return mapping.findForward(FORWARD_INIT);
    }

}

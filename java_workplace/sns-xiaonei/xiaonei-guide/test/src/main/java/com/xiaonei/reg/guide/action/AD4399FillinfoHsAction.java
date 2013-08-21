/**
 *
 */
package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.*;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.form.Fillinfo4399HsForm;
import com.xiaonei.reg.guide.form.MobileGZFillInfoForm;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.mobile.MobileFillinfoLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.dao.CollegeDAO;
import com.xiaonei.reg.register.dao.JuniorSchoolDAO;
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
 * AD4399FillinfoHsAction
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午07:29:30
 */
public class AD4399FillinfoHsAction extends ProtectAction {

    /**
     * 检查正确
     */
    public static final String CHECK_OK = null;

    private static final String CHECK_JUNIOR = "请填写初中";

    private static final String CHECK_HIGH = "请填写高中";

    private static final String CHECK_TECH = "请填写学校";

    private static final String CHECK_ENROLLYEAR = "请填写学校入学年";

    private static final String CHECK_HIGHSCHOOL_NOT_EXIST = "没有这个高中";

    private static final String CHEKC_JUNIORSCHOOL_NOT_EXIST = "没有这个初中";

    private static final String CHECK_TECH_NOT_EXIST = "没有这个学校";

    private static final String PAR_ACT = "action";
    private static final String ACT_INIT = "init";
    private static final String ACT_SUBMIT = "submit";
    private static final String FORWARD_INIT = "init";
    private static final String FORWARD_SUCC = "succ";

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
                                   HttpServletResponse response) throws SQLException {
        Fillinfo4399HsForm fillForm = (Fillinfo4399HsForm) form;
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        if (false == isPost(request)) {
            UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO),
            		UserAdapter.get(host).getStage(), null, null, null, null, 0);
            return this.init(mapping, fillForm, request);
        }
        String act = request.getParameter(PAR_ACT);
        if (StringUtils.equals(act, ACT_INIT)) {
            UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO),
            		UserAdapter.get(host).getStage(), null, null, null, null, 0);
            return this.init(mapping, fillForm, request);
        } else if (StringUtils.equals(act, ACT_SUBMIT) || StringUtils.isEmpty(act)) {
            UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(),
                    String.valueOf(RegOprDefine.FILL_INFO_SUBMIT), UserAdapter.get(host).getStage(), null, null, null, null, 0);
            StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_4399_FILLINFO
                    | StepStatusLogic.STEP_COMPLETE_GUIDE);
            return this.submit(mapping, fillForm, request);
        } else {
            return this.err(mapping, fillForm, request, "系统错误，请稍后重试");
        }
    }

    private ActionForward submit(ActionMapping mapping, Fillinfo4399HsForm fillForm, HttpServletRequest request) {
        String rt = checkForm(fillForm);
        if (rt != MobileFillinfoLogic.CHECK_OK) {
            return this.err(mapping, fillForm, request, rt);
        }
        switch (fillForm.getSchoolType()) {
            case MobileGZFillInfoForm.HIGH_SCHOOL_TYPE:
                return this.submitHS(mapping, fillForm, request);
            case MobileGZFillInfoForm.JUNIOR_SCHOOL_TYPE:
                return this.submitJS(mapping, fillForm, request);
            case MobileGZFillInfoForm.TECH_SCHOOL_TYPE:
                return this.submitTS(mapping, fillForm, request);
            default:
                return this.err(mapping, fillForm, request, "系统错误，请稍后重试");
        }
    }

    /**
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午10:08:35
     */
    private ActionForward submitTS(ActionMapping mapping, Fillinfo4399HsForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        RegNetworkLogic ml = RegNetworkLogic.getInstance();
        // 保存学校及入学
        ml.saveTechSchool(host, fillForm.getTechSchoolCode(), fillForm.getEnrollYear(), fillForm.getTechSchoolName(), true,
                NetworkStatus.CURRENT_NETWORK);

        // 标志走完此步骤
        return succ(mapping, fillForm, request);
    }

    /**
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午10:08:42
     */
    private ActionForward succ(ActionMapping mapping, Fillinfo4399HsForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        // 标志走完此步骤
        StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_4399_FILLINFO);
        return mapping.findForward(FORWARD_SUCC);
    }

    /**
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午10:08:44
     */
    private ActionForward submitJS(ActionMapping mapping, Fillinfo4399HsForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        RegNetworkLogic ml = RegNetworkLogic.getInstance();
        // 保存初中及入学
        ml.saveJuniorSchool(host, fillForm.getJuniorSchoolCode(), fillForm.getEnrollYear(), fillForm.getJuniorSchoolName(), true,
                NetworkStatus.CURRENT_NETWORK);
        // 标志走完此步骤
        return succ(mapping, fillForm, request);
    }

    /**
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午10:08:46
     */
    private ActionForward submitHS(ActionMapping mapping, Fillinfo4399HsForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        RegNetworkLogic ml = RegNetworkLogic.getInstance();
        // 保存高中及入学
        ml.saveHighSchool(host, fillForm.getHighSchoolCode(), fillForm.getEnrollYear(), fillForm.getHighSchoolName(), true,
                NetworkStatus.CURRENT_NETWORK);

        // 标志走完此步骤
        return succ(mapping, fillForm, request);
    }

    /**
     * @param fillForm
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午10:08:49
     */
    private String checkForm(Fillinfo4399HsForm fillForm) {

        if (Fillinfo4399HsForm.HIGH_SCHOOL_TYPE == fillForm.getSchoolType()) {
            if (StringUtils.isEmpty(fillForm.getHighSchoolName()) || 0 == fillForm.getHighSchoolCode()) {
                return CHECK_HIGH;
            }
            HighSchool highSchool = NetworkManager.getInstance().getHighSchoolById(fillForm.getHighSchoolCode());
            if (highSchool == null) {
                return CHECK_HIGHSCHOOL_NOT_EXIST;
            }
        } else if (Fillinfo4399HsForm.TECH_SCHOOL_TYPE == fillForm.getSchoolType()) {
            if (StringUtils.isEmpty(fillForm.getTechSchoolName()) || 0 == fillForm.getTechSchoolCode()) {
                return CHECK_TECH;
            }
            try {
                College college = CollegeDAO.getInstance().getByName(fillForm.getTechSchoolName());
                if (college == null) {
                    return CHECK_TECH_NOT_EXIST;
                }
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        } else if (Fillinfo4399HsForm.JUNIOR_SCHOOL_TYPE == fillForm.getSchoolType()) {
            if (StringUtils.isEmpty(fillForm.getJuniorSchoolName()) || 0 == fillForm.getJuniorSchoolCode()) {
                return CHECK_JUNIOR;
            }
            try {
                JuniorSchool juniorSchool = JuniorSchoolDAO.getInstance().getByName(fillForm.getJuniorSchoolName());
                if (juniorSchool == null) {
                    return CHEKC_JUNIORSCHOOL_NOT_EXIST;
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        if (0 == fillForm.getEnrollYear()) {
            return CHECK_ENROLLYEAR;
        }
        return CHECK_OK;
    }

    /**
     * @param mapping
     * @param fillForm
     * @param request
     * @param err
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午10:08:57
     */
    private ActionForward err(ActionMapping mapping, Fillinfo4399HsForm fillForm, HttpServletRequest request, String err) {
        addErr(request, err);
        return this.init(mapping, fillForm, request);
    }

    /**
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午10:09:00
     */
    private ActionForward init(ActionMapping mapping, Fillinfo4399HsForm fillForm, HttpServletRequest request) {
		return mapping.findForward(FORWARD_INIT);
	}

}

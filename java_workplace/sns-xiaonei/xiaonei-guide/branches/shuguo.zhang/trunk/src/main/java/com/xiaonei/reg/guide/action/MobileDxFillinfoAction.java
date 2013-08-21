/**
 *
 */
package com.xiaonei.reg.guide.action;

import com.opensymphony.oscache.util.StringUtil;
import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.form.MobileDXFillInfoForm;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.mobile.MobileFillinfoLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;
import java.util.List;

/**
 * MobileDxFillinfoAction
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-1 - 下午04:19:54
 */
public class MobileDxFillinfoAction extends ProtectAction {

    private static final String ACT_INIT = "init";
    private static final String ACT_SUBMIT = "submit";
    private static final String PAR_ACT = "action";
    private static final String FORWORD_INIT = "init";
    private static final String FORWARD_SUCC = "succ";
    private static final String SETTED_FLG = "true";//信息已经设置了标志

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
                                   HttpServletResponse response) throws SQLException {
        MobileDXFillInfoForm fillForm = (MobileDXFillInfoForm) form;
        if (false == isPost(request)) {
            return this.init(mapping, fillForm, request);
        }
        String act = request.getParameter(PAR_ACT);
        if (StringUtils.equals(act, ACT_INIT)) {
            return this.init(mapping, fillForm, request);

        } else if (StringUtils.equals(act, ACT_SUBMIT) || StringUtils.isEmpty(act)) {
            return this.submit(mapping, fillForm, request);

        } else {
            return this.err(mapping, fillForm, request, "系统错误，请稍后重试");

        }
    }

    /**
     * 出错
     *
     * @param mapping
     * @param fillForm
     * @param request
     * @param errMsg
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:57:05
     */
    private ActionForward err(ActionMapping mapping, MobileDXFillInfoForm fillForm, HttpServletRequest request, String errMsg) {
        addErr(request, errMsg);
        return this.init(mapping, fillForm, request);
    }

    /**
     * 提交<br>
     * 性别、类型、大学、院系、入学年、高中、入学年
     *
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:56:12
     */
    private ActionForward submit(ActionMapping mapping, MobileDXFillInfoForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" WHY?"+" host:"+host.getId());
        MobileFillinfoLogic ml = MobileFillinfoLogic.getInstance();
        String rt = MobileFillinfoLogic.getInstance().checkDXForm(fillForm);
        if (rt != MobileFillinfoLogic.CHECK_OK) {
            return this.err(mapping, fillForm, request, rt);
        }
        // 性别
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setGender")))
            ml.saveGender(host, fillForm.getGender());
        // 大学

        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setUniv")))
            ml.saveUniv(host, fillForm.getUnivEnrollyear(), fillForm.getUnivId(), fillForm.getUnivName(), fillForm.getTypeOfCourse(),
                    fillForm.getDepartment(), NetworkStatus.CURRENT_NETWORK);
        // 高中
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setHS")))
            ml.saveHighSchool(host, fillForm.getUnivEnrollyear(), fillForm.getHighSchoolEnrollYear(), fillForm.getHighSchoolName(),
                    false, 0);

        return succ(mapping, fillForm, request);
    }

    private ActionForward succ(ActionMapping mapping, MobileDXFillInfoForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        // 标志走完此步骤
        StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO_MOBILE);
        return mapping.findForward(FORWARD_SUCC);
    }

    /**
     * 初始化
     *
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:00:23
     */
    private ActionForward init(ActionMapping mapping, MobileDXFillInfoForm fillForm, HttpServletRequest request) {
        //add check 20090311
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" WHY?"+" host:"+host.getId());
        if (checkCompletion(host, request, fillForm)) {
            return succ(mapping, fillForm, request);
        }
        return mapping.findForward(FORWORD_INIT);
    }

    /**
     * 检查输入的完整性
     *
     * @param user
     * @param request
     * @param fillForm
     * @return boolean
     */
    private boolean checkCompletion(User user, HttpServletRequest request, MobileDXFillInfoForm fillForm) {
        boolean flg = true;
        //性别
        request.setAttribute("setGender", true);
        if (StringUtil.isEmpty(UserAdapter.get(user).getGender())) {
            flg = false;
            request.setAttribute("setGender", false);
        } else {
            fillForm.setGender(UserAdapter.get(user).getGender());
        }

        //大学
        request.setAttribute("setUniv", true);

        try {
            List<UniversityInfo> univs = AdapterFactory.getNetworkAdapter().getUniversityInfoList(user.getId());
            if (univs == null || univs.size() == 0) {
                request.setAttribute("setUniv", false);
                flg = false;
            } else {
                UniversityInfo univ = univs.get(0);
                fillForm.setUnivId(univ.getUniversityId());
                fillForm.setUnivName(univ.getUniversityName());
                fillForm.setUnivEnrollyear(univ.getEnrollYear());
                fillForm.setDepartment(univ.getDepartment());
                fillForm.setTypeOfCourse(univ.getTypeOfCourse());

                if (StringUtils.isEmpty(univ.getDepartment())) {
                    request.setAttribute("setUniv", false);
                    flg = false;
                }
            }
        } catch (Exception e) {
            request.setAttribute("setUniv", false);
            flg = false;
        }

        //高中
        request.setAttribute("setHS", true);
        try {
            List<HighSchoolInfo> hss = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(user.getId());
            if (hss == null || hss.size() == 0) {
                flg = false;
                request.setAttribute("setHS", false);
            } else {
                HighSchoolInfo hs = hss.get(0);
                fillForm.setHighSchoolCode(hs.getHighSchoolId());
                fillForm.setHighSchoolEnrollYear(hs.getEnrollYear());
                fillForm.setHighSchoolName(hs.getHighSchoolName());
            }
        } catch(Exception e){
			flg = false;
			request.setAttribute("setHS", false);
		}
		
		return flg;
	}
}

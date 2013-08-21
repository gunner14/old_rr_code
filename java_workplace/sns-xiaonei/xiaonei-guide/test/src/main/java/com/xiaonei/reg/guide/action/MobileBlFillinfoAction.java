/**
 *
 */
package com.xiaonei.reg.guide.action;

import java.sql.SQLException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.WorkplaceInfo;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.datum.dao.RegionInfoDAO;
import com.dodoyo.datum.model.RegionInfo;
import com.opensymphony.oscache.util.StringUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.form.MobileBLFillInfoForm;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.mobile.MobileFillinfoLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * MobileBlFillinfoAction
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-1 - 下午04:24:19
 */
public class MobileBlFillinfoAction extends ProtectAction {

    private static final String PAR_ACT = "act";
    private static final String ACT_INIT = "init";
    private static final String ACT_SUBMIT = "submit";
    private static final String FORWARD_INIT = "init";
    private static final String FORWARD_SUCC = "succ";
    private static final String SETTED_FLG = "true";//信息已经设置了标志

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws SQLException {
        MobileBLFillInfoForm fillForm = (MobileBLFillInfoForm) form;
        if (false == isPost(request)) {
            return this.init(mapping, fillForm, request);
        }
        String act = request.getParameter(PAR_ACT);
        if (StringUtils.equals(act, ACT_INIT)) {
            return this.init(mapping, fillForm, request);
        } else if (StringUtils.equals(act, ACT_SUBMIT) || StringUtils.isEmpty(act)) {
            return this.submit(mapping, fillForm, request);
        } else {
            return err(mapping, fillForm, request, "系统错误，请稍后重试");
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
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:24:15
     */
    private ActionForward err(ActionMapping mapping, MobileBLFillInfoForm fillForm, HttpServletRequest request, String errMsg) {
        addErr(request, errMsg);
        return this.init(mapping, fillForm, request);
    }

    /**
     * 提交<br>
     * 白领:性别、居住地、公司<br>
     * 以上都不是: 性别、居住地
     *
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:31:31
     */
    private ActionForward submit(ActionMapping mapping, MobileBLFillInfoForm fillForm, HttpServletRequest request) {
        MobileFillinfoLogic ml = MobileFillinfoLogic.getInstance();
        String rt = ml.checkBLForm(fillForm);
        if (rt != MobileFillinfoLogic.CHECK_OK) {
            return this.err(mapping, fillForm, request, rt);
        }
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" WHY?"+" host:"+host.getId());
        if (Stage.STAGE_MAJOR == fillForm.getStage()) {
            if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setGender")))
                ml.saveGender(host, fillForm.getGender());
            if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setRegion")))
                ml.saveLocation(host, fillForm.getLocationProvince(), fillForm.getLocationCity(), fillForm.getLocationCode());
            if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setCompany")))
                ml.saveCompany(host, fillForm.getCompany());
            return succ(mapping, fillForm, request);
        } else if (Stage.STAGE_NONE == fillForm.getStage() || 0 == fillForm.getStage()) {
            if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setGender")))
                ml.saveGender(host, fillForm.getGender());
            if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setRegion")))
                ml.saveLocation(host, fillForm.getLocationProvince(), fillForm.getLocationCity(), fillForm.getLocationCode());
            return succ(mapping, fillForm, request);
        } else {
            return this.err(mapping, fillForm, request, "系统错误，请稍后重试");
        }
    }

    private ActionForward succ(ActionMapping mapping, MobileBLFillInfoForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        // 标志走完此步骤
        StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO_MOBILE);
        return mapping.findForward(FORWARD_SUCC);
    }

    /**
     * 初始化页面
     *
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:31:09
     */
    private ActionForward init(ActionMapping mapping, MobileBLFillInfoForm fillForm, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" WHY?"+" host:"+host.getId());
        fillForm.setStage(UserAdapter.get(host).getStage());
        request.setAttribute("stage", UserAdapter.get(host).getStage());

        //add 20090311
        //检查完整性
        if (checkCompletion(host, request, fillForm)) {
            return succ(mapping, fillForm, request);
        }
        return mapping.findForward(FORWARD_INIT);
    }

    /**
     * 检查输入的完整性
     *
     * @param user
     * @param request
     * @return
     */
    private boolean checkCompletion(User user, HttpServletRequest request, MobileBLFillInfoForm fillForm) {
        boolean flg = true;
        //性别
        request.setAttribute("setGender", true);
        if (StringUtil.isEmpty(UserAdapter.get(user).getGender())) {
            flg = false;
            request.setAttribute("setGender", false);
        } else {
            fillForm.setGender(UserAdapter.get(user).getGender());
        }

        //居住地:locationProvince/locationCity/locationCode
        request.setAttribute("setRegion", true);
        try {
            RegionInfo region = RegionInfoDAO.getInstance().get(user.getId());
            if (region == null) {
                request.setAttribute("setRegion", false);
                flg = false;
            } else {
                fillForm.setLocationProvince(region.getProvince_name());
                fillForm.setLocationCity(region.getCity_name());
                fillForm.setLocationCode(region.getRegion_id());
            }
        } catch (Exception e) {
            request.setAttribute("setRegion", false);
            flg = false;
        }

        //公司:company
        request.setAttribute("setCompany", true);
        try {
            List<WorkplaceInfo> workplaces = AdapterFactory.getNetworkAdapter().getWorkplaceInfoList(user.getId());
            if (workplaces == null || workplaces.size() == 0) {
                flg = false;
                request.setAttribute("setCompany", false);
            } else {
                fillForm.setCompany(workplaces.get(0).getWorkplaceName());
            }
        } catch (Exception e){
			flg = false;
			request.setAttribute("setCompany", false);
		}
		
		return flg;
	}
}


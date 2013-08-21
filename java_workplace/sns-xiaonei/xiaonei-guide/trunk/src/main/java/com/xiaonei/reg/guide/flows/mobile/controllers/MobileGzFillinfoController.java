/**
 *
 */
package com.xiaonei.reg.guide.flows.mobile.controllers;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;

import com.opensymphony.oscache.util.StringUtil;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForward;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionMapping;
import com.xiaonei.reg.guide.form.MobileGZFillInfoForm;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.mobile.MobileFillinfoLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * MobileGzFillinfoController.java
 * 
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-8 新建文件 兼容老的fillinfomg.do
 */
@LoginRequired
@Path("fillinfomg.do")
public class MobileGzFillinfoController {

    private static final String PAR_ACT = "action";

    private static final String ACT_INIT = "init";

    private static final String ACT_SUBMIT = "submit";

    private static final String FORWARD_INIT = "init";

    private static final String FORWARD_SUCC = "succ";

    private static final String SETTED_FLG = "true";//信息已经设置了标志

    /*
     *  <action path="/fillinfomg" type="com.xiaonei.reg.guide.action.MobileGzFillinfoAction" name="mGzFiFom" scope="request" >
            <forward name="init" path="/pages/register/mobile_fillinfo_gz.jsp"></forward>
            <forward name="succ" path="/verifymail.do" redirect="true"></forward>
        </action>
    */
    @Get
    @Post
    public String index(Invocation inv, MobileGZFillInfoForm form) {
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        Map<String, ActionForward> map = new HashMap<String, ActionForward>();
        map.put("init", new ActionForward("/pages/register/mobile_fillinfo_gz.jsp"));
        map.put("succ", new ActionForward("verifymail.do", true));
        ActionMapping mapping = new ActionMapping(map);
        return executePt(mapping, form, request, response).toString();
    }

    private boolean isPost(HttpServletRequest request) {
        return MyStringUtils.equalsIgnoreCase(request.getMethod(), "POST");
    }

    private void addErr(HttpServletRequest request, String errMsg) {
        BizFilterUtil.addErr(request, errMsg);
    }

    public ActionForward executePt(ActionMapping mapping, MobileGZFillInfoForm form,
            HttpServletRequest request, HttpServletResponse response) {
        MobileGZFillInfoForm fillForm = (MobileGZFillInfoForm) form;
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
     * 错误跳转
     * 
     * @param mapping
     * @param form
     * @param request
     * @param err
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午05:42:14
     */
    private ActionForward err(ActionMapping mapping, MobileGZFillInfoForm fillForm,
            HttpServletRequest request, String err) {
        addErr(request, err);
        return this.init(mapping, fillForm, request);
    }

    /**
     * 提交fillinfo 信息 保存
     * 
     * @param mapping
     * @param form
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午05:33:57
     */
    private ActionForward submit(ActionMapping mapping, MobileGZFillInfoForm form, HttpServletRequest request) {
        MobileGZFillInfoForm fillForm = (MobileGZFillInfoForm) form;
        GuideLogger.printLog(" MOBILE_GZ");
        String rt = MobileFillinfoLogic.getInstance().checkGZForm(fillForm);
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
     * 处理技校用户fillinfo信息<br>
     * 保存性别<br>
     * 保存家乡<br>
     * 保存学校及入学<br>
     * 保存初中及入学<br>
     * 保存小学及入学<br>
     * 
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午08:10:08
     */
    private ActionForward submitTS(ActionMapping mapping, MobileGZFillInfoForm fillForm,
            HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" MOBILE_GZ" + " host:" + host.getId()+" fillForm:"+fillForm);
        MobileFillinfoLogic ml = MobileFillinfoLogic.getInstance();
        // 保存性别
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setGender"))) ml.saveGender(host,
                fillForm.getGender());
        // 保存家乡
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setHometown"))) ml.saveHometown(
                host, fillForm.getHometownProvince(), fillForm.getHometownCity(), fillForm
                        .getHometownCityCode());
        // 保存学校及入学
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setTS"))) ml.saveTechSchool(host,
                fillForm.getTechSchoolCode(), fillForm.getTechSchoolEnrollYear(), fillForm
                        .getTechSchoolName(), true, NetworkStatus.CURRENT_NETWORK);
        // 保存初中及入学
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setJS"))) ml.saveJuniorSchool(host,
                fillForm.getJuniorSchoolCode(), fillForm.getJuniorSchoolEnrollYear(), fillForm
                        .getJuniorSchoolName(), false, 0);
        // 保存小学及入学
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setES"))) ml.saveElementarySchool(
                host, fillForm.getElementarySchoolEnrollYear(), fillForm.getElementarySchoolName());

        // 标志走完此步骤
        return succ(mapping, fillForm, request);
    }

    /**
     * 处理初中用户fillinfo信息<br>
     * 保存性别<br>
     * 保存家乡<br>
     * 保存初中及入学<br>
     * 保存小学及入学<br>
     * 
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午08:09:41
     */
    private ActionForward submitJS(ActionMapping mapping, MobileGZFillInfoForm fillForm,
            HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" MOBILE_GZ" + " host:" + host.getId()+" fillForm:"+fillForm);
        MobileFillinfoLogic ml = MobileFillinfoLogic.getInstance();
        // 保存性别
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setGender"))) ml.saveGender(host,
                fillForm.getGender());
        // 保存家乡
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setHometown"))) ml.saveHometown(
                host, fillForm.getHometownProvince(), fillForm.getHometownCity(), fillForm
                        .getHometownCityCode());
        // 保存初中及入学
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setJS"))) ml.saveJuniorSchool(host,
                fillForm.getJuniorSchoolCode(), fillForm.getJuniorSchoolEnrollYear(), fillForm
                        .getJuniorSchoolName(), true, NetworkStatus.CURRENT_NETWORK);
        // 保存小学及入学
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setES"))) ml.saveElementarySchool(
                host, fillForm.getElementarySchoolEnrollYear(), fillForm.getElementarySchoolName());

        // 标志走完此步骤
        return succ(mapping, fillForm, request);
    }

    /**
     * 处理高中用户fillinfo信息<br>
     * 保存性别<br>
     * 保存家乡<br>
     * 保存高中及入学<br>
     * 保存初中及入学<br>
     * 保存小学及入学<br>
     * 
     * @param mapping
     * @param fillForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午08:09:02
     */
    private ActionForward submitHS(ActionMapping mapping, MobileGZFillInfoForm fillForm,
            HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" MOBILE_GZ" + " host:" + host.getId()+" fillForm:"+fillForm);
        MobileFillinfoLogic ml = MobileFillinfoLogic.getInstance();
        // 保存性别
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setGender"))) ml.saveGender(host,
                fillForm.getGender());
        // 保存家乡
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setHometown"))) ml.saveHometown(
                host, fillForm.getHometownProvince(), fillForm.getHometownCity(), fillForm
                        .getHometownCityCode());
        // 保存高中及入学
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setHS"))) ml.saveHighSchool(host,
                fillForm.getHighSchoolCode(), fillForm.getHighSchoolEnrollYear(), fillForm
                        .getHighSchoolName(), true, NetworkStatus.CURRENT_NETWORK);
        // 保存初中及入学
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setJS"))) ml.saveJuniorSchool(host,
                fillForm.getJuniorSchoolCode(), fillForm.getJuniorSchoolEnrollYear(), fillForm
                        .getJuniorSchoolName(), false, 0);
        // 保存小学及入学
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setES"))) ml.saveElementarySchool(
                host, fillForm.getElementarySchoolEnrollYear(), fillForm.getElementarySchoolName());

        // 标志走完此步骤
        return succ(mapping, fillForm, request);
    }

    private ActionForward succ(ActionMapping mapping, MobileGZFillInfoForm fillForm,
            HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" MOBILE_GZ" + " host:" + host.getId());
        // 标志走完此步骤
        StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FILLINFO_MOBILE);
        return mapping.findForward(FORWARD_SUCC);
    }

    /**
     * 页面初始化
     * 
     * @param mapping
     * @param form
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午05:32:31
     */
    private ActionForward init(ActionMapping mapping, MobileGZFillInfoForm fillForm,
            HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" MOBILE_GZ" + " host:" + host.getId());
        //		int schoolType = -1;
        //		try {
        //			schoolType = MiddleSchoolUserLogDAO.getInstance().getSchoolType(host.getId());
        //		} catch (SQLException e) {
        //			e.printStackTrace();
        //		}
        //fillForm.setSchoolType(schoolType);
        //		request.setAttribute("schoolType", schoolType);

        //add 20090311
        if (checkCompletion(host, request, fillForm)) {
            return succ(mapping, fillForm, request);
        }

        return mapping.findForward(FORWARD_INIT);
    }

    /**
     * 完整性检查
     * 
     * @param user
     * @param request
     * @param fillForm
     * @return
     */
    private boolean checkCompletion(User user, HttpServletRequest request,
            MobileGZFillInfoForm fillForm) {
        boolean flg = true;
        //性别
        request.setAttribute("setGender", true);
        if (StringUtil.isEmpty(UserAdapter.get(user).getGender())) {
            flg = false;
            request.setAttribute("setGender", false);
        } else {
            fillForm.setGender(UserAdapter.get(user).getGender());
        }

        //家乡
        request.setAttribute("setHometown", true);
        try {
            UserBasic ub = SnsAdapterFactory.getUserBasicAdapter().get(user.getId());
            if (StringUtils.isEmpty(ub.getCityCode())) {
                request.setAttribute("setHometown", false);
                flg = false;
            } else {
                fillForm.setHometownCity(ub.getHomeCity());
                fillForm.setHometownCityCode(ub.getCityCode());
                fillForm.setHometownProvince(ub.getHomeProvince());
            }
        } catch (Exception e) {
            request.setAttribute("setHometown", false);
            flg = false;
        }

        int schoolType = -1;
        boolean hsflg = true;
        //高中及入学
        request.setAttribute("setHS", true);
        try {
            List<HighSchoolInfo> hss = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(
                    user.getId());
            if (hss == null || hss.size() == 0) {
                hsflg = false;
                request.setAttribute("setHS", false);
            } else {
                HighSchoolInfo hs = hss.get(0);
                fillForm.setHighSchoolCode(hs.getHighSchoolId());
                fillForm.setHighSchoolEnrollYear(hs.getEnrollYear());
                fillForm.setHighSchoolName(hs.getHighSchoolName());

                schoolType = MobileGZFillInfoForm.HIGH_SCHOOL_TYPE;
            }
        } catch (Exception e) {
            request.setAttribute("setHS", false);
            hsflg = false;
        }

        if (schoolType == -1) {
            //技校
            request.setAttribute("setTS", true);
            try {
                List<CollegeInfo> cols = AdapterFactory.getNetworkAdapter().getCollegeInfoList(
                        user.getId());
                if (cols == null || cols.size() == 0) {
                    request.setAttribute("setTS", false);
                } else {
                    schoolType = MobileGZFillInfoForm.TECH_SCHOOL_TYPE;
                    CollegeInfo col = cols.get(0);
                    fillForm.setTechSchoolCode(col.getCollegeId());
                    fillForm.setTechSchoolEnrollYear(col.getEnrollYear());
                    fillForm.setTechSchoolName(col.getCollegeName());
                    hsflg = true;
                }
            } catch (Exception e) {
                request.setAttribute("setTS", false);
            }

        }

        flg = flg && hsflg;

        //初中及入学
        request.setAttribute("setJS", true);
        try {
            JuniorHighSchoolInfo jhs = AdapterFactory.getNetworkAdapter().getJuniorHighSchoolInfo(
                    user.getId());
            if (jhs == null) {
                request.setAttribute("setJS", false);
                flg = false;
            } else {
                fillForm.setJuniorSchoolCode(jhs.getJunHighentarySchoolId());
                fillForm.setJuniorSchoolEnrollYear(jhs.getJunHighentarySchoolYear());
                fillForm.setJuniorSchoolName(jhs.getJunHighentarySchoolName());

                schoolType = MobileGZFillInfoForm.JUNIOR_SCHOOL_TYPE;
            }
        } catch (Exception e) {
            request.setAttribute("setJS", false);
            flg = false;
        }

        //小学及入学
        request.setAttribute("setES", true);
        try {
            ElementarySchoolInfo esi = AdapterFactory.getNetworkAdapter().getElementarySchoolInfo(
                    user.getId());
            if (esi == null) {
                request.setAttribute("setES", false);
                flg = false;
            } else {
                fillForm.setElementarySchoolEnrollYear(esi.getElementarySchoolYear());
                fillForm.setElementarySchoolName(esi.getElementarySchoolName());
            }
        } catch (Exception e) {
            request.setAttribute("setES", false);
            flg = false;
        }

        if (schoolType == -1) {
            schoolType = MobileGZFillInfoForm.HIGH_SCHOOL_TYPE;
        }
        fillForm.setSchoolType(schoolType);
        request.setAttribute("schoolType", schoolType);
        return flg;
    }

}

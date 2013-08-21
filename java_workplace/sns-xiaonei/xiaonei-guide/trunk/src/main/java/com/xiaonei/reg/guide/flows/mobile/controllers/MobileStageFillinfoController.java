package com.xiaonei.reg.guide.flows.mobile.controllers;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.adapter.AdapterFactory;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForward;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionMapping;
import com.xiaonei.reg.guide.form.MobileStageFillinfoForm;
import com.xiaonei.reg.guide.logic.MobileStageFillinfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckName;
import com.xiaonei.reg.register.logic.pretreatment.validate.RegCheckLogic;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;


/**
 * MobileStageFillinfoController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-8-27 新建文件
 * 2010-9-6 封装逻辑 兼容fillinfost.do
 * 
 */
@SuppressWarnings("deprecation")
@LoginRequired
@Path({"fillinfost.do"})
public class MobileStageFillinfoController  {

    private static final String PAR_ACT = "act";
    private static final String ACT_INIT = "init";
    private static final String ACT_SUBMIT = "submit";
    private static final String FOR_INIT = "init";
    private static final String FOR_SUCC_MA = "succ-bl";
    private static final String FOR_SUCC_UN = "succ-dx";
    private static final String FOR_SUCC_HS = "succ-gz";
    private static final String SETTED_FLG = "true";//信息已经设置了标志

    /*
     * <action path="/fillinfost" type="com.xiaonei.reg.guide.action.MobileStageFillinfoAction" name="mStageFillForm" scope="request">
     * 
                <forward name="init" path="/pages/register/mobile_fillinfo_stage.jsp" />
                <forward name="succ-gz" path="/fillinfomg.do" redirect="true" />
                <forward name="succ-dx" path="/fillinfomd.do" redirect="true" />
                <forward name="succ-bl" path="/fillinfomb.do" redirect="true" />
        </action>
     */
    @Get
    @Post
    public String index(Invocation inv, MobileStageFillinfoForm form) {
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        Map<String, ActionForward> map = new HashMap<String, ActionForward>();
        map.put("init", new ActionForward("/pages/register/mobile_fillinfo_stage.jsp"));
        map.put("succ-gz", new ActionForward("/fillinfomg.do", true));
        map.put("succ-dx", new ActionForward("/fillinfomd.do", true));
        map.put("succ-bl", new ActionForward("/fillinfomb.do", true));
        ActionMapping mapping = new ActionMapping(map);
        return executePt(mapping, form, request, response).toString();
    }

    private boolean isPost(HttpServletRequest request) {
        return MyStringUtils.equalsIgnoreCase(request.getMethod(), "POST");
    }
    public ActionForward executePt(ActionMapping mapping, MobileStageFillinfoForm form, HttpServletRequest request,HttpServletResponse response){
    	GuideLogger.printLog(" MOBILE_STG ");
        MobileStageFillinfoForm mobileForm = (MobileStageFillinfoForm) form;
        String act = StringUtils.trimToEmpty(request.getParameter(PAR_ACT));
        if (!isPost(request)) {
            return init(mapping, mobileForm, request);
        }
        if (StringUtils.equals(act, ACT_INIT) || StringUtils.isEmpty(act)) {
            return init(mapping, mobileForm, request);
        } else if (StringUtils.equals(act, ACT_SUBMIT)) {
            return submit(mapping, mobileForm, request);
        } else {
            return init(mapping, mobileForm, request);
        }
    }

    /**
     * submit
     *
     * @param mapping
     * @param mobileForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午04:34:31
     */
    private ActionForward submit(ActionMapping mapping, MobileStageFillinfoForm mobileForm, HttpServletRequest request) {
        // submit
        User user = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" MOBILE_STG "+" host:"+user.getId());
        //check Code
        String msgCode = RegCheckLogic.getInstance().checkMobileStageFillinfo(user, mobileForm.getName(), request);
        

        if (!RegCheckLogic.OK.equals(msgCode)) {
            return err(mapping, request, RegCheckLogic.getInstance().getErrorValue(msgCode));
        }
        
        //inshion
        if(CheckName.containsIllegalChars(mobileForm.getName())){
        	return err(mapping, request, RegCheckLogic.ERR_NAME_ILLEGLE_CHAR);
        }


        // check
        switch (mobileForm.getStage()) {
            case Stage.STAGE_HEIGHSCHOOL:
            case Stage.STAGE_UNIV:
            case Stage.STAGE_MAJOR:
            case Stage.STAGE_NONE:
                break;
            default:
                return err(mapping, request, "请选择您目前身份");
        }

        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setStage")))
            MobileStageFillinfoLogic.saveStage(user, mobileForm.getStage());
        if (!SETTED_FLG.equalsIgnoreCase(request.getParameter("setName")))
            MobileStageFillinfoLogic.saveName(user, mobileForm.getName());
        StepStatusLogic.updateStepStatus(user.getId(), StepStatusLogic.STEP_FILLINFO_STAGE_MOBILE);
        // 用户行为跟踪
        UserTraceLogic.getInstance().insertRegHis(request, user, user.getAccount(),
                String.valueOf(RegOprDefine.FILL_INFO_STAGE_SUBMIT), UserAdapter.get(user).getStage(), null, null, null, null, 0);
        // return
        switch (mobileForm.getStage()) {
            case Stage.STAGE_HEIGHSCHOOL:
                return mapping.findForward(FOR_SUCC_HS);
            case Stage.STAGE_UNIV:
                return mapping.findForward(FOR_SUCC_UN);
            case Stage.STAGE_MAJOR:
            case Stage.STAGE_NONE:
                return mapping.findForward(FOR_SUCC_MA);
            default:
                return err(mapping, request, "请选择您目前身份");
        }
    }

    /**
     * @param mapping
     * @param request
     * @param errorValue
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午05:00:46
     */
    private ActionForward err(ActionMapping mapping, HttpServletRequest request, String errMsg) {
        BaseActionUtil.addErr(request, errMsg);
        return mapping.findForward(FOR_INIT);
    }

    /**
     * init
     *
     * @param mapping
     * @param mobileForm
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午04:32:55
     */
    private ActionForward init(ActionMapping mapping, MobileStageFillinfoForm mobileForm, HttpServletRequest request) {
        ActionForward rt = jumpThisStep(mapping, mobileForm, request);
        if (null != rt) {
            return rt;
        }
        User user = BaseThreadUtil.currentHost(request);

        // 用户行为跟踪
        UserTraceLogic.getInstance().insertRegHis(request, user, user.getAccount(),
                String.valueOf(RegOprDefine.FILL_INFO_STAGE), UserAdapter.get(user).getStage(), null, null, null, null, 0);


        // add 检查完成度 20090311
        if (checkCompletion(user, request, mobileForm)) {
            switch (UserAdapter.get(user).getStage()) {
                case Stage.STAGE_HEIGHSCHOOL:
                    return mapping.findForward(FOR_SUCC_HS);
                case Stage.STAGE_UNIV:
                    return mapping.findForward(FOR_SUCC_UN);
                case Stage.STAGE_MAJOR:
                case Stage.STAGE_NONE:
                    return mapping.findForward(FOR_SUCC_MA);
                default:
                    return mapping.findForward(FOR_INIT);
            }
        }
        return mapping.findForward(FOR_INIT);
    }

    /**
     * 如果用户完善过资料，跳过此步骤
     *
     * @param mapping
     * @param mobileForm
     * @param request
     * @return 没有完善过资料 返回 null
     * @author wang-tai(tai.wang@opi-corp.com) 2009-3-5 - 下午06:11:35
     */
    private ActionForward jumpThisStep(ActionMapping mapping, MobileStageFillinfoForm mobileForm, HttpServletRequest request) {
        User user = BaseThreadUtil.currentHost(request);
        // check
        switch (UserAdapter.get(user).getStage()) {
            case Stage.STAGE_HEIGHSCHOOL:
            case Stage.STAGE_UNIV:
            case Stage.STAGE_MAJOR:
            case Stage.STAGE_NONE:
                break;
            default:
                return null;
        }

        StepStatusLogic.updateStepStatus(user.getId(), StepStatusLogic.STEP_FILLINFO_STAGE_MOBILE);
        // return
        switch (UserAdapter.get(user).getStage()) {
            case Stage.STAGE_HEIGHSCHOOL:
                return mapping.findForward(FOR_SUCC_HS);
            case Stage.STAGE_UNIV:
                return mapping.findForward(FOR_SUCC_UN);
            case Stage.STAGE_MAJOR:
            case Stage.STAGE_NONE:
                return mapping.findForward(FOR_SUCC_MA);
            default:
                return null;
        }
    }

    /**
     * check用户信息是否完善
     *
     * @param user
     * @param request
     * @return boolean
     */
    private boolean checkCompletion(User user, HttpServletRequest request, MobileStageFillinfoForm mobileForm) {
        boolean flg = true;

        //名字
        request.setAttribute("setName", true);
        if (StringUtils.isEmpty(user.getName())) {
            flg = false;
            request.setAttribute("setName", false);
        } else {
            mobileForm.setName(user.getName());
        }

        //默认阶段： 20(属于大学),从wap过来是0
        //阶段
        request.setAttribute("setStage", true);
        try {
            if (UserAdapter.get(user).getStage() == 0 || (UserAdapter.get(user).getStage() == Stage.STAGE_UNIV && AdapterFactory.getNetworkAdapter().getHighSchoolInfoCount(user.getId()) > 0)) {
                request.setAttribute("setStage", false);
                flg = false;

                //设置名字也没添(默认有一个新用户+数字的名字)
                request.setAttribute("setName", false);
                mobileForm.setName("");
            } else {
                mobileForm.setStage(UserAdapter.get(user).getStage());
            }
        }catch(Exception e){
			request.setAttribute("setStage", false);
			flg = false;
		}
		
		return flg;
	}
}

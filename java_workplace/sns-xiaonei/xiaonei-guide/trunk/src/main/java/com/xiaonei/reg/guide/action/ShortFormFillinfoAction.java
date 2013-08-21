package com.xiaonei.reg.guide.action;

import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.action.BasePageAction;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.constants.IMiddleSchoolType;
import com.xiaonei.reg.guide.form.*;
import com.xiaonei.reg.guide.logic.ShortFillinfoLogic;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckHomecity;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckMiddleschool;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckUniversity;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class ShortFormFillinfoAction extends BasePageAction {

    private static final ShortFillinfoLogic logic = ShortFillinfoLogic
            .getInstance();

    private static final String FORWARD_SUCC = "succ";

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response) {
    	GuideLogger.printLog(" NOHERE! ",GuideLogger.ERROR);
        if (!"post".equalsIgnoreCase(request.getMethod())) {
            return initPage(request, mapping);
        } else {
            switch (((ShortFormFillInfoBaseForm) form).getStage()) {
                case Stage.STAGE_HEIGHSCHOOL:
                    String rt = checkHs((ShortFormFillInfoHsForm) form);
                    if (rt != null) {
                        return errPage(request, mapping, rt);
                    }
                    return sbHsPage((ShortFormFillInfoHsForm) form, response,
                            request, mapping);
                case Stage.STAGE_UNIV:
                    rt = checkUniv((ShortFormFillInfoUnivForm) form);
                    if (rt != null)
                        return errPage(request, mapping, rt);
                    return sbUnivPage((ShortFormFillInfoUnivForm) form, response,
                            request);
                case Stage.STAGE_MAJOR:
                case Stage.STAGE_NONE:
                    rt = checkMajor((ShortFormFillInfoMajorForm) form);
                    if (rt != null)
                        return errPage(request, mapping, rt);
                    return sbMajorPage((ShortFormFillInfoMajorForm) form, response,
                            request);
                default:
                    return errPage(request, mapping, "清选择身份");
            }
        }
    }

    /**
     * 检查大学
     *
     * @param form
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午04:26:03
     */
    private String checkUniv(ShortFormFillInfoUnivForm form) {
        String rt = new CheckUniversity().checkUniversity(form
                .getTypeOfCourse(), form.getUniversityid(), form
                .getUniversityname());
        if (!CheckMiddleschool.OK.equals(rt)) {
            return rt;
        }
        if (form.getUniversityyear() == 0 || form.getHighschoolyear() == 0) {
            return "请选择入学年";
        }
        if (form.getUniversityyear() < form.getHighschoolyear()) {
            return "大学入学年应小于高中入学年";
        }
        if ("".equals(form.getDepartment())) {
            return "请选择院系";
        }
        rt = new CheckMiddleschool().checkMiddleSchool(""
                + MobileGZFillInfoForm.HIGH_SCHOOL_TYPE, form
                .getHighschoolyear(), "", "" + form.getHighSchoolCode(), form
                .getHighSchoolName(), "0", "", "", "");
        if (!CheckMiddleschool.OK.equals(rt)) {
            return rt;
        }
        return null;
    }

    /**
     * 检查白领，和以上都不是
     *
     * @param form
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午04:26:20
     */
    private String checkMajor(ShortFormFillInfoMajorForm form) {
        String rt = new CheckHomecity().checkHomecity(""
                + form.getHomecityCode(), form.getHomecityName(), form
                .getHomeprovince());
        if (!CheckMiddleschool.OK.equals(rt)) {
            if (form.isSkip()) {
                form.setHomecityCode(0);
                form.setHomecityName("");
                form.setHomeprovince("");
                form.setCompany("");
                return null;
            }
            return rt;
        }
        return null;
    }

    /**
     * 检查中学阶段
     *
     * @param form
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午04:26:41
     */
    private String checkHs(ShortFormFillInfoHsForm form) {
        String rt = new CheckMiddleschool().checkMiddleSchool(""
                + form.getSchoolType(), 0, "", "" + form.getHighSchoolCode(),
                form.getHighSchoolName(), "" + form.getJuniorSchoolCode(), form
                        .getJuniorhighschoolname(), ""
                        + form.getTechSchoolCode(), form.getTechSchoolName());
        if (!CheckMiddleschool.OK.equals(rt)) {
            return rt;
        }
        switch (form.getSchoolType()) {
            case IMiddleSchoolType.HIGH_SCHOOL_TYPE:
                if (form.getHighschoolyear() == 0
                        || form.getJuniorhighschoolyear() == 0) {
                    return "请选择入学年";
                } else if (form.getHighschoolyear() < form
                        .getJuniorhighschoolyear()) {
                    return "高中入学年应大于初中入学年";
                }
                //added by Inshion -- to ask wangtai to confirm -- confirmed
                if ("".equals(form.getJuniorhighschoolname())) {
                    return "请输入初中";
                }
                break;
            case IMiddleSchoolType.JUNIOR_SCHOOL_TYPE:
                if (form.getJuniorhighschoolyear() == 0
                        || form.getElementaryschoolyear() == 0) {
                    return "请选择入学年";
                } else if (form.getJuniorhighschoolyear() < form
                        .getElementaryschoolyear()) {
                    return "初中入学年应大于小学入学年";
                }
                if ("".equals(form.getElementaryschoolname())) {
                    return "请输入小学";
                }
                break;
            case IMiddleSchoolType.TECH_SCHOOL_TYPE:
                if (form.getTechschoolyear() == 0
                        || form.getJuniorhighschoolyear() == 0) {
                    return "请选择入学年";
                } else if (form.getTechschoolyear() < form
                        .getJuniorhighschoolyear()) {
                    return "中专入学年应大于初中入学年";
                }
              //added by Inshion -- to ask wangtai to confirm -- confirmed
                if ("".equals(form.getJuniorhighschoolname())) {
                    return "请输入初中";
                }
                break;
        }
        return null;
    }

    /**
     * 提交中学信息
     *
     * @param form
     * @param response
     * @param request
     * @param mapping
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午04:27:01
     */
    private ActionForward sbHsPage(ShortFormFillInfoHsForm form,
                                   HttpServletResponse response, HttpServletRequest request,
                                   ActionMapping mapping) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        switch (form.getSchoolType()) {
            case IMiddleSchoolType.HIGH_SCHOOL_TYPE:
                logic.saveHs(host, form);
                break;
            case IMiddleSchoolType.JUNIOR_SCHOOL_TYPE:
                logic.saveJs(host, form);
                break;
            case IMiddleSchoolType.TECH_SCHOOL_TYPE:
                logic.saveTs(host, form);
                break;
            default:
                return this.errPage(request, mapping, "请选择阶段");
        }
        logic.saveStage(host, form.getStage());
        logic.setHsUc(host);
        if (!"".equals(form.getPorId()))
            logic.saveHead(host, form);
        return sbPage(response, request);
    }

    /**
     * 提交白领和其他信息
     *
     * @param form
     * @param response
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午04:27:22
     */
    private ActionForward sbMajorPage(ShortFormFillInfoMajorForm form,
                                      HttpServletResponse response, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        logic.saveStage(host, form.getStage());
        logic.setMajorUc(host);
        if (0 != form.getHomecityCode()) {
            logic.saveLocation(host, form.getHomeprovince(), form
                    .getHomecityName(), form.getHomecityCode());
        }
        if (StringUtils.isNotEmpty(form.getCompany())) {
            logic.setCompany(host, form.getCompany());
        }
        if (!"".equals(form.getPorId()))
            logic.saveHead(host, form);
        return sbPage(response, request);
    }

    /**
     * 提交大学信息
     *
     * @param form
     * @param response
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午04:27:48
     */
    private ActionForward sbUnivPage(ShortFormFillInfoUnivForm form,
                                     HttpServletResponse response, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        logic.saveStage(host, form.getStage());
        logic.setUnivUc(host);
        logic.saveUniv(host, form);
        if (!"".equals(form.getPorId()))
            logic.saveHead(host, form);
        return sbPage(response, request);
    }

    /**
     * 提交信息
     *
     * @param response
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午04:28:00
     */
    private ActionForward sbPage(HttpServletResponse response,
                                 HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        StepStatusLogic.updateStepStatus(host.getId(),
                StepStatusLogic.STEP_FILLINFO);
        return new ActionForward(Globals.urlGuide + "/guide.do",true);
    }

    /**
     * 出错页面
     *
     * @param request
     * @param mapping
     * @param errMsg
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午04:28:13
     */
    private ActionForward errPage(HttpServletRequest request,
                                  ActionMapping mapping, String errMsg) {
        BaseActionUtil.addErr(request, errMsg);
        return initPage(request, mapping);
    }

    /**
     * 初始化页面
     *
     * @param request
     * @param mapping
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午04:28:24
     */
    private ActionForward initPage(HttpServletRequest request,
                                   ActionMapping mapping) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        UserTraceLogic.getInstance().insertRegHis(request, host,
				host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO), 0,
				null, null, null, null, 0);
		return mapping.findForward(FORWARD_SUCC);
	}

}

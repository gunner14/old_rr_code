package com.xiaonei.reg.guide.action;

import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.logic.fillinfo.DXFillinfoLogic;
import com.xiaonei.reg.guide.logic.fillinfo.HSFillinfoLogic;
import com.xiaonei.reg.guide.logic.portrait.CommonPortraitLogic;
import com.xiaonei.reg.guide.model.ElementarySchool;
import com.xiaonei.reg.guide.pojo.CommonPortraitPojo;
import com.xiaonei.reg.guide.pojo.HSFillInfoElementary;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.xce.log.TimeCost;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.validator.DynaValidatorForm;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;

/**
 * 初中fillinfo页<br>
 * <p/>
 * HSFillinfoAction.java
 */
public class CZFillinfoAction extends ProtectAction {

    private final static String fail = "cz";
    private final static String forward_cz_info = "cz";
    // form property
    private final static String form_elementaryschoolyear = "elementaryschoolyear";

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response)
            throws SQLException {
        TimeCost cost = TimeCost.begin("DispatchViewAction");

        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        DynaValidatorForm infoForm = (DynaValidatorForm) form;
        try {
            UserTraceLogic.getInstance().insertRegHis(request, host,
                    host.getAccount(), String.valueOf(RegOprDefine.FILL_INFO),
                    UserAdapter.get(host).getStage(), null, null, null, null, 0);
            cost.endStep("1");
            // 检查用户是否完善过资料
            String toUrl = StepStatusLogic.goWhere(host);
            if (!Utils.getInstance().validateForString(toUrl)
                    && !toUrl
                    .equalsIgnoreCase(Globals.urlGuide+"/fillinfohs.do")) {
                return new ActionForward(toUrl, true);
            }
            cost.endStep("2");
            // 初始化页面
            if (!"post".equalsIgnoreCase(request.getMethod())) {
                return mapping.findForward(forward_cz_info);
            }
            // 处理页面提交数据
            else {
                request.setAttribute("userBasic", DXFillinfoLogic.getInstance()
                        .getUserBasic(host));
                cost.endStep("6");
                int schoolType = HSFillinfoLogic.getInstance().getSchoolType(
                        host);
                cost.endStep("7");
                HSFillinfoLogic.getInstance().checkSchoolType(schoolType, host);
                cost.endStep("8");
                request.setAttribute("highSchool", HSFillinfoLogic
                        .getInstance().getHighSchool(host));
                cost.endStep("9");

                // 更改小学和入学年
                int elementarySchoolId = 0;
                String elementarySchoolName = Escape
                        .stringToHTMLString(infoForm
                                .getString("elementaryschoolname"));
                int elementarySchoolYear = 0;
                request.setAttribute("elementarySchoolName",
                        elementarySchoolName);
                ElementarySchool elementarySchool = HSFillinfoLogic
                        .getInstance().getElementarySchoolByName(
                                elementarySchoolName);
                cost.endStep("12");
                if (elementarySchool != null) {
                    elementarySchoolId = elementarySchool.getId();
                }

                try {
                    elementarySchoolYear = Integer.parseInt(Escape
                            .stringToHTMLString(infoForm
                            .getString(form_elementaryschoolyear)));
                    request.setAttribute("elementarySchoolYear",
                            elementarySchoolYear);
                } catch (Exception e) {
                	BaseActionUtil.addErr(request, "无效的小学入学年");
                    return mapping.findForward(fail);
                }
                /* 再次判断小学信息 */
                if (Utils.getInstance().validateForString(elementarySchoolName)) {
                    cost.endStep("16");
                    BaseActionUtil.addErr(request, "请输入小学");
                    return mapping.findForward(fail);
                }

                HSFillInfoElementary hsfi = new HSFillInfoElementary();
                hsfi.setElementarySchoolId(elementarySchoolId);
                hsfi.setElementarySchoolName(elementarySchoolName);
                hsfi.setElementarySchoolYear(elementarySchoolYear);
                hsfi.setUser(host);
                HSFillinfoLogic.getInstance().updateElementarySchoolInfo(hsfi);
                cost.endStep("21");

                /*
                     * 更新用户的照片资料
                     */
                String headpic = Escape.stringToHTMLString(infoForm
                        .getString("headUrl"));
                String largepic = Escape.stringToHTMLString(infoForm
                        .getString("largeUrl"));
                String mainpic = Escape.stringToHTMLString(infoForm
                        .getString("mainUrl"));
                String tinypic = Escape.stringToHTMLString(infoForm
                        .getString("tinyUrl"));
                CommonPortraitPojo pp = new CommonPortraitPojo();
                pp.setHead(headpic);
                pp.setLarge(largepic);
                pp.setMain(mainpic);
                pp.setTiny(tinypic);
                CommonPortraitLogic.getInstance().updateHeadUrl(host, pp);
                // 存入加星系统
                CommonPortraitLogic.save(host);

                // 记录用户是否完善过资料
                StepStatusLogic.updateStepStatus(host.getId(),
                        StepStatusLogic.STEP_FILLINFO);
                cost.endStep("22");

                return new ActionForward(Globals.urlGuide + "/previewhs.do",
                        true);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            cost.endFinally();
        }
        return null;
    }
}
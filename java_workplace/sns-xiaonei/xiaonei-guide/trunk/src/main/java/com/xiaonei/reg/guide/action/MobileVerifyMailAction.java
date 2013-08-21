/**
 *
 */
package com.xiaonei.reg.guide.action;

import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.dao.UserPassportExtDAO;
import com.xiaonei.reg.register.logic.additional.RegEmailLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegMobileLogic;
import com.xiaonei.reg.register.model.UserPassportExt;

import org.apache.commons.lang.StringUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.SQLException;

/**
 * MobileGzVerifyMailAction<br>
 * 页面初始化<br>
 * 发送验证邮件<br>
 * 验证<br>
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-1 - 下午04:26:34
 */
public class MobileVerifyMailAction extends ProtectAction {

    private static final String ACT_INIT = "init";
    private static final String ACT_SEND_VERIFY_CODE = "sv";// send verify
    private static final String ACT_VERIFY = "verify";// verify

    private static final String PAR_EMAIL = "email";
    private static final String PAR_VERIFY_CODE = "vc";

    private static final String RT_SEND_OK = "0";
    private static final String RT_SEND_FAIL = "10";
    private static final String RT_EMAIL_INVALID = "20";
    private static final String RT_USER_EXIST = "40";

    /**
     * 初始化页面
     */
    private static final String FORWARD_INIT = "init";

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws SQLException {
    	GuideLogger.printLog(" WHY?");
        String act = request.getParameter("act");
        if (false == this.isPost(request)) {
            // for ajax
            // 页面初始化
            return this.init(mapping, form, request);
        } else {
            // 页面初始化
            if (StringUtils.equals(ACT_INIT, act) || StringUtils.isEmpty(act)) {
                return this.init(mapping, form, request);
            }
            // 验证
            else if (StringUtils.equals(ACT_VERIFY, act)) {
                return this.verify(mapping, form, request);
            }
            // 发送验证邮件
            else if (StringUtils.equals(ACT_SEND_VERIFY_CODE, act)) {
                return this.sendVerify(mapping, form, request, response);
            }
            // default
            else {
                String errMsg = "系统错误，请稍后重试";
                return this.err(mapping, form, request, errMsg);
            }
        }
    }

    /**
     * 错误流程
     *
     * @param mapping
     * @param form
     * @param request
     * @param errMsg
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-3 - 下午09:22:02
     */
    private ActionForward err(ActionMapping mapping, ActionForm form, HttpServletRequest request, String errMsg) {
        addErr(request, errMsg);
        request.setAttribute("email", StringUtils.trimToEmpty(request.getParameter(PAR_EMAIL)));
        return this.init(mapping, form, request);
    }

    /**
     * 验证验证码
     *
     * @param mapping
     * @param form
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-3 - 下午09:19:26
     */
    private ActionForward verify(ActionMapping mapping, ActionForm form, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" WHY? mobile 1 "+" host:"+host.getId());
        String vc = StringUtils.trimToEmpty(request.getParameter(PAR_VERIFY_CODE));
        //String ovc = UserAdapter.get(host).getEmailVerify();
        UserPassportExt upe = new UserPassportExt();
        upe.setId(host.getId());
        try {
			upe = UserPassportExtDAO.getInstance().select(upe);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		String ovc = upe == null? "":upe.getEmail_verify();
        
        GuideLogger.printLog(""+" host:"+host.getId()+" vc:"+vc+" ovc:"+ovc);
        if (StringUtils.equalsIgnoreCase(vc, ovc)) {
        	GuideLogger.printLog(""+" host:"+host.getId()+" vc:"+vc+" ovc:"+ovc);
            try {
            	GuideLogger.printLog(""+" host:"+host.getId()+" vc:"+vc+" ovc:"+ovc);
                UserAdapter.updateEmail(host.getId(), host.getAccount(), UserAdapter.get(host).getNewEmail().toLowerCase().trim());
                GuideLogger.printLog(""+" host:"+host.getId()+" vc:"+vc+" ovc:"+ovc);
            } catch (Exception e) {
            	GuideLogger.printLog(""+" host:"+host.getId()+" vc:"+vc+" ovc:"+ovc);
                e.printStackTrace();
                GuideLogger.printLog(""+" host:"+host.getId()+" vc:"+vc+" ovc:"+ovc);
                return this.errStep2(mapping, form, request, "系统错误，请稍后重新填写验证码，或点击信内激活连接");
            }
            // 标志走完此步骤
            GuideLogger.printLog(""+" host:"+host.getId()+" vc:"+vc+" ovc:"+ovc);
            return succ(mapping, form, request);
        } else {
        	GuideLogger.printLog(""+" host:"+host.getId()+" vc:"+vc+" ovc:"+ovc,GuideLogger.WARN);
            return this.errStep2(mapping, form, request, "验证码错误，请重新输入");
        }
    }

    /**
     * 出错后返回第二步
     *
     * @param mapping
     * @param form
     * @param request
     * @param errMsg
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-18 - 下午01:54:22
     */
    private ActionForward errStep2(ActionMapping mapping, ActionForm form, HttpServletRequest request, String errMsg) {
        request.setAttribute("step2", 1);
        return err(mapping, form, request, errMsg);
    }

    /**
     * 发送验证码
     *
     * @param mapping
     * @param form
     * @param request
     * @param response
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-3 - 下午09:18:05
     */
    private ActionForward sendVerify(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) {
        String email = StringUtils.trimToEmpty(request.getParameter(PAR_EMAIL)).toLowerCase();
        if (false == StringFormat.isValidEmailAddr(email)) {
            return this.ajaxOutput(response, RT_EMAIL_INVALID);
        }
        if (UserAdapter.hasUser(email) == true) {
            return this.ajaxOutput(response, RT_USER_EXIST);
        }
        User host = BaseThreadUtil.currentHost(request);
        try {
            // 保存连接
            String saveEmail = RegMobileLogic.getInstance().saveEmail(host, email);
            if (StringUtils.isNotEmpty(saveEmail)) {
                RegEmailLogic.getInstance().sendRegMobilerVerifyEmail(host, email, saveEmail, null);
            }
            return this.ajaxOutput(response, RT_SEND_OK);
        } catch (SQLException e) {
            e.printStackTrace();
            return this.ajaxOutput(response, RT_SEND_FAIL);
        }
    }

    /**
     * 页面初始化
     *
     * @param mapping
     * @param form
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-3 - 下午09:16:42
     */
    private ActionForward init(ActionMapping mapping, ActionForm form, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        if (StringFormat.isValidEmailAddr(host.getAccount()) == true) {
            // 标志走完此步骤
            return succ(mapping, form, request);
        }
        return mapping.findForward(FORWARD_INIT);
    }

    /**
     * 操作成功
     *
     * @param mapping
     * @param form
     * @param request
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-12 - 下午12:14:23
     */
    private ActionForward succ(ActionMapping mapping, ActionForm form, HttpServletRequest request) {
        User host = BaseThreadUtil.currentHost(request);
        // 标志走完此步骤
        StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_VERIFY_MOBILE | StepStatusLogic.STEP_COMPLETE_GUIDE);
        return new ActionForward((Globals.urlGuide + "/guide.do"), true);
    }

    /**
     * ajax 输出
     *
     * @param response
     * @param msg
     * @return null
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午12:27:06
     */
    private ActionForward ajaxOutput(HttpServletResponse response, String msg) {
        response.setContentType("text/html;charset=utf-8");
		try {
			PrintWriter out = response.getWriter();
			out.print(msg);
		} catch (IOException e) {
			e.printStackTrace(System.err);
		}
		return null;
	}
}

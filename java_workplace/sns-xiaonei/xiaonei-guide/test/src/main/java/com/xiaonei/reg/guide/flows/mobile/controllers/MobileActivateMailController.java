/**
 *
 */
package com.xiaonei.reg.guide.flows.mobile.controllers;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForm;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionForward;
import com.xiaonei.reg.guide.flows.oldactionsutil.ActionMapping;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.dao.UserPassportExtDAO;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.model.UserPassportExt;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * MobileActivateController.java
 * 
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-8 新建文件
 */
@Path("mobile-activate-mail.do")
public class MobileActivateMailController {

    private static final String PAR_ID = "id";

    private static final String PAR_VC = "vc";

    private static final ActionForward TRUE = null;
    
    /*<!-- mobile_verifymail_activate_mail -->
        <action path="/mobile-activate-mail" type="com.xiaonei.reg.guide.action.MobileActivateMail" scope="request"  />
    */
    @Get
    @Post
    public String index(Invocation inv) {
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        Map<String, ActionForward> map = new HashMap<String, ActionForward>();
        ActionMapping mapping = new ActionMapping(map);
        return executeBt(mapping, null, request, response).toString();
    }
    
    private void addErr(HttpServletRequest request, String errMsg) {
        BizFilterUtil.addErr(request, errMsg);
    }

    public ActionForward executeBt(ActionMapping mapping, ActionForm form,HttpServletRequest request, HttpServletResponse response){
        GuideLogger.printLog(" MOBILE_ACT_MAIL");
        String verifyCode = request.getParameter(PAR_VC);
        String sUserId = request.getParameter(PAR_ID);
        final ActionForward rt = checkUrl(mapping, request, verifyCode, sUserId);
        if (TRUE == rt) {
            final int userId = Integer.valueOf(sUserId);
            final User user = UserAdapter.getUser(userId);
            try {
            	UserAdapter.updateEmail(userId, user.getAccount(), UserAdapter.get(user).getNewEmail().toLowerCase().trim());
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+user.getId(),GuideLogger.ERROR);
			}
            // 标志走完此步骤
            StepStatusLogic.updateStepStatus(user.getId(), StepStatusLogic.STEP_VERIFY_MOBILE
                    | StepStatusLogic.STEP_COMPLETE_GUIDE);
            return this.succ(user, mapping, request, response);
        } else {
            return rt;
        }

    }

    /**
     * 激活成功
     * 
     * @param user
     * @param mapping
     * @param request
     * @param response
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午03:59:52
     */
    private ActionForward succ(User user, ActionMapping mapping, HttpServletRequest request,
            HttpServletResponse response) {
        try {
            LoginLogic.getInstance().doLoginR(user, request, response, 0, false);
        } catch (Exception e) {
            e.printStackTrace();
            return this.err(mapping, request, "email绑定成功，请用 " + user.getAccount() + " 登录");
        }
        return new ActionForward(Globals.urlHome + "/Home.do", true);
    }

    /**
     * 检查激活连接
     * 
     * @param mapping
     * @param request
     * @param verifyCode
     * @param sUserId
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午03:10:29
     */
    private ActionForward checkUrl(ActionMapping mapping, HttpServletRequest request,
            String verifyCode, String sUserId) {
        int userId = 0;
        GuideLogger.printLog(" MOBILE_ACT_MAIL");
        try {
            userId = Integer.valueOf(sUserId);
        } catch (Exception e) {
            return this.err(mapping, request, "无效的激活连接");
        }
        if (0 == userId) {
            return this.err(mapping, request, "无效的激活连接");
        }
        if (StringUtils.isEmpty(verifyCode)) {
            return this.err(mapping, request, "无效的激活连接");
        }
        User user = null;
        try {
            //user = UserAdapter.getUser(userId); 2010-4-14 19:08:05 为了不转队列……
            user = SnsAdapterFactory.getUserAdapter().get(userId);
        } catch (Exception e) {
            e.printStackTrace();
            return this.err(mapping, request, "系统错误，请稍后重试");
        }
        if (null == user) {
            return this.err(mapping, request, "无效的激活连接，没有此用户");
        }

        UserPassportExt upe = new UserPassportExt();
        upe.setId(user.getId());
        try {
            upe = UserPassportExtDAO.getInstance().select(upe);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        String ovc = upe == null ? "" : upe.getEmail_verify();

        if (StringUtils.equals(verifyCode, ovc/*UserAdapter.get(user).getEmailVerify()*/) == false) {
            return this.err(mapping, request, "无效的激活连接");
        }
        if (StringFormat.isValidEmailAddr(user.getAccount()) == true) {
            return this.err(mapping, request, "您之前已经验证了帐号，请用 " + user.getAccount() + " 登录");
        }
        return TRUE;
    }

    /**
     * 出错
     * 
     * @param mapping
     * @param request
     * @param errMsg
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午03:08:05
     */
    private ActionForward err(ActionMapping mapping, HttpServletRequest request, String errMsg) {
        addErr(request, errMsg);
        return new ActionForward(Globals.urlWww.toString()/*Globals.urlLogin+"/Login.do"*/, true);
    }

}

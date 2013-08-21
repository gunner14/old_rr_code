package com.xiaonei.reg.register.controllers.init;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.controllers.base.BaseRegController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.register.logic.additional.LoginLogic;


/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date Apr 19, 2010 3:53:26 PM
 * Ajax 注册成功后到达页面 原来的激活提示页面
 */

@Path("asrok")
public class FloatRegisterForwardController extends BaseRegController {

    private RegLogger alogger = RegLogger.getLoggerWithType(this.getClass());

    @Override
    public RegForward executeRt(RegMapping mapping, RegForm regForm, HttpServletRequest request,
            HttpServletResponse response) {
        alogger.debug("begin into register activated page .....");

        String regEmail = StringUtils.trimToEmpty(request.getParameter("regEmail")).toLowerCase();
        String vc = StringUtils.trimToEmpty(request.getParameter("vc"));
        
        request.setAttribute("email", regEmail);
        request.setAttribute("emailPage", RegUtil.emailLoginUrl(regEmail));
        User user = UserAdapter.getUser(regEmail, "");
        if(user == null){
            this.addErr(request, "非法的访问链接");
        }else{
            String uvc = UserAdapter.get(user).getVerifyCode();
            if(!vc.equals(uvc)){
                this.addErr(request, "非法的访问链接");                
            }else{
                //自动登录
                LoginLogic.getInstance()
                    .doLoginR(user, request, response, 0, false);
            }
        }
        
        return mapping.findForward("success");
    }

    @Override
    protected Map<String, RegForward> setRegMapping(Map<String, RegForward> mapping) {
        mapping.put("success", new RegForward("/pages/register/reg/register_ok_no_activated.jsp"));
        return null;
    }
}

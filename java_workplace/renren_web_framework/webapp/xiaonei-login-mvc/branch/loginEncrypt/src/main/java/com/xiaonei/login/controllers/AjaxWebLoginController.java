package com.xiaonei.login.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.AsSuperController;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import com.xiaonei.commons.interceptors.access.origurl.DontPutOrigURL;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.form.PassportForm;
import com.xiaonei.login.logic.annotation.Singleton;
import com.xiaonei.login.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.login.logic.validator.annotation.FormValidator;
import com.xiaonei.platform.core.utility.Encoder;

@Path(value = { "ajaxLogin" })
@AsSuperController
@DontPutOrigURL
@Singleton
public class AjaxWebLoginController extends WebLoginController{
	@Post("login")
    public String login(final Invocation inv,//
            @FormValidator(validateAllParams = true) final PassportForm pForm){

        final String pwd=pForm.getPassword();
        final boolean isPasswordEncode = inv.getRequest().getAttribute(LoginParamDef.PARAM_ENPWD) != null ? true
                : false;
        final String account = pForm.getEmail();
        final String password = isPasswordEncode ? pwd : Encoder.encode(pwd);
        inv.getRequest().setAttribute(LoginParamDef.ATTRIBUTE_ENCODE_PASSWORD, password);
        String url=super.post(inv, pForm);
        url=url.replace("r:", "");
        final LoginStatusCode code=(LoginStatusCode) inv.getModel(LoginParamDef.ATTRIBUTE_LOGIN_STATUS_CODE);
        final String ip = getClientIpFromRequest(inv);
        if(LoginStatusCode.LOGIN_OK==code){
            /**
             * 如果有验证码验证，且登陆成功。则清空登陆计数器。否则更新登陆计数器
             */
            CaptchaAdapterFactory.getCaptchaAdapterInstance().processLoginCountAfterLoginSuccess(
                    inv.getRequest(), account, this.getUserId(inv), ip);
            // 生成T票 并跳转
            return "@"+this.createSuccessJSONString(url);
        }else{
            CaptchaAdapterFactory.getCaptchaAdapterInstance().processLoginCountAfterLoginFail(
                    inv.getRequest(), account, code, ip);
            String emailLoginUrl=null;
            if(inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_EMAIL_LOGIN_URL)!=null){
            	emailLoginUrl=(String) inv.getRequest().getAttribute(LoginParamDef.ATTRIBUTE_EMAIL_LOGIN_URL);
            }
            return "@"+this.createFailJSONString(code, CaptchaAdapterFactory.getCaptchaAdapterInstance().getLoginCountFromRequest(inv.getRequest()), url, account, emailLoginUrl);
        }

    }
    /**
     * 根据错误码和次数构建json字符串
     * @param code 错误代码
     * @param count 错误登录次数
     * @return 字符串
     */
    private String createFailJSONString(final LoginStatusCode code, final Integer count, final String url, final String email, final String emailLoginUrl){
        final JSONObject object=new JSONObject();
        object.put("catchaCount", count);
        object.put("code", Boolean.FALSE);
        object.put("homeUrl", url);
		object.put("failDescription", code.getDescription());
		object.put("failCode", code.getCode());
		object.put("email", email);
		object.put("emailLoginUrl", emailLoginUrl);
        return object.toString();
    }
    /**
     * 根据错误码和次数构建json字符串
     * @param code 错误代码
     * @param count 错误登录次数
     * @return 字符串
     */
    private String createSuccessJSONString(final String url){
        final JSONObject object=new JSONObject();
        object.put("code", Boolean.TRUE);
        object.put("homeUrl", url);
        return object.toString();
    }
}

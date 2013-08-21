package com.xiaonei.login.logic.captcha;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.login.constants.LoginStatusCode;

/**
 * 
 * @author xiaodong.yuan
 * 
 *         调用安全中心接口，处理验证码相关内容
 * 
 */
public interface ICaptchaAdapter {
    /**
     * 是否显示验证码
     * 
     * @param account 帐号
     * @return 验证码级别 0为不显示,如果不是renren.com不显示
     */
    public int getCaptchaGrade(HttpServletRequest request, String account, final String ip);

    /**
     * 从request获取失败次数,如果没有则返回0
     * 
     * @param request request
     * @return 现在的登陆次数
     */
    public int getLoginCountFromRequest(HttpServletRequest request);

    /**
     * 当登陆失败后，处理累加器。如果此次登陆失败的原因是因为密码错误，则增加累积器
     * 
     * @param request request
     * @param account 帐号
     * @param rt 失败代码
     */
    public void processLoginCountAfterLoginFail(HttpServletRequest request, String account,
            LoginStatusCode rt, final String ip);

    /**
     * 当登陆成功后，处理累加器。如果此次登陆有验证码的验证，则清空累积器，否则增加累积器
     * 
     * @param request request
     * @param account 帐号
     * @param userId 用户id
     */
    public void processLoginCountAfterLoginSuccess(HttpServletRequest request, String account,
            int userId, final String ip);

    /**
     * 验证验证码
     * 
     * @param icode 验证码
     * @param ick cookie存的生成验证码的值
     * @param 网页发来的域名
     * @return 是否成功
     */
    public boolean validateCatcha(HttpServletRequest request, String account, final String ip);

}

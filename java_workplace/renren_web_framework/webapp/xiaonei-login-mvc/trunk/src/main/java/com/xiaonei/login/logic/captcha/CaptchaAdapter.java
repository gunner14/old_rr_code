package com.xiaonei.login.logic.captcha;

import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.security.logintrack.captcha.policy.CaptchaPolicyFactory;
import com.renren.security.logintrack.status.IStatusAdapter;
import com.renren.security.logintrack.status.client.StatusAdapterClient;
import com.xiaonei.icode.verify.CodeUtil;
import com.xiaonei.login.logic.LoginConfigureImpl;
import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;

public class CaptchaAdapter implements ICaptchaAdapter, IWeakPassword {

    private static CaptchaAdapter _instance = new CaptchaAdapter();
    private static IStatusAdapter statusAdapter = StatusAdapterClient.getInstance();

    private static ExecutorService service = null;

    private static int TIME_OUT_VALUE = 100;

    private final static Log logger = LogFactory.getLog(CaptchaAdapter.class);

    private static com.renren.security.logintrack.captcha.ICaptchaAdapter adapter = CaptchaPolicyFactory
            .getWebCaptchaPolicy();

    private final static String CAPTCHA_ICK_TEMP_VALUE="capcha_ick_temp_value";
    public final static int CAPTCHA_GRADE_DEFUALT_VALUE = 0;

    public final static int LOGIN_COUNT_DEFUALT_VALUE = 0;

    public final static int IS_WEAK_PASSWORD= 0;

    public final static String IS_SHOW_CAPTCHA = "captcha:showCaptcha";

    public final static String LOGIN_COUNT = "captcha:loginCount";
    
    protected PpLogger remoteLogger = LoginLoggerDef.login;
    static {
        service = new ThreadPoolExecutor(300, 700, 2L, TimeUnit.MINUTES,
                new LinkedBlockingQueue<Runnable>(7000));
        
    }

    public static CaptchaAdapter getInstance() {
        return _instance;
    }
    
    private  int getTimeOutValue() {
    	 Map<String,String> loginConfMap = LoginConfigureImpl.getInstance().getLoginConfigure();
         
         return  (loginConfMap != null && loginConfMap.size() != 0) ? 
 				        		(loginConfMap.get(Globals.TIME_OUT_VALUE) != null ? 
 				        		Integer.valueOf((loginConfMap.get(Globals.TIME_OUT_VALUE))) 
 			        				: TIME_OUT_VALUE) 
 			        					: TIME_OUT_VALUE;
    }

    @Override
    public int getCaptchaGrade(final HttpServletRequest request,
            final String account, final String ip) {
        // final String ip = getIp(request);
        if (request == null) {
            return CAPTCHA_GRADE_DEFUALT_VALUE;
        }
        if (this.isNotNeedCaptcha(request)) {
            return CAPTCHA_GRADE_DEFUALT_VALUE;
        }
        if (request.getAttribute(CaptchaAdapter.IS_SHOW_CAPTCHA) == null) {
            final Future<Integer> result = this.ascGetCaptchaGrade(account, ip);
            request.setAttribute(CaptchaAdapter.IS_SHOW_CAPTCHA, result);
        }
        final int value = this.getValueFromRequest(request, this.getTimeOutValue(),
                IS_SHOW_CAPTCHA, CAPTCHA_GRADE_DEFUALT_VALUE);
        if (value > 0) {
            request.setAttribute("needValidate", true);
        }
        return value;
    }

    @Override
    public int getLoginCountFromRequest(final HttpServletRequest request) {
        if (this.isNotNeedCaptcha(request)) {
            return LOGIN_COUNT_DEFUALT_VALUE;
        }
        return this.getValueFromRequest(request, this.getTimeOutValue(), LOGIN_COUNT,
                LOGIN_COUNT_DEFUALT_VALUE);
    }

    @Override
    public boolean isWeakPassword(final int userId) {
        try {
            final Future<Boolean> value= service.submit(new Callable<Boolean>() {
                @Override
                public Boolean call() throws Exception {
                    return statusAdapter.isWeakPassword(userId);
                }
            });
            try {
                if (value.isDone()) {
                    return value.get();
                }
                return value.get(this.getTimeOutValue(), TimeUnit.MILLISECONDS);
            } catch (final InterruptedException e) {
                logger.error("获取弱密码线程发生中断", e);
                return false;
            } catch (final ExecutionException e) {
                logger.error("获取弱密码线程出现异常", e);
                return false;
            } catch (final TimeoutException e) {
                logger.error("获取弱密码数据超时，超时时间设置为" + this.getTimeOutValue() + "毫秒", e);
                ThreadPoolExecutor pool = (ThreadPoolExecutor)service;
                logger.error("当前线程池的核心线程数为：" + pool.getPoolSize());
                logger.error("当前线程池的等待队列数为：" + pool.getQueue().size());
                value.cancel(true);
                return false;
            }
        } catch (final Exception e) {
            logger.error("获取是否弱密码错误， userId=:" + userId);
            return false;
        }
    }

    @Override
    public void processLoginCountAfterLoginFail(
            final HttpServletRequest request, final String account,
            final LoginStatusCode rt, final String ip) {
        if (this.isNotNeedCaptcha(request)) {
            return;
        }
        if ((rt != LoginStatusCode.FAILUSER)
                && (rt != LoginStatusCode.FAILVALIDATELOGIN)&&(rt!=LoginStatusCode.FAILUSERNOTEXIST)) {
            return;
        }
        final int captchaGrade = this.getCaptchaGrade(request, account, ip);
        if (CAPTCHA_GRADE_DEFUALT_VALUE == captchaGrade) {
            this
            .incrementLoginCountOnlyOnce(
                    request,
                    account,
                    0,
                    com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus.None,
                    ip);
        } else {
            if (rt == LoginStatusCode.FAILVALIDATELOGIN) {
                this
                .incrementLoginCountOnlyOnce(
                        request,
                        account,
                        0,
                        com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus.False,
                        ip);
            } else {
                this
                .incrementLoginCountOnlyOnce(
                        request,
                        account,
                        0,
                        com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus.True,
                        ip);
            }
        }
        this.getValueFromRequest(request, this.getTimeOutValue(), LOGIN_COUNT,
                LOGIN_COUNT_DEFUALT_VALUE);
        this.getNowCaptchaGrade(request, account, ip);
    }

    @Override
    public void processLoginCountAfterLoginSuccess(
            final HttpServletRequest request, final String account,
            final int userId, final String ip) {
        if (this.isNotNeedCaptcha(request)) {
            return;
        }
        final int captchaGrade = this.getCaptchaGrade(request, account, ip);
        if (CAPTCHA_GRADE_DEFUALT_VALUE == captchaGrade) {
            this
            .incrementLoginCountOnlyOnce(
                    request,
                    account,
                    userId,
                    com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus.None,
                    ip);
            this.getValueFromRequest(request, this.getTimeOutValue(), LOGIN_COUNT,
                    LOGIN_COUNT_DEFUALT_VALUE);
            this.getNowCaptchaGrade(request, account, ip);
        } else {
            this.clearLoginCount(request, account);
        }
    }

    @Override
    public boolean validateCatcha(final HttpServletRequest request,
            final String account, final String ip) {
        if (this.isNotNeedCaptcha(request)) {
            return true;
        }
        final int captchaGrade = this.getCaptchaGrade(request, account, ip);
        if (CAPTCHA_GRADE_DEFUALT_VALUE == captchaGrade) {
            return true;
        }

        String ick = null;

        final String icode = request.getParameter("icode");
        final String key_id = request.getParameter("key_id");
        String captchaType = request.getParameter("captcha_type");
        if(StringUtils.isEmpty(captchaType)){
            captchaType="login";
        }
        final User user=BaseThreadUtil.currentHost(request);
        if(user!=null){
            ick=String.valueOf(user.getId());
        }else{
            if("web_login".equals(captchaType)){
                ick=CookieManager.getInstance().getCookie(request,
                        "ick_login");
            }
            if(StringUtils.isEmpty(ick)){
                if(!StringUtils.isEmpty(request.getParameter(CAPTCHA_ICK_TEMP_VALUE))){
                    ick=request.getParameter(CAPTCHA_ICK_TEMP_VALUE);
                }else{
                    ick=CookieManager.getInstance().getCookie(request,
                            LoginCookieDef.ICODE);
                }
            }
        }
        this.remoteLogger.info("type:%s;ick:%s;icode:%s;key_id:%s;account:%s;ip:%s", "captcha", ick,
                icode, key_id, account, ip);

        if ((icode == null) ||  (ick == null || icode.length()>8)) {
            logger.debug("icode contains invalid characters");
            return false;
        }
        if (!CodeUtil.getInstance().validCodeForLogin(icode, captchaType , ick, account)) {
            // 没输，或者输入错误，都是不对的
            return false;
        }
        return true;
    }

    /**
     * 也许有的板块不想用验证码，此方法是做是否使用验证码的判断。目前只有人人桌面不需要，需要特殊处理
     * 
     * @param request
     * @return
     */
    private boolean isNotNeedCaptcha(final HttpServletRequest request) {
        //		String userAgent = request.getHeader("User-Agent");
        //		if (StringUtils.startsWith(userAgent, "RenRenClient")) {
        //			return true;
        //		}
        return false;
    }

    /**
     * 异步获取captchaGrade值
     * 
     * @param account
     *            登陆名
     * @param ip
     *            登陆ip
     * @return
     */
    protected Future<Integer> ascGetCaptchaGrade(final String account,
            final String ip) {
        try {
            return service.submit(new Callable<Integer>() {

                @Override
                public Integer call() throws Exception {
                    final int captchaGrade = adapter.getCaptchaGrade(account, ip);
                    logger.info("getCaptchaGrade from safe center : "+account+","+ip+","+captchaGrade);
                    return captchaGrade;
                }
            });
        } catch (final Exception e) {
            logger.error("获取是否显示验证码异常， accoun=:" + account);
            return new CaptchaFuture();
        }
    }

    /**
     * 异步增加登陆次数
     * 
     * @param account
     *            登陆帐号
     * @param userId
     *            用户id
     * @param ip
     *            用户ip
     * @param captchaStatus
     *            验证码状态
     * @return
     */
    protected Future<Integer> asynIncrementLoginCount(
            final String account,
            final int userId,
            final String password,
            final String ip,
            final com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus captchaStatus) {
        try {
            return service.submit(new Callable<Integer>() {

                @Override
                public Integer call() throws Exception {
                    return adapter.incrementLoginCount(account, userId,
                            password, captchaStatus, ip);
                }
            });
        } catch (final RejectedExecutionException e) {
            logger.error("异步增加登陆次数出错, 处理验证码的线程队列已满， accoun=:" + account);
            return new CaptchaFuture(LOGIN_COUNT_DEFUALT_VALUE);
        }

    }

    /**
     * 清空计数器
     * 
     * @param account
     *            帐号
     */
    protected void clearLoginCount(final HttpServletRequest request,
            final String account) {
        if (logger.isDebugEnabled()) {
            logger
            .debug("start clear user's login count, the user's account is "
                    + account);
        }
        try {
            service.execute(new Runnable() {

                @Override
                public void run() {
                    adapter.clearLoginCount(account);
                }
            });
        } catch (final RejectedExecutionException e) {
            logger.error("处理验证码的线程队列已满， accoun=:" + account);
            adapter.clearLoginCount(account);
        }

    }

    /**
     * 是否显示验证码
     * 
     * @param account
     *            帐号
     * @return 验证码级别 0为不显示,如果不是renren.com不显示
     */
    protected int getNowCaptchaGrade(final HttpServletRequest request,
            final String account, final String ip) {
        request.removeAttribute(CaptchaAdapter.IS_SHOW_CAPTCHA);
        return this.getCaptchaGrade(request, account, ip);
    }

    /**
     * 从request中获取future对象。判断在超时时间内是否可以返回值。如果不能返回值。则设置默认值返回给客户端
     * 
     * @param request
     * @param timeout
     * @param key
     * @return
     */
    @SuppressWarnings("unchecked")
    protected Integer getValueFromRequest(final HttpServletRequest request,
            final long timeout, final String key, final int defaultValue) {
        if ((request == null) || (request.getAttribute(key) == null)) {
            logger.warn("request中没有对应的验证码相关数据，此种情况可能存在异常。验证码业务代码:" + key);
            return defaultValue;
        }
        final Future<Integer> value = (Future<Integer>) request
                .getAttribute(key);
        try {
            if (value.isDone()) {
                return value.get();
            }
            return value.get(timeout, TimeUnit.MILLISECONDS);
        } catch (final InterruptedException e) {
            logger.error("获取验证码相关数据线程发生中断，验证码业务代码:" + key, e);
            request.setAttribute(key, new CaptchaFuture());
            return defaultValue;
        } catch (final ExecutionException e) {
            logger.error("获取验证码相关业务时，线程出现异常，验证码业务代码:" + key, e);
            request.setAttribute(key, new CaptchaFuture());
            return defaultValue;
        } catch (final TimeoutException e) {
            logger.error("获取验证码相关数据超时，超时时间设置为" + this.getTimeOutValue() + "毫秒，验证码业务代码:"
                    + key, e);
            ThreadPoolExecutor pool = (ThreadPoolExecutor)service;
            logger.error("当前线程池的核心线程数为：" + pool.getPoolSize());
            logger.error("当前线程池的等待队列数为：" + pool.getQueue().size());
            value.cancel(true);
            request.setAttribute(key, new CaptchaFuture());
            return defaultValue;
        }

    }

    /**
     * 如果已经记过次数，则不处理，否则增加次数，并把结果放在request中
     * 
     * @param request
     *            request
     * @param account
     *            登陆帐号
     * @param userId
     *            用户id
     * @param captchaStatus
     */
    protected void incrementLoginCountOnlyOnce(
            final HttpServletRequest request,
            final String account,
            final int userId,
            final com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus captchaStatus,
            final String ip) {
        // final String ip = getIp(request);
        String password = null;
        if (request.getAttribute(LoginParamDef.ATTRIBUTE_ENCODE_PASSWORD) != null) {
            password =(String)request.getAttribute(LoginParamDef.ATTRIBUTE_ENCODE_PASSWORD);
        }
        if (request.getAttribute(CaptchaAdapter.LOGIN_COUNT) == null) {
            final Future<Integer> future = this.asynIncrementLoginCount(
                    account, userId, password, ip, captchaStatus);
            request.setAttribute(LOGIN_COUNT, future);
        }
    }

}

package com.xiaonei.passport.logic.captcha;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.security.logintrack.captcha.policy.CaptchaPolicyFactory;
import com.renren.security.logintrack.status.IStatusAdapter;
import com.renren.security.logintrack.status.client.StatusAdapterClient;
import com.xiaonei.icode.verify.CodeUtil;
import com.xiaonei.passport.ILoginLocalVar;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.logger.PpLogger;

public class CaptchaAdapter implements ICaptchaAdapter, IWeakPassword {

    private static CaptchaAdapter _instance = new CaptchaAdapter();
    private static IStatusAdapter statusAdapter = StatusAdapterClient.getInstance();

    private static ExecutorService service = null;

    private static int TIME_OUT_VALUE = 100;

    private final static Log logger = LogFactory.getLog(CaptchaAdapter.class);

    private static com.renren.security.logintrack.captcha.ICaptchaAdapter adapter = new CaptchaPolicyFactory().getWebCaptchaPolicy();

    public final static int CAPTCHA_GRADE_DEFUALT_VALUE = 0;

    public final static int LOGIN_COUNT_DEFUALT_VALUE = 0;

    public final static int IS_WEAK_PASSWORD= 0;

    public final static String IS_SHOW_CAPTCHA = "captcha:showCaptcha";

    public final static String LOGIN_COUNT = "captcha:loginCount";
    protected PpLogger remoteLogger = com.xiaonei.passport.logger.LoginLoggerDef.login;
    static {
        service = new ThreadPoolExecutor(300, 700, 2L, TimeUnit.MINUTES,
                new LinkedBlockingQueue<Runnable>(7000));
    }

    public static CaptchaAdapter getInstance() {
        return _instance;
    }

    @Override
    public int getCaptchaGrade(final ILoginLocalVar context,
            final String account, final String ip) {
        if (context == null) {
            return CAPTCHA_GRADE_DEFUALT_VALUE;
        }
        if (context.getAttribute(CaptchaAdapter.IS_SHOW_CAPTCHA) == null) {
            final Future<Integer> result = this.ascGetCaptchaGrade(account, ip);
            context.setAttribute(CaptchaAdapter.IS_SHOW_CAPTCHA, result);
        }
        //为了测试修改，目前把validate(userId)注释掉， 去掉验证码的显示标志
        final int value = this.getValueFromContext(context, TIME_OUT_VALUE,
               IS_SHOW_CAPTCHA, CAPTCHA_GRADE_DEFUALT_VALUE);
        //final int value = 0;
        if (value > 0) {
        	context.setAttribute("needValidate", true);
        }
        return value;
    }

    @Override
    public int getLoginCountFromContext(final ILoginLocalVar context) {
    	//为了测试修改，目前把validate(userId)注释掉， 去掉验证码的显示标志
        return this.getValueFromContext(context, TIME_OUT_VALUE, LOGIN_COUNT,
                LOGIN_COUNT_DEFUALT_VALUE);
    	//return 0;
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
                return value.get(TIME_OUT_VALUE, TimeUnit.MILLISECONDS);
            } catch (final InterruptedException e) {
                logger.error("获取弱密码线程发生中断", e);
                return false;
            } catch (final ExecutionException e) {
                logger.error("获取弱密码线程出现异常", e);
                return false;
            } catch (final TimeoutException e) {
                logger.error("获取弱密码数据超时，超时时间设置为" + TIME_OUT_VALUE + "毫秒", e);
                return false;
            }
        } catch (final Exception e) {
            logger.error("获取是否弱密码错误， userId=:" + userId);
            return false;
        }
    }

    @Override
    public void processLoginCountAfterLoginFail(ILoginLocalVar context, String account,final String password,
            LoginStatusCode rt, String ip) {
        if ((rt != LoginStatusCode.FAILUSER)
                && (rt != LoginStatusCode.FAILVALIDATELOGIN)&&(rt!=LoginStatusCode.FAILUSERNOTEXIST)) {
            return;
        }
        final int captchaGrade = this.getCaptchaGrade(context, account, ip);
        if (CAPTCHA_GRADE_DEFUALT_VALUE == captchaGrade) {
            this
            .incrementLoginCountOnlyOnce(
                    context,
                    account, password,
                    0,
                    com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus.None,
                    ip);
        } else {
            if (rt == LoginStatusCode.FAILVALIDATELOGIN) {
                this
                .incrementLoginCountOnlyOnce(
                        context,
                        account, password,
                        0,
                        com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus.False,
                        ip);
            } else {
                this
                .incrementLoginCountOnlyOnce(
                        context,
                        account,password,
                        0,
                        com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus.True,
                        ip);
            }
        }
        //为了测试修改，目前把validate(userId)注释掉， 去掉验证码的显示标志
        this.getValueFromContext(context, TIME_OUT_VALUE, LOGIN_COUNT,
                LOGIN_COUNT_DEFUALT_VALUE);
        this.getNowCaptchaGrade(context, account, ip);
    }

    @Override
    public void processLoginCountAfterLoginSuccess(
            final ILoginLocalVar context, final String account,final String password,
            final int userId, final String ip) {
        final int captchaGrade = this.getCaptchaGrade(context, account, ip);
        if (CAPTCHA_GRADE_DEFUALT_VALUE == captchaGrade) {
            this
            .incrementLoginCountOnlyOnce(
                    context,
                    account, password,
                    userId,
                    com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus.None,
                    ip);
            //为了测试修改，目前把validate(userId)注释掉， 去掉验证码的显示标志
            this.getValueFromContext(context, TIME_OUT_VALUE, LOGIN_COUNT,
                    LOGIN_COUNT_DEFUALT_VALUE);
            this.getNowCaptchaGrade(context, account, ip);
        } else {
            this.clearLoginCount(context, account);
        }
    }

    @Override
    public boolean validateCatcha(ILoginLocalVar context, String account, String icode,
			String captchaType, String ick, String ip) {
        final int captchaGrade = this.getCaptchaGrade(context, account, ip);
        if (CAPTCHA_GRADE_DEFUALT_VALUE == captchaGrade) {
            return true;
        }
        this.remoteLogger.info("type:%s;ick:%s;icode:%s;key_id:%s;account:%s;ip:%s", "captcha", ick,
                icode, 1, account, ip);
        
        if (StringUtils.isEmpty(icode) || StringUtils.isEmpty(ick)) {
            return false;
        }
        if(StringUtils.isEmpty(captchaType)){
        	captchaType="login";
        }
        if (!CodeUtil.getInstance().validCodeForLogin(icode, captchaType , ick, account)) {
            // 没输，或者输入错误，都是不对的
            return false;
        }
        return true;
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
    protected void clearLoginCount(final ILoginLocalVar context,
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
    protected int getNowCaptchaGrade(final ILoginLocalVar context,
            final String account, final String ip) {
        context.removeAttribute(CaptchaAdapter.IS_SHOW_CAPTCHA);
        return this.getCaptchaGrade(context, account, ip);
    }

    /**
     * 从context中获取future对象。判断在超时时间内是否可以返回值。如果不能返回值。则设置默认值返回给客户端
     * 
     * @param context
     * @param timeout
     * @param key
     * @return
     */
    @SuppressWarnings("unchecked")
    protected Integer getValueFromContext(final ILoginLocalVar context,
            final long timeout, final String key, final int defaultValue) {
        if ((context == null) || (context.getAttribute(key) == null)) {
            logger.warn("context中没有对应的验证码相关数据，此种情况可能存在异常。验证码业务代码:" + key);
            return defaultValue;
        }
        final Future<Integer> value = (Future<Integer>) context
                .getAttribute(key);
        try {
            if (value.isDone()) {
                return value.get();
            }
            return value.get(timeout, TimeUnit.MILLISECONDS);
        } catch (final InterruptedException e) {
            logger.error("获取验证码相关数据线程发生中断，验证码业务代码:" + key, e);
            context.setAttribute(key, new CaptchaFuture());
            return defaultValue;
        } catch (final ExecutionException e) {
            logger.error("获取验证码相关业务时，线程出现异常，验证码业务代码:" + key, e);
            context.setAttribute(key, new CaptchaFuture());
            return defaultValue;
        } catch (final TimeoutException e) {
            logger.error("获取验证码相关数据超时，超时时间设置为" + TIME_OUT_VALUE + "毫秒，验证码业务代码:"
                    + key, e);
            context.setAttribute(key, new CaptchaFuture());
            return defaultValue;
        }

    }

    /**
     * 如果已经记过次数，则不处理，否则增加次数，并把结果放在context中
     * 
     * @param context
     *            context
     * @param account
     *            登陆帐号
     * @param userId
     *            用户id
     * @param captchaStatus
     */
    protected void incrementLoginCountOnlyOnce(
            final ILoginLocalVar context,
            final String account,
            final String password,
            final int userId,
            final com.renren.security.logintrack.captcha.ICaptchaAdapter.CaptchaStatus captchaStatus,
            final String ip) {
        if (context.getAttribute(CaptchaAdapter.LOGIN_COUNT) == null) {
            final Future<Integer> future = this.asynIncrementLoginCount(
                    account, userId, password, ip, captchaStatus);
            context.setAttribute(LOGIN_COUNT, future);
        }
    }


}

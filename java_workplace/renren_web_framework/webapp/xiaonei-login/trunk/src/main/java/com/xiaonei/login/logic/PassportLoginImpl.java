package com.xiaonei.login.logic;

import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Properties;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.afterlogin_chains.LoginResultChains;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.login.logic.status.ValidationImpl;
import com.xiaonei.login.logic.ticket.PassportTicketOprImpl;
import com.xiaonei.login.model.LoginModel;
import com.xiaonei.login.task.RunnableTaskExecutor;
import com.xiaonei.login.task.set.FlyerTaskSet;
import com.xiaonei.login.task.set.LogTaskSet;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.xce.passport.Account;
import com.xiaonei.xce.passport.PassportAdapterFactory;

@Service
public class PassportLoginImpl implements PassportLogin, InitializingBean {

    public static final Set<String> blackSet = new HashSet<String>();

    public static final Set<String> whiteSet = new HashSet<String>();

    public static void main(final String[] args) {
        System.out.println(p.matcher("aa@aa.com").matches());
        System.out.println(p.matcher("123456").matches());
    }

    @Autowired
    private ValidationImpl validation;

    protected final Log logger = LogFactory.getLog(this.getClass());

    protected PpLogger remoteLogger = LoginLoggerDef.login;

    private static final Pattern p = Pattern.compile("\\w+");

    @Override
    public void afterPropertiesSet() throws Exception {
        InputStream in = null;
        try {
            in = PassportTicketOprImpl.class.getClassLoader().getResourceAsStream(
                    "origurl-list.properties");
            if (null == in) {
                System.out.print("origurl-list.properties no");
                return;
            }
            final Properties p = new Properties();
            p.load(in);
            final Set<Object> keys = p.keySet();
            for (final Object object : keys) {
                final String host = (String) object;
                final String property = p.getProperty(host);
                if ("w".equals(property)) {
                    PassportLoginImpl.whiteSet.add(property);
                } else if ("b".equals(property)) {
                    PassportLoginImpl.blackSet.add(property);
                }
            }
        } catch (final Exception e) {
            e.printStackTrace();
        } finally {
            if (null != in) {
                try {
                    in.close();
                } catch (final Exception e) {
                    e.printStackTrace();
                }
            }
        }

    }

    @Override
    public void doTaskAfterLogin(final HttpServletRequest request, final int userId) {
        final String ip = BizFilterUtil.getClientIP(request);
        // 后继任务
        RunnableTaskExecutor.getInstance().runTaskForFM(
                new LogTaskSet(userId, ip, request.getRequestURL().toString()));
        RunnableTaskExecutor.getInstance().runTaskForFM(
                new FlyerTaskSet(userId, ip, request.getParameter(LoginParamDef.PARAM_DOMAIN)));
    }

    @Override
    public List<Account> getAccountsByEmail(String email) {
        email = StringUtils.trimToEmpty(email);
        if (StringUtils.isEmpty(email)) {
            return new ArrayList<Account>(0);
        }

        return PassportAdapterFactory.getWebLogin().get(email, Globals.defaultLogin);
    }

    @Override
    public List<Account> getAccountsById(final int ticketId) {
        return PassportAdapterFactory.getWebLogin().getById(ticketId);
    }

    @Override
    public String getOrigUrl(final String domain, String origUrl, final boolean needNotify,
            final Object weakPassportTag) {
        if (StringUtils.isEmpty(origUrl)) {
            origUrl = "http://home." + domain + "/home";
        }
        // 如果不能构造成url对象,则认为这是一个url encode 的结果,需要decode
        try {
            new URL(origUrl);
        } catch (final MalformedURLException e1) {
            try {
                origUrl = URLDecoder.decode(origUrl, "UTF-8");
            } catch (final UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        }
        if (origUrl.contains(OpiConstants.urlLogin + "/GLogin.do") // login
                // || origURL.contains("SysHome.do") // 不跳回主页
                // || origUrl.contains(OpiConstants.urlPassport) // login
                ) {
            origUrl = "http://home." + domain + "/home";
        }

        //如果白名单里面有值,走白名单
        if (!this.inWhite(origUrl)) {
            origUrl = "http://home." + domain + "/home";
        }

        if (needNotify && (weakPassportTag != null)) {
            String origUrlEncode;
            try {
                origUrlEncode = URLEncoder.encode(origUrl, "UTF-8");
            } catch (final UnsupportedEncodingException e) {
                e.printStackTrace();
                origUrlEncode = origUrl;
            }
            final String plus = "lk".equals(weakPassportTag) ? "check=1&" : "";
            origUrl = OpiConstants.urlSafeCenter + "/Notice.do?" + plus + "origURL="
                    + origUrlEncode;
        }

        return origUrl;
    }

    @Override
    public String getOrigUrlEncode(final String domain, final String origUrl,
            final boolean needNotify, final Object weakPassportTag) {
        try {
            return URLEncoder.encode(this.getOrigUrl(domain, origUrl, needNotify, weakPassportTag),
                    "UTF-8");
        } catch (final UnsupportedEncodingException e) {
            e.printStackTrace();
            return "";
        }
    }

    @Override
    public LoginStatusCode login(final HttpServletRequest request,
            final HttpServletResponse response, final boolean isAutoLogin, final String email,
            final String pwd, final String domain, final LoginResultChains chains) {
        //默认不使用新的接口
        return this.login(request, response, isAutoLogin, email, pwd, domain, chains, false);

    }

    @Override
    public LoginStatusCode login(final HttpServletRequest request,
            final HttpServletResponse response, final boolean isAutoLogin, final String email,
            final String pwd, final String domain, final LoginResultChains chains,
            final boolean hasVirtual) {
        // weblogin
        final boolean isPasswordEncode = request.getAttribute(LoginParamDef.PARAM_ENPWD) != null ? true
                : false;
        final String passwordEncode = isPasswordEncode ? pwd : Encoder.encode(pwd);
        final String ip = BizFilterUtil.getClientIP(request);

        /* 地址封禁过滤 */
        final boolean isBan = this.validation.isBanIP(ip);
        if (isBan) {
            return LoginStatusCode.FAILBANIP;
        }
        /* 根据用户名和密码获取用户对象 */
        Account account = null;
        if (Globals.C56_DOMAIN.equals(domain)) { //56登陆的先验证56，否则先验证人人
            account = this.process56User(account, hasVirtual, domain, isAutoLogin, passwordEncode,
                    email);
            account = this.processNon56User(account, hasVirtual, domain, isAutoLogin,
                    passwordEncode, email);
        } else {
            account = this.processNon56User(account, hasVirtual, domain, isAutoLogin,
                    passwordEncode, email);
            account = this.process56User(account, hasVirtual, domain, isAutoLogin, passwordEncode,
                    email);
        }

        if (null == account) {
            return LoginStatusCode.FAILDEFAULT;
        }

        final LoginModel loginModel = new LoginModel();
        loginModel.setIp(ip);
        loginModel.setPwd(pwd);
        loginModel.setUserName(email);
        loginModel.setAccount(account);
        loginModel.setRequest(request);
        loginModel.setDomain(domain);
        loginModel.setPer(isAutoLogin);
        loginModel.setResponse(response);
        return chains.getFristNode().doChain(loginModel, LoginStatusCode.LOGIN_OK);

    }

    @SuppressWarnings("unused")
    private boolean inBlack(final String url) {
        final String black = "";
        final Pattern pattern = Pattern.compile("^(http://)?([0-9a-zA-Z\\.]+\\.)?" + black
                + "(/.*)?");
        final Matcher matcher = pattern.matcher(url);
        return matcher.matches();
    }

    private boolean inWhite(final String url) {
        //        Set<String> whiteSet = PassportLoginImpl.whiteSet;
        //        for (String white : whiteSet) {
        //            Pattern pattern = Pattern.compile("^(http://)?([0-9a-zA-Z\\.]+\\.)?" + white + "(/.*)?");
        //            Matcher matcher = pattern.matcher(url);
        //            if(matcher.matches()){
        return true;
        //            }
        //        }
        //        return false;
    }

    private boolean needProcessAnthorFlow(final Account account){
        if(account == null){
            return false;
        }
        // 输入密码错误
        if (account.getSuccess() == -2) {
            return true;
        }
        // 输入用户不存在
        if (account.getSuccess() == -1) {
            return true;
        }
        if (account.getSuccess() < 0) {
            return true;
        }
        if (account.getId() <= 0) {
            return true;
        }
        return false;
    }

    private Account passportAdapterLogin(final String email, final boolean hasVirtual,
            final String domain, final boolean isAutoLogin, final String passwordEncode) {
        Account account = null;
        try {
            if (hasVirtual) {
                account = PassportAdapterFactory.getWebLogin().loginVirtual(//
                        Globals.defaultLogin,//
                        email,//
                        passwordEncode,//
                        domain, //
                        isAutoLogin ? -1 : 6 * 3600,//
                                true//
                        );
            } else {
                account = PassportAdapterFactory.getWebLogin().login(//
                        Globals.defaultLogin,//
                        email,//
                        passwordEncode,//
                        domain, //
                        isAutoLogin ? -1 : 6 * 3600,//
                                true//
                        );
            }
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return account;
    }

    /**
     * 处理56冲突的用户
     * 
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Dec 13, 2011 2:05:21 PM
     * 
     * @param account
     * @param hasVirtual
     * @param domain
     * @param isAutoLogin
     * @param passwordEncode
     * @param email
     * @return
     */
    private Account process56User(final Account account, final boolean hasVirtual,
            final String domain, final boolean isAutoLogin, final String passwordEncode,
            final String email) {
        if ((account != null) && !this.needProcessAnthorFlow(account)) {
            return account;
        }
        final Matcher m = PassportLoginImpl.p.matcher(email);
        if (!m.matches()) {
            return account;
        }
        return this.passportAdapterLogin(email + "@56", hasVirtual, domain, isAutoLogin,
                passwordEncode);
    }

    /**
     * 处理非56帐号
     * 
     * @author Wang Tai (cn.wang.tai@gmail.com)
     * @date Dec 13, 2011 4:29:31 PM
     * 
     * @param account
     * @param hasVirtual
     * @param domain
     * @param isAutoLogin
     * @param passwordEncode
     * @param email
     * @return
     */
    private Account processNon56User(final Account account, final boolean hasVirtual,
            final String domain, final boolean isAutoLogin, final String passwordEncode,
            final String email) {
        if ((account != null) && !this.needProcessAnthorFlow(account)) {
            return account;
        }
        return this.passportAdapterLogin(email, hasVirtual, domain, isAutoLogin, passwordEncode);
    }
}

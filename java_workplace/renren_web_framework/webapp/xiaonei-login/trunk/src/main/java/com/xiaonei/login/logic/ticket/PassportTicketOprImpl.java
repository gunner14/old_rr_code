package com.xiaonei.login.logic.ticket;

import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.HashSet;
import java.util.Properties;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.login.cache.AbstractCommonCache;
import com.xiaonei.login.cache.AutoLoginCache;
import com.xiaonei.login.cache.CacheTime;
import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginCookieDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.logic.PassportLoginImpl;
import com.xiaonei.login.util.LoginDateTimeUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.sns.platform.core.opt.base.CookieManager;
import com.xiaonei.xce.passport.PassportAdapterFactory;

@Service
public class PassportTicketOprImpl implements PassportTicketOpr, InitializingBean {

    //    public static int TAIL_NUM = -1;

    private static final Set<Integer> theIdsOfMergeUsers = new HashSet<Integer>();

    protected Log logger = LogFactory.getLog(this.getClass());

    @Autowired
    PassportLoginImpl passportLoginImpl;

    @Override
    public void afterPropertiesSet() throws Exception {
        InputStream in = null;
        try {
            in = PassportTicketOprImpl.class.getClassLoader().getResourceAsStream(
                    "combination-account-list.properties");
            if (null == in) {
                System.out.print("combination-account-list.properties no");
                return;
            }
            final Properties p = new Properties();
            p.load(in);
            final Set<Object> keys = p.keySet();
            for (final Object object : keys) {
                final String sId = (String) object;
                final Integer iId = Integer.parseInt(sId);
                if (null != iId) {
                    PassportTicketOprImpl.theIdsOfMergeUsers.add(iId);
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
    public String forwardSetTicket(final HttpServletRequest request, final String ticket,
            String domain, String origUrl, final String p3p, final boolean autoLogin,
            final boolean needNotify) {

        domain = Globals.RENREN_DOMAIN;
        //        origUrl = "";
        String url = "http://www." + domain + "/callback.do?";
        url = url + "t=" + ticket;
        origUrl = this.getOrigUrl(request, domain, origUrl, needNotify);
        if (StringUtils.isNotEmpty(origUrl)) {
            try {
                url = url + "&origURL=" + URLEncoder.encode(origUrl, "UTF-8");
            } catch (final UnsupportedEncodingException e) {
                url = url + "&origURL=" + origUrl;
            }
        }
        if ((p3p != null) && !"".equals(p3p)) {
            url += "&p3p=true";
        }
        if (autoLogin || (request.getAttribute(LoginParamDef.ATTRIBUTE_AUTOLOGIN) != null)) {
            url += "&autoLogin=true";
        }

        if (needNotify
                && Boolean.parseBoolean((String) request
                        .getAttribute(LoginParamDef.ATTRIBUTE_LOGIN_CROSS))) {
            url += "&needNotify=true";
        } else {
            url += "&needNotify=false";
        }
        
        return url;
    }

    @Override
    public String getPassportFromCookie(final HttpServletRequest request) {
        return CookieManager.getInstance().getCookie(request, LoginCookieDef.COOKIE_PASSPORT);
    }

    @Override
    public int getUserIdByPassport(final HttpServletRequest request,
            final HttpServletResponse response) {
        final CookieManager cookieManager = CookieManager.getInstance();

        final String passport = this.getPassportFromCookie(request);
        // passport无效
        if (passport == null) {
            return 0;
        }
        final String ap = cookieManager.getCookie(request, LoginCookieDef.AUTOLOGIN);
        final int userid = this.ticket2Id(passport);
        // 没有用户情况下,清空cookie passport
        if (userid <= 0) {
            cookieManager.clearCookie(response, LoginCookieDef.COOKIE_PASSPORT, 0, "/",
                    Globals.PASSPORT_DOMAIN);
            this.logger.warn("Error Passport Ticket:" + passport + (null == ap ? "" : ":" + ap));
            return 0;
        }
        request.setAttribute(LoginParamDef.ATTRIBUTE_HOSTID, userid);

        // 没有t票,但是有ap的时候认为是自动登录
        if ((cookieManager.getCookie(request, LoginCookieDef.loginKey_ticket) == null)
                && (ap != null)) {
            // 自动登录的,重启种入p票
            this.savePassport2Cookie(userid, request, response, passport, true);
            request.setAttribute(LoginParamDef.ATTRIBUTE_AUTOLOGIN, true);
        }
        return userid;
    }

    @Override
    public String id2Ticket(final int id) {
        return PassportAdapterFactory.getWebLogin().id2Ticket(id);
    }

    @Override
    public void savePassport2Cookie(final int userid, final HttpServletRequest request,
            final HttpServletResponse response, final String passport, final boolean isPersistent) {
        if (isPersistent) {
            CookieManager.getInstance().saveCookie(response, LoginCookieDef.COOKIE_PASSPORT,
                    passport, 30 * 24 * 60 * 60, "/", Globals.PASSPORT_DOMAIN);
            CookieManager.getInstance().saveCookie(response, LoginCookieDef.AUTOLOGIN, "" + userid,
                    30 * 24 * 60 * 60, "/", Globals.PASSPORT_3RD_DOMAIN);
        } else {
            CookieManager.getInstance().saveCookie(response, LoginCookieDef.COOKIE_PASSPORT,
                    passport, -1, "/", Globals.PASSPORT_DOMAIN);
        }
        CookieManager.getInstance().saveCookie(response, LoginCookieDef.FIRST_LOGIN_FLAG, "1", -1, "/", Globals.RENREN_DOMAIN);
    }

    @Override
    public int setTicket(final String ticket, final boolean autoLogin,
            final HttpServletRequest request, final HttpServletResponse response) {
        try {
            if ((ticket != null) && (ticket.trim().length() > 0)) {
                // Webpager用
                int hostid;
                hostid = PassportAdapterFactory.getWebTicket().verifyTicket(ticket,
                        PassportManager.getInstance().getTicketProfiler(request));
                // 无条件写入ticket
                CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket,
                        ticket, -1, "/", "." + OpiConstants.domainMain);
                CookieManager.getInstance().saveCookie(response, LoginCookieDef.loginKey_ticket,
                        Encoder.encode(ticket), -1, "/xtalk/", OpiConstants.domainMain.toString());
                CookieManager.getInstance().saveCookie(response,
                        LoginCookieDef.loginKey_old_ticket, ticket, -1, "/",
                        "." + OpiConstants.domainMain);

                // 他们要的(webpager JS FE)
                CookieManager.getInstance().saveCookie(response, LoginCookieDef.ID, "" + hostid);
                // 黄欢要的
                CookieManager.getInstance().saveCookie(response, LoginCookieDef.XNSID,
                        Integer.toHexString((int) (System.currentTimeMillis() ^ -1)));
                return hostid;
            }
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    @Override
    public int ticket2Id(final String passport) {
        try {
            return PassportAdapterFactory.getWebLogin().ticket2Id(passport);
        } catch (final Exception e) {
            e.printStackTrace();
            return -1;
        }
    }

    /**
     * 获取最终跳转链接(包含弱密码跳转逻辑)
     * 
     * @param origUrl
     * @return
     * @throws UnsupportedEncodingException
     */

    private String getOrigUrl(final HttpServletRequest request, final String domain,
            String origUrl, final boolean needNotify) {
        final String attr_puzzle_origurl = (String) request
                .getAttribute(LoginParamDef.ATTRIBUTE_PUZZLE_ORIGURL);
        if (StringUtils.isNotEmpty(attr_puzzle_origurl)) {
            origUrl = attr_puzzle_origurl;
        }
        final String attr_origurl = (String) request.getAttribute(LoginParamDef.ATTRIBUTE_ORIGURL);
        if (StringUtils.isEmpty(origUrl)) {
            origUrl = attr_origurl;
        }
        return this.passportLoginImpl.getOrigUrl(domain, origUrl, needNotify,
                request.getAttribute(LoginParamDef.ATTRIBUTE_WEAK_PASSWORD_WARNING));
    }

	@Override
	public boolean  isAutoLoginByDay(int userId) {
		AbstractCommonCache autoLoginCache = AutoLoginCache.getInstance();
		String count = (String) autoLoginCache.get(String.valueOf(userId));
		if (StringUtils.isNotEmpty(count)) {
			return true;
		}else {
			//TODO: 关于缓存过期时间有待商榷
			String startDate = LoginDateTimeUtil.addDateEndfix(LoginDateTimeUtil.formatDateToStr(LoginDateTimeUtil.getCurrDate()));
	    	Long timeInt     = (LoginDateTimeUtil.getFormatDate(startDate,"yyyy-MM-dd HH:mm:ss").getTime()-LoginDateTimeUtil.getCurrDate().getTime())/1000;
		    autoLoginCache.set(String.valueOf(userId), String.valueOf(1),timeInt.intValue());
		    return false;
		}
		
	}

}

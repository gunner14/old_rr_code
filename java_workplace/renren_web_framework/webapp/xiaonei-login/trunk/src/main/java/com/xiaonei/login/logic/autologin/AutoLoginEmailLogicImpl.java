package com.xiaonei.login.logic.autologin;

import java.net.URLEncoder;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.email.service.control.ImmediateEmail;
import com.xiaonei.email.service.model.EmailConstants;
import com.xiaonei.login.dao.UserPassportExtDAO;
import com.xiaonei.login.model.UserPassportExt;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.xce.passport.Account;
import com.xiaonei.xce.passport.PassportAdapterFactory;

@Service
public class AutoLoginEmailLogicImpl implements AutoLoginEmailLogic {

    private static String emailContent = "您好，%s<br />"
            + "我们发现您在登录网站时遇到了困难，点击以下链接可以自动登录并设置新的登录密码。<br />" + "<a href='%s'>%s</a><br />"
            + "如果上面的链接无法点击，您也可以复制链接，粘贴到您浏览器的地址栏内，然后按“回车”键打开预设页面，完成密码的更改。"
            + "如果有其他问题，请联系我们：admin@%s<br />" + "<br />" + "谢谢！ <br />";

    private static String emailLink = "%s/verifyEmail.do?uid=%s&email=%s&key=%s&type=verifyPWD";

    @Autowired
    private UserPassportExtDAO userPassportExtDAO;

    @Override
    public boolean isExistEmail(final String email, final List<Account> account) {
        List<Account> account1;
        try {
            account1 = PassportAdapterFactory.getWapLogin().getAccount(email);
        } catch (final Exception e) {
            e.printStackTrace();
            return false;
        }
        if (account1 == null) {
            return false;
        } else {
            for (final Account a : account1) {
                account.add(a);
            }
            return true;
        }
    }

    @Override
    public void sendAutoLoginEmail(final String email, final String domain) {
        // get user id
        final List<Account> accounts = new ArrayList<Account>();
        if (!this.isExistEmail(email, accounts)) {
            return;
        }
        final int userid = this.getUserId(accounts, domain);
        if (-1 == userid) {
            return;
        }

        // send the email
        String name = "";
        try {
            final User user = WUserAdapter.getInstance().get(userid);
            name = user.getName();
        } catch (final Exception e) {
            e.printStackTrace();
        }
        final String link = this.getLink(userid, email, domain);
        String olink;
        try {
            olink = String.format("http://c.renren.com/c.jsp?d=%s&k=%s&u=%s", System
                    .currentTimeMillis(), UUID.randomUUID().toString(), URLEncoder.encode(link,
                    "UTF-8"));
        } catch (final Exception e1) {
            e1.printStackTrace();
            olink = link;
        }
        final String content = String.format(emailContent, name, olink, link, domain);
        final boolean isRRDomain = "renren.com".equals(domain);
        final String from = isRRDomain ? EmailConstants.RR_OFFICIAL_FROM
                : EmailConstants.KX_OFFICIAL_FROM;
        final String titleN = isRRDomain ? "人人网" : "开心网";
        final int disc = isRRDomain ? 31210040 : 31200040;
        try {
            ImmediateEmail.send(email, from, content, titleN + "登录确认", disc, domain);
        } catch (final SQLException e) {
            e.printStackTrace();
        } catch (final Exception e) {
            e.printStackTrace();
        }

    }

    /**
     * getLink<br>
     * 
     * @param userId
     * @param name
     * @return
     * 
     * @author tai.wang@opi-corp.com Mar 16, 2010 - 12:36:00 PM
     */
    private String getLink(final int userId, final String name, final String domain) {
        UserPassportExt m = new UserPassportExt();
        m.setId(userId);
        try {
            // UserPassportExtDAO userPassportExtDAO =
            // Globals.getRoseAppContext()
            // .getBean(UserPassportExtDAO.class);
            m = this.userPassportExtDAO.select(userId);
        } catch (final Exception e) {
            e.printStackTrace();
            return "http://www." + domain;
        }
        if (m != null) {
            final String key = m.getVerify_code();
            return String.format(emailLink, "http://safe." + domain, userId, name, key);
        } else {
            return "http://home." + domain;
        }
    }

    private int getUserId(final List<Account> accounts, final String domain) {
        if (accounts.size() == 1) {
            return accounts.get(0).getId();
        }
        for (final Account account : accounts) {
            if (StringUtils.equals(domain, account.getDomain())) {
                final int userid = account.getId();
                return userid;
            }
        }
        return -1;
    }

}

package com.xiaonei.platform.core.opt.base;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.dao.CoreConfigDAO;
import com.xiaonei.xce.passport.Account;

/**
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
public class UserConfirmLogic {

    private static UserConfirmLogic current;

    private static long timestampe;

    private int startUser;

    private int mod;

    private UserConfirmLogic(int startUser, int mod) {
        this.startUser = startUser;
        this.mod = mod;
    }

    public static UserConfirmLogic getInstance() {
        if (current == null || System.currentTimeMillis() - timestampe > 60 * 1000) {
            timestampe = System.currentTimeMillis();
            int mod = 10;
            int startUser = Integer.MAX_VALUE;
            try {
                String value = CoreConfigDAO.getInstance().getCoreConfigValue("user_confirm_mod");
                if (StringUtils.isNotBlank(value)) {
                    mod = Integer.parseInt(value);
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
            try {
                String value = CoreConfigDAO.getInstance().getCoreConfigValue(
                        "user_confirm_start_user");
                if (StringUtils.isNotBlank(value)) {
                    startUser = Integer.parseInt(value);
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
            current = new UserConfirmLogic(startUser, mod);
        }
        return current;
    }

    /**
     * 哪些人需要验证激活后置；true-代表需要激活后置；false=不需要激活后置
     * 
     * @param account
     * @see #iNeedYou(Account)
     * @return
     */
    public boolean iNeedYou(User user) {
        assert user.getAccount() != null;
        if (isIgnored(user.getAccount()) || isRegisterById(user.getAccount())) {
            return false;
        }
        return isNeeded(user.getId(), user.getAccount());
    }

    /**
     * 哪些人需要验证激活后置；true-代表需要激活后置；false=不需要激活后置
     * 
     * @param user
     * @see #iNeedYou(User)
     * @return
     */
    public boolean iNeedYou(Account user) {
        assert user.getAccount() != null;
        if (isIgnored(user.getAccount())) {
            return false;
        }
        return isNeeded(user.getId(), user.getAccount());
    }

    /**
     * 忽略这些帐号不用走激活后置
     * 
     * @param account
     * @return
     */
    private boolean isIgnored(String account) {
        // 其它需要忽略的return true的，继续这里写？
        return false;
    }

    private boolean isNeeded(int id, String account) {
        if (account.startsWith("jihuohouzhi")) {
            return true;
        }
        if (account.endsWith("@qq.com")) {
            return true;
        }
        if (mod < 0 || id < startUser) {
            return false;
        } else {
            return (mod == 0 || id % mod == 0);
        }
    }

    // 通过ID注册的?
    private boolean isRegisterById(String id) {
        //mobile?
        if (NumberUtils.isDigits(id)) {
            return false;
        }
        // email?
        if (id.indexOf('@') != -1) {
            return false;
        }
        return true;
    }

    @Deprecated
    public boolean isConfirmed(User user, HttpServletRequest request, HttpServletResponse response) {
        return isConfirmed(user);
    }

    /**
     * true: 已验证过的；<br>
     * false: 未验证的，现在需要去验证的
     * <p>
     * 
     * @param user
     * @param request
     * @param response
     * @return
     */
    public boolean isConfirmed(User user) {
        if (!iNeedYou(user)) {
            return true;
        }
        return (user.getStatus() == User.statusNormal);
    }

    public boolean isConfirmed(Account account) {
        if (!iNeedYou(account)) {
            return true;
        }
        return (account.getStatus() == User.statusNormal);
    }

    /**
     * 根据用户认证状态，设置或清除前端能够识别的cookie标识
     * 
     * @param request
     * @param response
     * @param confirmed
     */
    public static void saveOrClearNoconfirmCookie(HttpServletRequest request,
            HttpServletResponse response, boolean confirmed) {
        if (confirmed) {
            if (CookieManager.getInstance().getCookie(request, "noconfirm") != null) {
                CookieManager.getInstance().clearCookie(response, "noconfirm", 1, "/",
                        "." + OpiConstants.domainMain);
            }
        } else {
            CookieManager.getInstance().saveCookie(response, "noconfirm", "1", -1, "/",
                    "." + OpiConstants.domainMain);
        }
    }

    /**
     * 根据用户认证状态，设置或清除前端能够识别的cookie标识
     * 
     * @param request
     * @param response
     * @param user
     * @return true 已认证；false-为认证的
     */
    public boolean saveOrClearNoconfirmCookie(User user, HttpServletRequest request,
            HttpServletResponse response) {
        if (isConfirmed(user)) {
            if (CookieManager.getInstance().getCookie(request, "noconfirm") != null) {
                CookieManager.getInstance().clearCookie(response, "noconfirm", 1, "/",
                        "." + OpiConstants.domainMain);
            }
            return true;
        } else {
            CookieManager.getInstance().saveCookie(response, "noconfirm", "1", -1, "/",
                    "." + OpiConstants.domainMain);
            return false;
        }
    }
}

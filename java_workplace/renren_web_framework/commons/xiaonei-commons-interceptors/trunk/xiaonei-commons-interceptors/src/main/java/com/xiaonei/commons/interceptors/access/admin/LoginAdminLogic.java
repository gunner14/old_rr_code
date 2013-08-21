package com.xiaonei.commons.interceptors.access.admin;

import java.util.Random;

import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

/**
 * 
 * @author xingtao.shi@opi-corp.com
 * 
 */
public class LoginAdminLogic {

    private static Log logger = LogFactory.getLog(LoginAdminLogic.class);

    private static final String CACHE_ADMIN_COOKIE_TICKET = "admin_cookie_ticket";

    public static final String ADMIN_COOKIE = "admin";

    public static void resetCookie(HttpServletResponse response, int userId) {
        Random rand = new Random();
        MemCacheManager mgr = MemCacheFactory.getInstance().getManagerByPoolName(
                MemCacheKeys.pool_default);
        Object obj = mgr.get(CACHE_ADMIN_COOKIE_TICKET + userId);
        String number;
        if (obj == null) number = String.valueOf(rand.nextInt(1000000));
        else number = (String) obj;
        mgr.asyncSet(CACHE_ADMIN_COOKIE_TICKET + userId, number, 3600);
        //CookieManager.getInstance().saveCookie(response, LoginAdminLogic.ADMIN_COOKIE, number);
    }

    public static void removeCookie(int userId) {
        MemCacheManager mgr = MemCacheFactory.getInstance().getManagerByPoolName(
                MemCacheKeys.pool_default);
        mgr.delete(CACHE_ADMIN_COOKIE_TICKET + userId);
    }

    public static boolean isCookieRight(int userId) {
        try {
            MemCacheManager mgr = MemCacheFactory.getInstance().getManagerByPoolName(
                    MemCacheKeys.pool_default);
            Object ticket = mgr.get(CACHE_ADMIN_COOKIE_TICKET + userId);
            return !(ticket == null);
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
        }
        return false;
    }

}

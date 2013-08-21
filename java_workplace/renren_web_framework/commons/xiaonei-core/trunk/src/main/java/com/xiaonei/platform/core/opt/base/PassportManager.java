package com.xiaonei.platform.core.opt.base;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.opt.MD5;
import com.xiaonei.xce.passport.PassportAdapterFactory;

/**
 * passport 管理类
 * 
 * @author zhangjie@opi-corp.com
 */
public class PassportManager {

    public static final String loginKey_old_ticket = "societyguester";

    public static final String loginKey_ticket = "t";

    public static final String loginKey_mticket = "mt";

    private static PassportManager instance = new PassportManager();

    public static PassportManager getInstance() {
        return instance;
    }

    /**
     * 清除用户登录标识
     * 
     * @param request
     * @param response
     */
    public void clearHostId(HttpServletRequest request, HttpServletResponse response) {
        String ticket = this.getCookie(request, loginKey_ticket);
        this.clearCookie(request, response, loginKey_ticket);
        if (ticket != null) PassportAdapterFactory.getWebTicket().destroyTicket(ticket);
    }

    /**
     * 根据登录标识获取登陆者的Id
     * 
     * @param request
     * @return
     */
    public Integer getHostIdFromCookie(HttpServletRequest request, HttpServletResponse response) {
        return getHostIdFromCookie(request, response, loginKey_ticket);
    }

    public Integer getHostIdFromCookie(HttpServletRequest request, HttpServletResponse response,
            String cookieName) {
        String ticket = this.getCookie(request, cookieName);
        return getHostIdFromIce(request, ticket);
    }

    public Integer getHostIdFromCookieForWap(HttpServletRequest request,
            HttpServletResponse response, String cookieName) {
        String ticket = this.getCookie(request, cookieName);
        return getHostIdFromWap(request, ticket);
    }

    /**
     * 从cookie中清除T票
     * @param response
     */
    public void clearTicketFromCookie(HttpServletResponse response) {
    	CookieManager.getInstance().clearCookie(response, loginKey_ticket, 0, "/");
    }
    
    /**
     * 从request参数中取票并验票。
     * 
     * @author Li Weibo
     * @since 2009-02-06
     * 
     * @param request
     * @param paramName 参数名
     * @return
     */
    public Integer getHostIdFromRequestParam(HttpServletRequest request, String paramName) {
        String ticket = request.getParameter(paramName); // 从request参数中取票
        return getHostIdFromIce(request, ticket);
    }

    public Integer getHostIdFromRequestParamForWap(HttpServletRequest request, String paramName) {
        String ticket = request.getParameter(paramName); // 从request参数中取票
        return getHostIdFromWap(request, ticket);
    }

    public String createTicket(int userId, HttpServletRequest request) {
        return PassportAdapterFactory.getWebTicket().createTicket(userId,
                getTicketProfiler(request));
    }

    public Map<String, String> getTicketProfiler(HttpServletRequest request) {
        Map<String, String> profiler = new HashMap<String, String>();
        profiler.put("User-Agent", request.getHeader("User-Agent"));
        profiler.put("IP", request.getRemoteAddr());
        return profiler;
    }

    private Integer getHostIdFromIce(HttpServletRequest request, String ticket) {
        if (ticket == null || ticket.equals("")) return null;
        Integer id = PassportAdapterFactory.getWebTicket().verifyTicket(ticket,
                getTicketProfiler(request));
        return id == null || id.intValue() < 0 ? null : id;
    }

    private Integer getHostIdFromWap(HttpServletRequest request, String ticket) {
        if (ticket == null || ticket.equals("")) return null;
        Integer id = PassportAdapterFactory.getWapLogin().ticket2Id(ticket);
        return id == null || id.intValue() < 0 ? null : id;
    }

    public String getCookie(HttpServletRequest request, String key) {
        return CookieManager.getInstance().getCookie(request, key);
    }

    public void clearCookie(HttpServletRequest request, HttpServletResponse response, String name) {
        CookieManager.getInstance().clearCookie(response, name, 0, "/");
    }


    
    /**
     * 加密T票的加密的算法是userId + ENCRYPT_SEP + md5(userId + ticket)
     * ENCRYPT_SEP就是那个分隔符
     */
    public static final String ENCRYPT_SEP = "-";
    
    /**
     * 由于上传业务等使用的flash来访问web，在这种情况下cookie不能正确地呆过来，
     * 所以会使用参数来传递T票。但是如果使用明文来传输T票的话可能会有安全隐患，
     * 所以在此类需求下需要对T票进行加密再传输。
     * 
     * 具体的加密方法请参见encryptTicket(int userId, String ticket)方法
     * 
     * @param userId
     * @return
     * 
     * @author weibo.li@opi-corp.com
     */
    public String getEncryptedTicket(int userId) {
    	String ticket = PassportAdapterFactory.getWebTicket().queryTicket(userId);
    	if (ticket == null) {
    		return null;
    	}
    	return encryptTicket(userId, ticket);
    }
    
    /**
     * 
     * 加密的算法是userId + ENCRYPT_SEP + md5(userId + ticket)
     * 
     * @param userId
     * @param ticket
     * @return
     */
    public String encryptTicket(int userId, String ticket) {
    	return userId + ENCRYPT_SEP + MD5.digest(userId + ticket);
    }
    
    /**
     * 对getEncryptedTicket方法中描述的加密T票进行验证
     * 
     * @param userId
     * @param encryptedTicket
     * @return 验证通过返回用户ID，否则返回-1
     * 
     * @author weibo.li@opi-corp.com
     */
    public int verifyEncryptedTicket(String encryptedTicket) {
    	String[] ss = encryptedTicket.split(ENCRYPT_SEP);
    	if (ss.length != 2) {
    		return -1;
    	}
    	try {
			int userId = Integer.parseInt(ss[0]);	//分离出userId
			String rightEncryptedTicket = getEncryptedTicket(userId);	//按此userId取一遍加密T票
	    	if( encryptedTicket != null && encryptedTicket.equals(rightEncryptedTicket)) {//比较之
	    		return userId;
	    	} else {
	    		return -1;
	    	}
		} catch (NumberFormatException e) {
			return -1;
		}
    }
    
}

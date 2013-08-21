package com.xiaonei.reg.register.logic.additional;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.MD5Util;
import com.xiaonei.reg.common.RegUtil;

/**
 * 注册免激活
 * @author wei.cheng@opi-corp.com
 * @date
 */
public class RegNoActivateLogic {
    
    private static RegNoActivateLogic _instance = new RegNoActivateLogic();

    public static RegNoActivateLogic getInstance() {
        return _instance;
    }

    private RegNoActivateLogic() {
        super();
    }
    
    public boolean isNoActivatedEmail(User user) {
        if(user == null || StringUtils.isEmpty(user.getAccount()))
            return false;
        return user.getAccount().toLowerCase().endsWith("@qq.com") 
                && (user.getId() % 10 == 2 || user.getId() % 10 == 4 || user.getId() % 10 == 6);
    }
 
    /**
     * 用户合法性的判断
     * @param user
     * @param req
     * @return
     */
    public boolean isValidMsnUserPost(User user, HttpServletRequest req){
    	if("205234".equals(StringUtils.trimToEmpty(req.getParameter("action_id")))
    			|| "205235".equals(StringUtils.trimToEmpty(req.getParameter("action_id")))){
    		if(isValidMsnUserGet(req)){
    			return true;
    		}
    	}
    	return false;
    }
    
    /**
     * 令牌校验
     * @param req
     * @return
     */
    public boolean isValidMsnUserGet(HttpServletRequest req){
    	String token = StringUtils.trimToEmpty(req.getParameter("m"));
    	String email = StringUtils.trimToEmpty(req.getParameter("email"));
    	if(StringUtils.isEmpty(email)){
    		email = StringUtils.trimToEmpty(req.getParameter("regEmail"));
    	}
    	if(MD5Util.md5(RegUtil.getEncryptString(email)).equals(token)){
    		return true;
    	}
    	return false;
    }
    
    /**
     * 是否发送激活信的逻辑
     * @param user
     * @return
     * @author wei.cheng@opi-corp.com
     * @version 1.0
     * @date 创建时间：Mar 16, 2010 4:09:53 PM
     */
    public boolean isNoActivatedSendEmail(User user) {
        if(user == null || StringUtils.isEmpty(user.getAccount()))
            return true;
        return false;
    }
    
    public boolean jumpActivatedPage(User user) {
		return false;
	}
}

package com.xiaonei.reg.register.controllers.ajax;

import java.util.Arrays;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.usertrace.q.UTUtils;

/**
 * 试用于分2步骤注册的页面，第一次点击按钮的事件记录 2010-01-08
 * 
 * @author wei.cheng@opi-corp.com
 */

@Path({ "AjaxRegLog" })
public class AjaxSearchRegLogController {

    private RegLogger logger = RegLogger.getLoggerWithType(this.getClass().getSimpleName(),UTUtils.authorEmail);
    
	@SuppressWarnings("unchecked")
    @Get
	@Post
	public String executeBt(Invocation invocation) {

		HttpServletRequest request = invocation.getRequest();
		String type = request.getParameter("tp");
		String input = request.getParameter("input");
		
		if(StringUtils.contains(type, "search")){
		    //搜人注册流程
		    logger.info("[SearchFriendReg],input=" +input + ",ip=" + BizFilterUtil.getClientIP(request) + ",tp=" + type + ", referer=" + request.getHeader("Referer"));
		}else if(StringUtils.contains(type, "login")){
		    //登录框统计
		    this.printObj(request.getParameterMap());
		    logger.info("[login stat],email=" + input + ",ip=" + BizFilterUtil.getClientIP(request)+ ",tp=" + type +", referer=" + request.getHeader("Referer"));
		}
		return null;
	}
	
	private void printObj(Map<String, String> map) {
        if (map != null) {
            for (java.util.Map.Entry<String, String> b : map.entrySet()) {
                String k = (String)b.getKey();
                String val = "";
                Object obj = b.getValue();
                if(obj instanceof String[]){
                    val = Arrays.toString((String[])obj);
                }else{
                    val = (String)obj;
                }
                RegLogger.getLoggerWithType(this.getClass()).debug(
                        "key: " + k + ",value: " + val);
            }
        }
    }
}

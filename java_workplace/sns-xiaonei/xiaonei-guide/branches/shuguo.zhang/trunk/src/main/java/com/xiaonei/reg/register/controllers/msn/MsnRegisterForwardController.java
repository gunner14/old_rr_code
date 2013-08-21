package com.xiaonei.reg.register.controllers.msn;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.reg.common.RegLogger;


/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date May 11, 2010 3:53:26 PM
 * MSN合作。注册分支判断页面
 */

@Path("msnflow")
public class MsnRegisterForwardController{

    private RegLogger mslog = RegLogger.getLoggerWithType(this.getClass());

    @Get
	@Post
	public String index(Invocation inv){
    	mslog.debug("begin into msn flow page .....");
        
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
        
        if(checkLogin(request, response)){
        	return "r:http://www.renren.com/Home.do";
        }else{
        	return "r:http://reg.renren.com/xn5234.do";
        }
    }

    /**
     * 判断用户是否已经登录
     * @param request
     * @param response
     * @return
     */
    private boolean checkLogin(HttpServletRequest request, HttpServletResponse response){
    	//TODO
    	return false;
    }
}

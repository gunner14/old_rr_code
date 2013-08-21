package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserSortLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.xce.socialgraph.sortbycluster.Offer;

@LoginRequired
@Path({"previewfriendsort"})
@IgnoreUserStatusImportedValidation
public class PreviewInitAddFriendSortController {
	@Autowired
	GuideXFiveUserSortLogic guideXFiveUserSortLogic;
	
	@Get
    @Post
    public String get(Invocation inv){
        HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
        HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
        User host = GuideXFiveRequestUtil.getCurrentHost(request);      
        try {
			if(request == null || response == null){
			    GuideLogger.printLog(" request or response null "+(request==null)+(response==null), GuideLogger.ERROR);
			    return null;
			} else if(host == null){
			    GuideLogger.printLog(" host null ", GuideLogger.ERROR);
			    return null;
			} else{
				Map<String, String> userInfoMap = getParamMap(inv);
//				GuideLogger.printLog(">>>>>>>>GuideXFiveUserSortLogic userInfoMap:" + userInfoMap.get("p") + " | " +userInfoMap.get("uid") + "<<<<<<<<");
				List<Offer> offerList = GuideUtil.getOfferFriendFinalRecommendByCluster(userInfoMap, 50);
//				GuideLogger.printLog(">>>>>>>>GuideXFiveUserSortLogic offerList size:" + offerList.size() + "<<<<<<<<");
				String resultBody = GuideUtil.getJsonForOfferFriend(offerList);
//				GuideLogger.printLog(">>>>>>>>GuideXFiveUserSortLogic resultBody:" + resultBody + "<<<<<<<<");
			    //返回ajax json
			    writeAjaxStream(response, resultBody);
			    return null;
			}
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
    }
    
    /**
     * 
     * TODO       封装请求参数到map中
     *	
     * @author	Kobin (bin.huang1@renren-inc.com)
     * @since		2012-7-11 下午6:52:07
     * @param    inv
     * @return
     */
    private Map<String, String> getParamMap(Invocation inv){
    	HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
    	
    	String ajax = request.getParameter("ajax");
		String p = request.getParameter("p");
		String s = request.getParameter("s");
		String u = request.getParameter("u");
		String uid = request.getParameter("uid");
		String us = request.getParameter("us");
		
		Map<String, String> paramMap = new HashMap<String, String>();
		
		paramMap.put("ajax", ajax);
		paramMap.put("p", p);
		paramMap.put("s", s);
		paramMap.put("u", u);
		paramMap.put("uid", uid);
		paramMap.put("us", us);
		return paramMap;
    }
    
    /**
     * ajax返回json串
     * @param response
     * @param str
     */
    private void writeAjaxStream(final HttpServletResponse response, final String str) {
        try {
            response.setContentType("text/html; charset=UTF-8");
            PrintWriter writer = response.getWriter();
            writer.print(str);
            writer.flush();
            writer.close();
        } catch (IOException e) {
        	e.printStackTrace();
        }
    }
}

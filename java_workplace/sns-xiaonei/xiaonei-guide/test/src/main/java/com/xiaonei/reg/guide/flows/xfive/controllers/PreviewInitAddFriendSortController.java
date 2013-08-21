package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.httpclient.DefaultHttpMethodRetryHandler;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.params.HttpMethodParams;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUserSortLogic;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.util.GuideLogger;

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
        if(request == null || response == null){
            GuideLogger.printLog(" request or response null "+(request==null)+(response==null), GuideLogger.ERROR);
            return null;
        } else if(host == null){
            GuideLogger.printLog(" host null ", GuideLogger.ERROR);
            return null;
        } else{
    		//构造HttpClient的实例
    		HttpClient httpClient = new HttpClient();
    		//创建GET方法的实例
    		String url = "http://browse.renren.com/searchgd?";
    		
    		//转UTF-8编码
    		String qureyString = "";
    		try {
	    		String ajax = URLEncoder.encode(request.getParameter("ajax"), "utf-8");
	    		String p = URLEncoder.encode(request.getParameter("p"), "utf-8");
	    		String s = URLEncoder.encode(request.getParameter("s"), "utf-8");
	    		String u = URLEncoder.encode(request.getParameter("u"), "utf-8");
	    		String uid = URLEncoder.encode(request.getParameter("uid"), "utf-8");
	    		String us = URLEncoder.encode(request.getParameter("us"), "utf-8");
	    		qureyString = "ajax=" + ajax + "&p=" + p + "&s=" + s + "&u=" + u + "&uid=" + uid + "&us=" + us;
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
    		
    		GetMethod getMethod = new GetMethod(url + qureyString);
            GuideLogger.printLog(">>>>>>>>>>>>>>previewfriendsort httpClient url=" + url + qureyString , GuideLogger.INFO);
            
            //使用系统提供的默认的恢复策略
            getMethod.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,new DefaultHttpMethodRetryHandler());
            
            try {
    			int status = httpClient.executeMethod(getMethod);
    			if(status != HttpStatus.SC_OK){
    				GuideLogger.printLog(">>>>>>>>>>>>>>previewfriendsort httpClient.executeMethod(getMethod) status<<<<<<<<<<<<<<" + status, GuideLogger.ERROR);
    			}
    			if (status == HttpStatus.SC_OK) {
//    				GuideLogger.printLog(">>>>>>>>>>>>>>getResponseBodyAsStream<<<<<<<<<<<<<<" + status, GuideLogger.ERROR);
    				InputStream resStream = getMethod.getResponseBodyAsStream();  
    				BufferedReader br = new BufferedReader(new InputStreamReader(resStream));  
                    StringBuffer resBuffer = new StringBuffer();  
                    String resTemp = "";  
                    while((resTemp = br.readLine()) != null){  
                        resBuffer.append(resTemp);  
                    }  
                    
                    String resultBody = resBuffer.toString(); 
                    
//                  GuideLogger.printLog("Httpclient GetMethod ResponseBody:" + resultBody, GuideLogger.ERROR);
                    
                    /*
                     * kobin  2012/05/25
                     * 添加用户价值排序逻辑
                     * 开放尾号为9的用户(前端JS控制尾号)
                     */
                    long sortStartTime = System.currentTimeMillis();
                    resultBody = guideXFiveUserSortLogic.sortJsonArrayByUserMark(resultBody.trim());
                    long sortEndTime = System.currentTimeMillis();
                    GuideLogger.printLog(">>>>>>>>GuideXFiveUserSortLogic cost time:" + (sortEndTime - sortStartTime) + "<<<<<<<<");
                    
//                  GuideLogger.printLog("Sort ResponseBody return resultBody:" + resultBody, GuideLogger.ERROR);
                    
                    //返回ajax json
                    writeAjaxStream(response, resultBody);
                    
    				return null;
    			}
    			return null;
    		} catch (Exception e1) {
    			e1.printStackTrace();
    			return null;
    		} finally { 
    			//释放连接 
    			getMethod.releaseConnection(); 
            } 
        }
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

package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.renren.sns.tag.view.MinisiteCategory;
import com.renren.sns.tag.view.render.SuggestSiteRender;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * FlowRecommendInterestController.java
 * @author 董刚锋(gangfeng.dong@renren-inc.com)
 * 2012-03-12
 */
@LoginRequired
@Path({"flowrecommendinterest"})
@IgnoreUserStatusImportedValidation
public class FlowRecInterestZ0Controller {
    
   
    private static final String SUCC_DEFAULT = "/pages/guide/guiderecommendflow/guiderecommend.jsp";
    
    @Autowired
    com.renren.sns.tag.service.ZhanService zhanService;
    
    @Get
    @Post
    //此方法返回兴趣页面。
    public String get(Invocation inv){
        
        
        HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
        HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
        User host = GuideXFiveRequestUtil.getCurrentHost(request); 
        try {
            if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
                GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
                return "r:http://reg.renren.com/sb/fillemail";
            }
            List<MinisiteCategory> minisiteList=zhanService.getCategories();
            int minisiteListSize=minisiteList.size();
            List<MinisiteCategory> showMinisiteList=new ArrayList<MinisiteCategory>();//返回到客户端的list
            Random random=new Random();
            MinisiteCategory minisiteCategory=null;
            if(minisiteListSize>11){
            	
            	 for(int i=0;i<11;i++){
            		 minisiteCategory=minisiteList.remove(random.nextInt(minisiteListSize));
            		 showMinisiteList.add(minisiteCategory);
            		 minisiteListSize--;
                 }
            }
            else{
            	showMinisiteList=minisiteList;
            }
            int slectNumber=random.nextInt(showMinisiteList.size());
            minisiteCategory=showMinisiteList.get(slectNumber);
            //返回到客户端的小站集合
            List<SuggestSiteRender> suggestSiteList= zhanService.
            		getSuggestSite(minisiteCategory.gettagId(), host.getId());

            request.setAttribute("showminisitelist", showMinisiteList);
            request.setAttribute("suggestsitelist", suggestSiteList);
            request.setAttribute("chooseminisite", minisiteCategory);
            request.setAttribute("slectnumber", slectNumber);
            
           
            
        } catch (Exception e) {
            GuideLogger.printLog("setInitAttributes failed! err "+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        return SUCC_DEFAULT;
    }
    
    
    //获取某一兴趣下小站内容接口
    @Post("gettagminisite")
    @Get("gettagminisite")
    public String getTagMinisite(Invocation inv) {
    	
		HttpServletRequest request = inv.getRequest();
		String tagId = request.getParameter("tagid");// 统计操作的位置
		if(tagId==null || "".equals(tagId) || (tagId.matches("[0-9]+")==false)){
            GuideLogger.printLog("tagId is not correct:tagId:"+tagId);
            return "@tagId is not correct tagid:"+tagId;
        }
    	try {
			User host = GuideXFiveRequestUtil.getCurrentHost(request);
			List<SuggestSiteRender> suggestSiteList= zhanService.
            		getSuggestSite(new Integer(tagId), host.getId());
			
			request.setAttribute("subsuggestsitelist", suggestSiteList);
			return "/pages/guide/guiderecommendflow/subguiderecommend.jsp";
			
		} catch (Exception e) {
			e.printStackTrace();
		}
    	GuideLogger.printLog("server happen");
        return "@server happen";
    }
    
    //这个方法不知道为什么调用不到，已换成FlowRecommendModifyStepController这个控制器来处理
    @Post("gettagminisitee")
    @Get("gettagminisitee")
    public String getTagMinisitee(Invocation inv) {
    	 User host = GuideXFiveRequestUtil.getCurrentHost(inv.getRequest()); 
    	 try {
				//改标志
				StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FlOW_RECOMMEND);
				GuideLogger.printLog("| host:"+host.getId()+" update step:"+StepStatusLogic.STEP_FlOW_RECOMMEND);
			} catch (Exception e) {
				e.printStackTrace();
			}
    	 return "f:lead";
    }
}

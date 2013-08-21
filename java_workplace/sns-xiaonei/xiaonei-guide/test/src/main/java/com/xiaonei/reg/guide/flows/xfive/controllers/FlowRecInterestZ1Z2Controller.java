package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.renren.sns.guide.service.GuideRecommendLogic;
import com.renren.sns.tag.view.MinisiteCategory;
import com.renren.sns.tag.view.render.SuggestSiteRender;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.pojo.RecommendContent;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * FlowRecommendInterestController.java
 * @author 董刚锋(gangfeng.dong@renren-inc.com)
 * 2012-03-12
 */
@LoginRequired
@Path({"flowrecommend5and6interest"})
@IgnoreUserStatusImportedValidation
public class FlowRecInterestZ1Z2Controller {
	
	@Autowired
	private  GuideRecommendLogic guideRecommendLogic;
    
   
    private static final String SUCC_DEFAULT = "/pages/guide/guiderecommendflow/guideweihao56recommend.jsp";
    
   
    @Get
    @Post
    //此方法返回兴趣页面。
    public String get(Invocation inv){
    	
    	if(guideRecommendLogic.getGuideRecommendContentList()==null){
    		guideRecommendLogic.loadData();
        }
    	
       
    	
        
        HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
        User host = GuideXFiveRequestUtil.getCurrentHost(request); 
        if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
            GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
            return "r:http://reg.renren.com/sb/fillemail";
        }
        Map<String,String> inserestMap=new HashMap<String, String>();
        Map<String,List<RecommendContent>> sortGuideRecommendMap=guideRecommendLogic.getSortGuideRecommendMap();
        
    	List<RecommendContent> recommentList=null;
    	String interest="";
    	Iterator<Entry<String, List<RecommendContent>>> ite=sortGuideRecommendMap.entrySet().iterator();
    	Entry<String, List<RecommendContent>> entry=null;
    	int size=0;
    	StringBuilder sbf=new StringBuilder();
    	 while(ite.hasNext()){
    		 entry =  ite.next(); 
    		 interest=entry.getKey();
    		 recommentList=entry.getValue();
    		 size=recommentList.size();
    		 for(int i=0;i<size;i++){
    			 if(i==size-1){
    				 sbf.append("site=");sbf.append(recommentList.get(i).getNickname());
    			 }
    			 else{
    				 sbf.append("site=");sbf.append(recommentList.get(i).getNickname());sbf.append("&");
    			 }
    			
    		 }
    		 inserestMap.put(interest, sbf.toString());
    		 sbf.delete(0, sbf.length());
             
         }
        inv.getRequest().setAttribute("guiderecommendnickname",inserestMap);
        return SUCC_DEFAULT;
    }
}

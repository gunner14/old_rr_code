package com.renren.sns.guide.controllers;


import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import com.renren.sns.guide.service.GuideRecommendLogic;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.usertrace.dao.GuideRecommendJadeDAO;
import com.xiaonei.xce.feed.FeedLogicAdapter;
import com.xiaonei.platform.core.model.User;

@LoginRequired
@Path({ "guiderecommendstatistics" })
@IgnoreUserStatusImportedValidation
public class GuideRecFllowClickController {
    
	@Autowired
	private  GuideRecommendLogic guideRecommendLogic;

    
    //@Autowired
    //com.renren.sns.tag.service.ZhanService zhanService;

    @Get
    public String get(Invocation inv) {
        return index(inv);
    }
    //guide首页推荐位点击和关注需要调用的方法
    @Post
    public String index(Invocation inv) {
        
        HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
    
        User host = GuideXFiveRequestUtil.getCurrentHost(request);
        int userid=host.getId();
        
        String functionType=request.getParameter("functiontype");//获取功能类型分为两种：点击、关注
        if(functionType==null || "".equals(functionType)){
            return "@functiontype is null";
        }
        String from =request.getParameter("from");//统计操作的位置
        if(StringUtils.isBlank(from)){
        	from ="guide_home";
        }
        //如果是点击
        if("click".equals(functionType)){
            String id=inv.getRequest().getParameter("id");//获取推荐内容id 此id是流水id
            GuideLogger.printLog("id:"+id);
            if(id==null || "".equals(id) || (id.matches("[0-9]+")==false)){
                GuideLogger.printLog("id is not correct:id:"+id);
                return "@";
            }
            GuideLogger.printLog("guiderecommendclick userid:"+userid+" id:"+id);
            try {
            	//guideRecommendLogic.insertGuideRecommendCountViewLog(userid, 2, Integer.parseInt(id),from,"");
                //guideRecommendJadeDAO.insertGuideRecommendCountViewLog(userid, 2, Integer.parseInt(id),from,"");
                if(from.equals("guide_home")){
                	guideRecommendLogic.updateGuideRecommendhits(Integer.parseInt(id));
                }
                
            } catch (NumberFormatException e) {
                e.printStackTrace();
            } 
            return"@";
        
        }
        
        
        //如果是关注
        else if("flow".equals(functionType)){
            JSONObject rtnJson = new JSONObject();
            String id=inv.getRequest().getParameter("id");//获取推荐内容id 此id是流水id
            GuideLogger.printLog("id:"+id);
            if(id==null || "".equals(id) || (id.matches("[0-9]+")==false)){
                rtnJson.put("code", "1");
                rtnJson.put("message", "id is not correct");
                GuideLogger.printLog("参数值为空 id:"+id);
                return"@"+rtnJson.toString();
            }
            
            String businessid=inv.getRequest().getParameter("businessid");//businessid是推荐内容的id
            GuideLogger.printLog("businessid:"+businessid);
            if(businessid==null || "".equals(businessid) || (businessid.matches("[0-9]+")==false)){
                rtnJson.put("code", "1");
                rtnJson.put("message", "businessid is not correct");
                GuideLogger.printLog("参数值为空 businessid:"+businessid);
                return"@"+rtnJson.toString();
            }
            
            boolean isAttention=guideRecommendLogic.attentionXiaoZhao
                    (Integer.parseInt(businessid), userid);
            if(isAttention){
                try {
                	guideRecommendLogic.insertGuideRecommendCountViewLog(userid, 3, Integer.parseInt(id),from,"");
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                } 
                
                rtnJson.put("code", "0");
                rtnJson.put("message", "success");
                GuideLogger.printLog("success userid:"+userid+" id:"+id);
                try {
                   // zhanService.sendSiteFeedToUser(Integer.parseInt(businessid),userid,5);
                	guideRecommendLogic.sendFeed(Integer.parseInt(businessid), userid);
                    //FeedLogicAdapter.getInstance().addMiniSiteFeed(Integer.parseInt(businessid),userid,30);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }
                return"@"+rtnJson.toString();
            }
            else{
                rtnJson.put("code", "2");
                rtnJson.put("message", "false");
                GuideLogger.printLog("关注失败 userid:"+userid+" id:"+id);
                return"@"+rtnJson.toString();
            }
            
        }
        
        else{
            GuideLogger.printLog("parameter error");
            return "@parameter error";
        }
    }
    
    //尾号是3和4流程推荐页面关注全部调用的接口，批量关注由前端调直接调用小站的接口，此方法只做了统计和发feed
    
    @Post("followtotal")
    @Get("followtotal")
    public String followTotal(Invocation inv) {
    	
		HttpServletRequest request = inv.getRequest();
		String from = request.getParameter("from");// 统计操作的位置
		if (StringUtils.isBlank(from)) {
			GuideLogger.printLog("from is null");
			return "@from is null";
		}
		
		String tagName = request.getParameter("tagname");// 统计操作的位置
		if (StringUtils.isBlank(tagName)) {
			GuideLogger.printLog("tagName is null");
			return "@tagName is null";
		}
		String businessIds="";
    	try {
			User host = GuideXFiveRequestUtil.getCurrentHost(request);
			int userid=host.getId();
			businessIds=request.getParameter("businessids");
			if(businessIds==null || businessIds.length()<=0){
				return "@businessids is null";
			}
			
			String [] ids=businessIds.split(":");
			int size =ids.length;
			List<Integer>  recommendContentIds=new ArrayList<Integer>();
			for(int i=0;i<size;i++){
				recommendContentIds.add(Integer.parseInt(ids[i]));
				/*GuideRecommendLogic.getInstance().insertGuideRecommendCountViewLog(
						userid, 3, Integer.parseInt(ids[i]),from,tagName);
				try {
					//zhanService.sendSiteFeedToUser(Integer.parseInt(ids[i]),userid,5);
					GuideRecommendLogic.getInstance().sendFeed(Integer.parseInt(ids[i]), userid);
					//FeedLogicAdapter.getInstance().addMiniSiteFeed(Integer.parseInt(ids[i]),userid,30);
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}*/
			}
			
			guideRecommendLogic.insertGuideRecommendCountViewLog(
					userid, 3, recommendContentIds,from,tagName);
			guideRecommendLogic.sendFeed(recommendContentIds, userid);
		} catch (Exception e) {
			GuideLogger.printLog("businessIds:"+businessIds);
			e.printStackTrace();
		}
        return "@success";
    }
    
  //尾号是3和4流程推荐页面单个关注调用的接口。
    @Post("followsingle")
    @Get("followsingle")
    public String followSingle(Invocation inv) {
    	
		HttpServletRequest request = inv.getRequest();
		JSONObject rtnJson = new JSONObject();
		String from = request.getParameter("from");// 统计操作的位置
		if (StringUtils.isBlank(from)) {
			rtnJson.put("code", "1");
            rtnJson.put("message", "from is null");
			GuideLogger.printLog("from is null");
			return"@"+rtnJson.toString();
		}
		
		String tagName = request.getParameter("tagname");// 统计操作的位置
		if (StringUtils.isBlank(tagName)) {
			rtnJson.put("code", "1");
            rtnJson.put("message", "tagName is null");
			GuideLogger.printLog("tagName is null");
			return"@"+rtnJson.toString();
		}
		
		User host = GuideXFiveRequestUtil.getCurrentHost(request);
		int userid=host.getId();
		
    	try {
            
            String businessid=inv.getRequest().getParameter("businessid");//businessid是推荐内容的id
            GuideLogger.printLog("businessid:"+businessid);
            if(businessid==null || "".equals(businessid) || (businessid.matches("[0-9]+")==false)){
                rtnJson.put("code", "1");
                rtnJson.put("message", "businessid is not correct");
                GuideLogger.printLog("参数值为空 businessid:"+businessid);
                return"@"+rtnJson.toString();
            }
            
            boolean isAttention=guideRecommendLogic.attentionXiaoZhao
                    (Integer.parseInt(businessid), userid);
            if(isAttention){
                try {
                	guideRecommendLogic.insertGuideRecommendCountViewLog(userid, 3, Integer.parseInt(businessid),from,tagName);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }
                
                rtnJson.put("code", "0");
                rtnJson.put("message", "success");
                GuideLogger.printLog("success userid:"+userid+" businessid:"+businessid);
                try {
                    //zhanService.sendSiteFeedToUser(Integer.parseInt(businessid),userid,5);
                	guideRecommendLogic.sendFeed(Integer.parseInt(businessid), userid);
                    //FeedLogicAdapter.getInstance().addMiniSiteFeed(Integer.parseInt(businessid),userid,30);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }
                
            }
            else{
                rtnJson.put("code", "1");
                rtnJson.put("message", "fllow failure");
                GuideLogger.printLog("关注失败 userid:"+userid+" businessid:"+businessid);
                return"@"+rtnJson.toString();
            }
            
        } catch (Exception e) {
			e.printStackTrace();
		}
    	return"@"+rtnJson.toString();
    }
}

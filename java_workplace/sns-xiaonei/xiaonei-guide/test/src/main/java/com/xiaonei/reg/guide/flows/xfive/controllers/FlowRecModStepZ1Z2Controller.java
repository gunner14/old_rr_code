package com.xiaonei.reg.guide.flows.xfive.controllers;

import java.util.HashMap;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import com.renren.sns.guide.service.GuideRecommendLogic;
import com.renren.sns.tag.view.render.SuggestSiteRender;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.pojo.RecommendContent;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.usertrace.dao.GuideRecommendJadeDAO;
import com.xiaonei.xce.feed.FeedLogicAdapter;

/**
 * FlowRecommendInterestController.java
 * @author 董刚锋(gangfeng.dong@renren-inc.com)
 * 2012-03-12
 */
@LoginRequired
@Path({"flowrecmodify5and6step"})
@IgnoreUserStatusImportedValidation
public class FlowRecModStepZ1Z2Controller {
	
	@Autowired
	private  GuideRecommendLogic guideRecommendLogic;

	
	/*public static HashMap<String,int []> idMap=new HashMap<String,int []>();
	
	static{
		idMap.put("音乐",new int []{671000843,671002289,671003329,671003997,671006803,671065917,671107677,671192850});
		idMap.put("电影",new int []{671000128,671005883,671021081,671086910,671121710,671139273});
		idMap.put("创意",new int []{671000108,671007490,671053298,671059768,671086594,671095914,671129039});
		idMap.put("旅行",new int []{671000125,671003682,671003790,671009169,671022567,671036265,671163370});
		idMap.put("摄影",new int []{671003371,671004023,671012216,671014716,671019806,671091412,671096010,671145531});
		idMap.put("美食",new int []{671000161,671002067,671002498,671002534,671016021,671050873,671060678,671208473,671209783});
		idMap.put("设计",new int []{671006276,671008790,671017437,671044682,671097280,671101337,671131511,671148628,671172431});
		idMap.put("时尚",new int []{671000856,671002454,671002690,671048756,671063410,671105124,671120323,671120906,671171355});
		idMap.put("艺术品",new int []{671005916,671006824,671007179,671113904,671116447,671129190,671200521});
		idMap.put("家居",new int []{671000706,671003893,671004742,671023635,671099715,671115134,671129523});
		idMap.put("动漫",new int []{671003131,671005269,671024652,671030288,671038714,671082166});
		idMap.put("建筑",new int []{671009859,671039837,671098252,671118109,671120202,671188642});
		idMap.put("汽车",new int []{671041558,671042937,671044024,671091460,671102965,671109268});
		idMap.put("星座",new int []{671000379,671059801,671075793,671125219,671191046,671228221});
		idMap.put("宠物",new int []{671000849,671002798,671004456,671009631,671054545,671102618});
		idMap.put("科学",new int []{671006512,671013379,671022464,671062753,671112637});
		idMap.put("军事",new int []{671083025,671101770,671127857,671161008,671162322,671225080});
		idMap.put("游戏",new int []{671002516,671109927,671226331,671250490,671254047});
		idMap.put("篮球",new int []{671068654,671130100,671131706,671176248,671207632,671248825});
		idMap.put("数码",new int []{671001011,671002948,671012046,671047387,671063568,671250349});
		idMap.put("互联网",new int []{671020126,671027202,671077851,671176902,671188311,671205576});
		idMap.put("自然",new int []{671013467,671018142,671169781,671169878,671179492,671221145});
		idMap.put("服饰",new int []{671006773,671007306,671039137,671134095,671153878,671210936});
		idMap.put("购物",new int []{671002528,671023573,671183268,671200157,671224858,671253679});
		idMap.put("厨艺",new int []{671002498,671002534,671016021,671019826,671051220,671240962});
		idMap.put("文艺",new int []{671000367,671001393,671005007,671015560,671052674,671206640});
		idMap.put("短片",new int []{671001811,671002233,671009402,671034655,671078386,671244301});
		idMap.put("武器",new int []{671047306,671092986,671101770,671139821,671242943});
	}*/
	
	
	
    @Get
    @Post
    public String get(Invocation inv){
		
		if(guideRecommendLogic.getGuideRecommendContentList()==null){
    		guideRecommendLogic.loadData();
        }
    	 
    	 HttpServletRequest request=inv.getRequest();
    	 User host = GuideXFiveRequestUtil.getCurrentHost(request); 
    	 int userid=host.getId();
    	 String skip=request.getParameter("skip");
		if (skip != null && "1".equals(skip)) {
			try {
				// 改标志
				StepStatusLogic.updateStepStatus(host.getId(),
						StepStatusLogic.STEP_FlOW_RECOMMEND);
				GuideLogger.printLog("| host:" + host.getId() + " update step:"
						+ StepStatusLogic.STEP_FlOW_RECOMMEND);
			} catch (Exception e) {
				e.printStackTrace();
			}
			return "f:lead";
		}
    	 String interestNmaes=request.getParameter("interestnames");
    	 if(interestNmaes==null || interestNmaes.length()<=0){
             GuideLogger.printLog("interestNmaes is not correct xiaozhanids:"+interestNmaes);
             try {
 				//改标志
 				StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FlOW_RECOMMEND);
 				GuideLogger.printLog("| host:"+host.getId()+" update step:"+StepStatusLogic.STEP_FlOW_RECOMMEND);
 			} catch (Exception e) {
 				e.printStackTrace();
 			}
             return "f:lead";
    	 }
    	 GuideLogger.printLog("interestNmaes:"+interestNmaes);
    	 String [] nameList=interestNmaes.split(",");
    	 int size =nameList.length;
			for(int i=0;i<size;i++){
				if(nameList[i]!=null && nameList[i].length()>0){
					guideRecommendLogic.insertGuideRecommendCountViewLog(
							userid, 3, -1,"guide5and6flow",nameList[i]);
					
					List<RecommendContent> ids=guideRecommendLogic.getSortGuideRecommendMap().get(nameList[i]);
					
					if(ids!=null){
						guideRecommendLogic.sendFeed1(ids, userid);
					}
					
				}
				
			}
    
    	 try {
				//改标志
				StepStatusLogic.updateStepStatus(host.getId(), StepStatusLogic.STEP_FlOW_RECOMMEND);
				GuideLogger.printLog("| host:"+host.getId()+" update step:"+StepStatusLogic.STEP_FlOW_RECOMMEND);
			} catch (Exception e) {
				e.printStackTrace();
			}
    	 return "f:lead";
    }
    
    //不知道为什么不行
    @Post("skipstep")
    @Get("skipstep")
	public String skip(Invocation inv) {
		User host = GuideXFiveRequestUtil.getCurrentHost(inv.getRequest());
		try {
			// 改标志
			StepStatusLogic.updateStepStatus(host.getId(),
					StepStatusLogic.STEP_FlOW_RECOMMEND);
			GuideLogger.printLog("| host:" + host.getId() + " update step:"
					+ StepStatusLogic.STEP_FlOW_RECOMMEND);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return "f:lead";
	}
}

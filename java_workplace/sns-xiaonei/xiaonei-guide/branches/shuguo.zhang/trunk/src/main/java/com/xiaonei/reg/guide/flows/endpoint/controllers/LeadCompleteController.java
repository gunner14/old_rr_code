package com.xiaonei.reg.guide.flows.endpoint.controllers;

import java.io.IOException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.renren.sns.guide.utils.GuideUtils;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.wService.userGuide.adapter.UserGuideAdapter;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

@LoginRequired
@Path("compld")
@IgnoreUserStatusImportedValidation
public class LeadCompleteController {
	private final int thisStep = StepStatusLogic.STEP_COMPLETE_GUIDE;
	//private final static String SUCC = "r:/guide.do?t=first";
	//2010-8-3 12:29:24 改成JSP吧……
	private final static String SUCC = "/views/guide/endpoint/leadend.jsp";
	
	
	@Post
	public String post(Invocation inv){  
		GuideLogger.printLog(" - start ");
		GuideLogger.printLog(" - end to get ");
		return get(inv);
	}
	
	@Get
	public String get(Invocation inv){
		
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		
		if(request == null){
			GuideLogger.printLog(" request null ",GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
			GuideLogger.printLog(" host:"+host.getId()+" - start ");
			GuideTimeCost cost = GuideTimeCost.begin();

			try {
				//加Page
				/*if(GuideUtil.hasExtra_XXX(host, IRegExtraMsgLogic.reg_im_mtxx)){
					//GuideXFiveUserPageLogic.getInstance().beFansofPage("600008483", host);
					// TODO Auto-generated catch block
				}*/
			    String fromsource = RegFromSourceLogic.getFromSource(host);
			    /*int stepstatus = com.xiaonei.reg.guide.root.logics.StepStatusLogic.getInstance().getStatusById(host.getId());
			    String ret = StepProcessLogic.getInstance().goWhere(host,fromsource,stepstatus);
			    if(!"f:compld".equals(ret)){
			        return ret;
			    }*/
			    if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
	                GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
	                return "r:http://reg.renren.com/sb/fillemail";
	            }
			    
			    
			    List<String> fromlist = RegFromSourceLogic.getFromSoureList(fromsource);
			    if(fromlist.contains(RegFromSourceLogic.FS_LEAD_ACB)){
			        GuideLogger.printLog("entryMtxx:"+host.getId());
	                java.util.Map<String,String> map = new java.util.HashMap<String,String>();
	                map.put("pageid","600008483");
	                UserGuideAdapter.getInstance().push(host.getId(),UserGuideAdapter.FOLLOW_PAGE,map);
			    }
			    
			} catch (Exception e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			
            try {
				//(new OfferFriendsGuideAdapter<DefaultOfferData>(new DefaultOfferFactory())).getOfferRandom(host.getId(), 50);
				//(new OfferFriendsGuideAdapter<DefaultOfferData>(new DefaultOfferFactory())).rebuildOffer(host.getId());
            	//算可能认识的人的通知
            	//2010-6-9 10:11:56 换接口
            	OfferFriendsAdapter.getInstance().rebuild(host.getId());
			} catch (Exception e) {
				GuideLogger.printLog("data err host:" + host.getId() + e.toString(), GuideLogger.ERROR);
				e.printStackTrace();
			}
			cost.endStep();
			
			
			GuideUtils.getInstance().setPassportStatusCutDown(host);
			StepStatusLogic.updateStepStatus(host.getId(), thisStep);
			GuideLogger.printLog(" host:"+host.getId()+" update step:"+thisStep);
			
			//inshion 分尾号安装的需求 删除 2010-11-01 17:21:37
			/*Runnable task_save = new Runnable() {
				public void run() {
					try {
						int tail = host.getId() % 10;
						if(false
								|| tail == 0
								|| tail == 1
								|| tail == 2
								|| tail == 3
								|| tail == 4
								|| tail == 5
								|| tail == 6
								|| tail == 7
								|| tail == 8
						){
							
							AppUserFacade.getInstance().installAppsForRemainder(tail,host);
						}
					} catch (Exception e) {
						GuideLogger.printLog("save err host:" + host.getId() + e.toString(), GuideLogger.ERROR);
						e.printStackTrace();
					}
				}
			};
			GuideLogger.printLog(" host:"+host.getId()+" install app");
			RunnableTaskExecutor.getInstance().runTask(task_save);
			*/
			
			String old_fromsource = com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic.getFromSource(host);
			String lead_flag = com.renren.sns.lead.core.LeadCoreLogic.getInstance().getLeadCoreFlag(host,
					old_fromsource);
			if ("_ABB".equals(lead_flag)) {  
				GuideLogger.printLog(" host:" + host.getId() + " Lead Over - to gaokao");
				try {
					response.sendRedirect("http://huodong.renren.com/gaokao");
				} catch (IOException e) {
					GuideLogger.printLog(" response.sendRedirect: huodong.renren.com/gaokao error");
					e.printStackTrace();
				}
			}
		/*	
			
			if ("_ABG".equals(lead_flag)) {
                GuideLogger.printLog(" host:" + host.getId() + " Lead Over - to ha7");
                try {
                    response.sendRedirect("http://huodong.renren.com/bian/ha7");
                } catch (IOException e) {
                    GuideLogger.printLog(" response.sendRedirect: huodong.renren.com/ha7 error");
                    e.printStackTrace();
                }
            }
			*/
			
			GuideLogger.printLog(" - end ");
			cost.endStep();
			cost.endFinally();
		}
		return SUCC;
	}
}

package com.xiaonei.reg.guide.flows.endpoint.controllers;

import java.io.IOException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.renren.sns.guide.service.GuideRecommendLogic;
import com.renren.sns.guide.utils.GuideUtils;
import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.page.xoa.adapter.PageClientAdapter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

@LoginRequired
@Path("compld")
@IgnoreUserStatusImportedValidation
public class LeadCompleteController {
	
	@Autowired
	private  GuideRecommendLogic guideRecommendLogic;
	
	
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
			String fromsource="";
			int userid=host.getId();

			try {
				

		        // int[] appIds = new int[]{166607,166613,166615,166616,166617};
		        // RecentMenuLogicAdapter.getInstance().tracks(Integer.valueOf(host.getId()),appIds);

				//加Page
				/*if(GuideUtil.hasExtra_XXX(host, IRegExtraMsgLogic.reg_im_mtxx)){
					//GuideXFiveUserPageLogic.getInstance().beFansofPage("600008483", host);
					// TODO Auto-generated catch block
				}*/
				 
				 if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
		                GuideLogger.printLog("returntoregister id:"+userid+" domain:"+host.getDomain()+" status:"+host.getStatus());
		                return "r:http://reg.renren.com/sb/fillemail";
		            }
				 
				 
			    fromsource = RegFromSourceLogic.getFromSource(host);
			    int stepstatus = com.xiaonei.reg.guide.root.logics.StepStatusLogic.getInstance().getStatusById(host.getId());
			    String ret = StepProcessLogic.getInstance().goWhere(host,fromsource,stepstatus);
			    if(!"f:compld".equals(ret)){
			        return ret;
			    }
			    
			    //guideRecommendLogic.followUniversityChannel(host);//关注校园频道
			    
			    //关注校园page
			    if(host.getStage()==20){
			    	GuideLogger.printLog("*******ACE entry");
			    	guideRecommendLogic.followUniversityPage(host.getId());
			    }
			    
			    
			    List<String> fromlist = RegFromSourceLogic.getFromSoureList(fromsource);
			    if(fromlist.contains(RegFromSourceLogic.FS_LEAD_ACB)){
			        GuideLogger.printLog("entryMtxx:"+userid);
	                //java.util.Map<String,String> map = new java.util.HashMap<String,String>();
	               // map.put("pageid","600008483");
	               // UserGuideAdapter.getInstance().push(host.getId(),UserGuideAdapter.FOLLOW_PAGE,map);
	                PageClientAdapter.makeFans(600008483, host.getId(), true);

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
            	OfferFriendsAdapter.getInstance().rebuild(userid);
			} catch (Exception e) {
				GuideLogger.printLog("data err host:" + host.getId() + e.toString(), GuideLogger.ERROR);
				e.printStackTrace();
			}
			cost.endStep();
			
			
			GuideUtils.getInstance().setPassportStatusCutDown(host);
			StepStatusLogic.updateStepStatus(userid, thisStep);
			
			//以下是往日志表里打信息
           // String fromeSource=RegFromSourceLogic.getFromSource(host).trim();
            try {
                if(fromsource==null||fromsource.trim().length()<=0){
                    fromsource="nosource";
                }
                //下面是AAF逻辑
                if (fromsource.endsWith("_AAF")) {
                    if ("".equals("gg")) {
                    	
                    	guideRecommendLogic.insertNewflowLogRecord(userid, 
                                host.getStage(),
                                fromsource+"_I0F0", -2,"C0");
                    } else {
                        if (userid%10==1 || userid%10==2) {
                            
                        	guideRecommendLogic.insertNewflowLogRecord(userid, 
                                    host.getStage(),
                                    fromsource+"_I0F0H0", -2,"C0");
                            
						}
                        else if (userid%10==3 || userid%10==4) {
                        	guideRecommendLogic.insertNewflowLogRecord(userid,
											host.getStage(), fromsource+"_I0F0Z3H0", -2,
											"C0");
                        } 
                        else if (userid%10==5 || userid%10==6 ||userid%10==7 || userid%10==8) {

                        	guideRecommendLogic.insertNewflowLogRecord(userid,
											host.getStage(), fromsource+"_I0F0Z2H0", -2,
											"C0");

							// mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(),
							// "userid:"+userid,(int) MemCacheKeys.day * 31);
                        } 
                        else if (userid % 10 == 9) {

                        	guideRecommendLogic.insertNewflowLogRecord(userid, 
                                    host.getStage(),
                                    fromsource+"_I0F0", -2,"C0");
                        } 
                        
                        else if (userid % 10 == 0) {

                        	guideRecommendLogic.insertNewflowLogRecord(userid, 
                                    host.getStage(),
                                    fromsource+"_I0F0Z2", -2,"C0");
                        } 
                        
                        else {
                        	GuideLogger.printLog("exception userid:"+userid,GuideLogger.ERROR);
                        	guideRecommendLogic.insertGuideLogRecord(userid, 
                                    host.getStage(),
                                    fromsource, -2);
                        }
                    }
                }
                
                //下面是AAR逻辑
                else if (fromsource.endsWith("_AAR")) {
                    if ("".equals("gg")) {
                    	
                    	guideRecommendLogic.insertNewflowLogRecord(userid, 
                                host.getStage(),
                                fromsource+"_I0F0Z1H0", -2,"C0");
                    } else {
                        if (userid%10==1 || userid%10==2) {
                            
                        	guideRecommendLogic.insertNewflowLogRecord(userid, 
                                    host.getStage(),
                                    fromsource+"_I0F0H0", -2,"C0");
                            
						}
                        else if (userid%10==3 || userid%10==4 || userid%10==5 || userid%10==6
				        		|| userid%10==7 || userid%10==8 || userid%10==9 || userid%10==0) {

                        	guideRecommendLogic.insertNewflowLogRecord(userid,
											host.getStage(), fromsource+"_I0F0Z2H0", -2,
											"C0");

							// mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(),
							// "userid:"+userid,(int) MemCacheKeys.day * 31);
                        } 
                        
                        
                        else {
                        	GuideLogger.printLog("exception userid:"+userid,GuideLogger.ERROR);
                        	guideRecommendLogic.insertGuideLogRecord(userid, 
                                    host.getStage(),
                                    fromsource, -2);
                        }
                    }
                }
                
              //下面是AAP逻辑
                else if (fromsource.endsWith("_AAP")) {
                    if ("".equals("gg")) {
                    	
                    	guideRecommendLogic.insertNewflowLogRecord(userid, 
                                host.getStage(),
                                fromsource+"_I0F0Z1H0", -2,"C0");
                    } else {
                        if (userid%10==1 || userid%10==2) {
                            
                        	guideRecommendLogic.insertNewflowLogRecord(userid, 
                                    host.getStage(),
                                    fromsource+"_I0F0H0", -2,"C0");
                            
						}
                        else if (userid%10==3 || userid%10==4 || userid%10==5 || userid%10==6
				        		|| userid%10==7 || userid%10==8 || userid%10==9 || userid%10==0) {

                        	guideRecommendLogic.insertNewflowLogRecord(userid,
											host.getStage(), fromsource+"_I0F0Z2H0", -2,
											"C0");

							// mem.set(GuideUtils.GUIDE_V6_USER+logUser.getId(),
							// "userid:"+userid,(int) MemCacheKeys.day * 31);
                        } 
                        
                        else {
                        	GuideLogger.printLog("exception userid:"+userid,GuideLogger.ERROR);
                        	guideRecommendLogic.insertGuideLogRecord(userid, 
                                    host.getStage(),
                                    fromsource, -2);
                        }
                    }
                }
                
                else{
                	guideRecommendLogic.insertGuideLogRecord(userid, 
                            host.getStage(),
                            fromsource, -2);
                }
            } catch (Exception e1) {
                e1.printStackTrace();
            }
            //打印日志结束
			
			
			
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

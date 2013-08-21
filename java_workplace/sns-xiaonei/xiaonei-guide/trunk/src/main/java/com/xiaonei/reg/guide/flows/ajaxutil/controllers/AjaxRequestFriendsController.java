package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveSupportFriendsUtil;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * AjaxRequestFriendsController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-25 添加说明 
 */
@LoginRequired
@Path("ajaxRequestFriends1")
public class AjaxRequestFriendsController {
	@Post
	@Get
	public String post(Invocation inv){
		final HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
			GuideTimeCost cost = GuideTimeCost.begin();
			cost.endStep();
			// 用户id组
			String user_id = request.getParameter("friends");
			final String from_flag = request.getParameter("from");
			if(from_flag == null){
				String ref = request.getHeader("Referer");
				GuideLogger.printLog(" host:"+host.getId()+" from null!"+" ref:"+ref,GuideLogger.WARN);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" from:"+from_flag);
			}
			cost.endStep();
			String[] user_ids = { "" };
			cost.endStep();
			if (user_id != null && !"".equalsIgnoreCase(user_id)) {
				user_ids = user_id.split(",");
				cost.endStep();
			}
			final String[] ids = user_ids;
			cost.endStep();
			try {
				cost.endStep();
				PrintWriter out = response.getWriter();
				final String currentDomain = OpiConstants.domain.toString();			
				//异步请求开始
				Runnable taskSave_NET = new Runnable() {
					public void run() {
						boolean error = false;
						GuideTimeCost cost = GuideTimeCost.begin();
						cost.endStep();
						if (ids != null && ids.length > 0) {// 分发多个请求
							GuideLogger.printLog(" host:"+host.getId()+" request count:"+ ids.length);
							for (int i = 0; i < ids.length; i++) {
								cost.endStep();
								int accepterId = 0;
								try {
									accepterId = Integer.parseInt(ids[i]);
								} catch (NumberFormatException e) {
									BaseActionUtil.addMsg(request, "您没有向任何人发出邀请");
									error = true;
									break;
								}
								cost.endStep();
								if (accepterId > 0) {
									User touser = SnsAdapterFactory.getUserAdapter().get(accepterId);
									if(touser!=null){									    
	                                        //FriendApply fa=new FriendApply(host, touser,currentDomain);
	                                        try {
	                                            //FriendsHome.getInstance().requestFriendApply(fa);
	                                            GuideXFiveSupportFriendsUtil.beFriendsAsync(host, ids, currentDomain, "old_ajax_a");
	                                        } catch (Exception e) {
	                                            // TODO Auto-generated catch block
	                                            e.printStackTrace();
	                                        }
									}
								}
								cost.endStep();
							}
							cost.endStep();
							if (!error){
								BaseActionUtil.addMsg(request, "已经成功向" + ids.length+ "人发出了添加为好友的请求，请等待对方确认");
							}
							cost.endStep();
							if (
							    host.getStage() == Stage.STAGE_MAJOR) {
								GuideLogger.printLog(" host:"+host.getId()+"to print 2 ...");
							} else {
								GuideLogger.printLog(" host:"+host.getId()+"to print 1 ...");
							}
							cost.endStep();
						} else {
							GuideLogger.printLog(" host:"+host.getId()+"to print 0 ...");
						}					
						cost.endFinally();
					}
				};
				RunnableTaskExecutor.getInstance().runTask(taskSave_NET);
				//异步请求结束
				GuideLogger.printLog(" host:"+host.getId()+"to print 1 ...");
				out.print("1");
				cost.endStep();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
			cost.endFinally();
		}
		
		return "@";
	}
}

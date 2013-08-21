package com.xiaonei.reg.activity.msntab.controllers;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.json.JSONArray;
import org.json.JSONObject;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.component.application.notification.AppNotificationMgr;
import com.xiaonei.platform.component.application.notification.exception.AppNotificationException;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.activity.msntab.logic.InviteForMSNTabLogic;
import com.xiaonei.reg.activity.msntab.logic.InviteForMSNTabLogic.MSNUserInfo;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;

@LoginRequired
@Path("msninit")
public class MsnTabInitController {
	private static final String TO_INIT = "/views/activity/msntab/msntab.jsp";
	private static final String TO_SUBMIT = "r:http://www.renren.com";
	@Get
	@Post
	public String index(Invocation inv){
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		User host = GuideXFiveRequestUtil.getCurrentHost(request);		
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
			try {
				String action = GuideXFiveRequestUtil.getParamString(request, "a", host.getId());
				if(MyStringUtils.equals("submit", action)){
					//doNotify(inv,request,host); 
					return TO_SUBMIT;
				}
				else if(MyStringUtils.equals("login", action)){
					return TO_SUBMIT;
				}
				
			} catch (Exception e) {
				// TODO: handle exception
			}
			try {
				List<MSNUserInfo> ulist = InviteForMSNTabLogic.getInstance().getRelations(host);
				if(ulist == null) ulist = new ArrayList<MSNUserInfo>();
				String json = getJson(ulist);
				GuideLogger.printLog(" host:"+host.getId()+" ulist:"+ulist.size());
				inv.addModel("activity_msn_tab_list",ulist);
				inv.addModel("activity_msn_tab_json_list", json);
				inv.addModel("activity_msn_tab_hostname",host.getName());
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString() ,GuideLogger.ERROR);
			}
			
		}
		
		
		return TO_INIT;
	}

	@SuppressWarnings("unused")
	private void doNotify(Invocation inv, HttpServletRequest request, final User host) {
		Runnable task_nofity = new Runnable() {
			public void run() {
				try {
					Set<Integer> idset = new HashSet<Integer>();
					
					List<Integer> friendlist = FriendsHome.getInstance().getFriendsIds(host.getId());
					int cnt = 0;
					for (Integer fid : friendlist) {
						if(cnt++ > 20) break;
						idset.add(fid);
					}
					String regurl = "http://a619.oadz.com/link/C/619/110803/ziWH2-gtkmcjpYWviewypwx-rdk_/a/1860/reg.msn.cn"; 
					String notes = "<a href='http://www.renren.com/profile.do?id="+host.getId()+"'>"+host.getName()+"</a> 通过 MSN登录了 人人网，马上去加他为MSN好友吧。如果没有MSN，<a href='"+regurl+"'>点此立即注册</a>。";
					AppNotificationMgr.sendNotification(101558, host.getId(),
							idset, notes,
							AppNotificationMgr.NOTIFICATION_TYPE_USER_TO_USER);
					// GuideLogger.printLog(" host:"+host.getId()+" fake_noteto:"+invitee.getInvitee_id());
				} catch (AppNotificationException e) {
					GuideLogger.printLog(" host:" + host.getId() + " to:"	+ " e:" + e.toString(), GuideLogger.ERROR);
					// TODO Auto-generated catch block
					// e.printStackTrace();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}

		};
		RunnableTaskExecutor.getInstance().runTask(task_nofity);
		
	}

	private String getJson(List<MSNUserInfo> ulist) {
		// 返回数据
		JSONObject jo = new JSONObject();
		JSONArray retCandidate = new JSONArray();
		try {
			if(ulist != null){
				for (MSNUserInfo userInfo : ulist) {
					if(userInfo != null){
						JSONObject joinner = new JSONObject();
						joinner.put("head", userInfo.uhead);
						joinner.put("name", userInfo.uname);
						retCandidate.put(joinner);
					}
				}
			}
			
			
			jo.put("candidate", retCandidate);
			return jo.toString();
		} catch (Exception e) {
			// TODO: handle exception
		}
		return "";
	}
}

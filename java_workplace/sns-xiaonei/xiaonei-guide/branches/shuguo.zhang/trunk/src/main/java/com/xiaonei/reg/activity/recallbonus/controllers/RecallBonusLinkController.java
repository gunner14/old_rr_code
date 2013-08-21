package com.xiaonei.reg.activity.recallbonus.controllers;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.reg.activity.recallbonus.logic.RecallBonusInviteLogic;
import com.xiaonei.reg.activity.recallbonus.model.RecallBonusInviteModel;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;

@Path("rcbonuslink")
public class RecallBonusLinkController {

	@Get
	@Post
	public String index(Invocation inv){
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		int uid = GuideXFiveRequestUtil.getParamInteger(request, "uid", 0);
		String action = GuideXFiveRequestUtil.getParamString(request, "a", 0);
		if(MyStringUtils.isNullOrEmpty(action)){
		}
		
		if(MyStringUtils.equals("init", action)){//init
		}
		else if(MyStringUtils.equals("invite", action)){//invite
		}
		else if(MyStringUtils.equals("invitefriends", action)){//invite
			String link = doGetLink(request, response, uid);
			return "@"+link;
		}
		return "@";
	}
	
	public List<Integer> doGetCanLinkId(HttpServletRequest request,HttpServletResponse response, int uid){
		List<Integer> ids = RecallBonusInviteLogic.getInstance().getAllInviteeId(uid,100);
		return ids;
	}
	
	private String doGetLink(HttpServletRequest request,	HttpServletResponse response, int uid) {
		List<RecallBonusInviteModel> list = RecallBonusInviteLogic.getInstance().getByInviterIdForLink(uid);
		//String ids = "_";
		StringBuilder ids = new StringBuilder();
		ids.append("_");
		for (RecallBonusInviteModel m : list) {
			//ids += m.getId();
			ids.append(m.getId());
		}
		
		String sids = ids.toString().substring(1);
		GuideLogger.printLog(" host:"+uid+" linkids:"+sids);
		String link = ""+"gudie."+OpiConstants.domain+"/rcbonus?a=accept&ids="+sids;
		GuideLogger.printLog(" host:"+uid+" link:"+link);
		return link;
	}
}

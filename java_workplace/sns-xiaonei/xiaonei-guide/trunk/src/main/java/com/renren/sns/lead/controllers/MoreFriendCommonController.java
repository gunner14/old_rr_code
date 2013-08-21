package com.renren.sns.lead.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.lead.core.ILeadConstants;
import com.renren.sns.lead.core.LeadConfigUtil;
import com.renren.sns.lead.core.MapperFactory;
import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.controllers.MoreFriendController;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

@LoginRequired
@Path( { "morefriendnew" })
public class MoreFriendCommonController extends LeadBaseController{
	private final AbstractLeadPageMapper mapper = MapperFactory.getMapper("Morefriend");
	
	@Override
	public String doIndex(Invocation inv, Portal portal, User host,CacheManager cm) {
		portal.setTimeout(300);//超时300ms就切除
		String lead_flag = (String)inv.getModel(STRING_LEAD_TYPE_FLAG);
		if("_AAJ".equals(lead_flag)){
			doingAAJ(inv, portal, host, cm);
		}
		else{
			String initmethodkey = LeadConfigUtil.getPageInit(host, lead_flag, ILeadConstants.STEP_KEY_MOREFRIEND);
			if(!MyStringUtils.isNullOrEmpty(initmethodkey)){
				this.doingCustomMethod(inv, portal, host, cm, initmethodkey);
			}
			else{
				this.doing(inv,portal, host, cm);
			}
		}
		
		
		return "";
	}

	private void doing(Invocation inv,Portal portal, User host, CacheManager cm) {
	}
	
	protected void doingGetMoreFriend(Invocation inv,Portal portal, User host, CacheManager cm) {
		MoreFriendController.doAAFSearch(inv, inv.getRequest(), host, 0);
	}
	
	private void doingAAJ(Invocation inv,Portal portal, User host, CacheManager cm) {
		addWindow(portal, "get_guideGetOfferFriends", "guideGetOfferFriends"); 
		addWindow(portal, "get_guideGetRequestFriendsCluster", "guideGetRequestFriendsCluster"); 
	}
	
	@Override
	public AbstractLeadPageMapper getMapper(User host) {
		return mapper;
	}
	
    
}

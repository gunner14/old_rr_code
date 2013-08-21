package com.renren.sns.lead.init.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.lead.controllers.LeadBaseController;
import com.renren.sns.lead.core.ILeadConstants;
import com.renren.sns.lead.core.LeadConfigUtil;
import com.renren.sns.lead.core.MapperFactory;
import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.additional.RegEmailLogic;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

@LoginRequired
@Path( { "mergeuserinit" })
public class DaqinMergeUserInitController extends LeadBaseController{
	private final AbstractLeadPageMapper mapper = MapperFactory.getMapper("Mergeuser");
	
	@Override
	public String doIndex(Invocation inv, Portal portal, User host,CacheManager cm) {
		String lead_flag = (String)inv.getModel(STRING_LEAD_TYPE_FLAG);
		String initmethodkey = LeadConfigUtil.getPageInit(host, lead_flag, ILeadConstants.STEP_KEY_MERGEUSER);
		GuideLogger.printLog(" host:"+host.getId()+" lead_type_flag:"+lead_flag+" mk:"+initmethodkey);
		if(!MyStringUtils.isNullOrEmpty(initmethodkey)){
			this.doingCustomMethod(inv, portal, host, cm, initmethodkey);
		}
		else{
			this.doing(inv,portal, host, cm);
		}
		
		return "";
	}

	private static final MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_user_space);
	private static final String merge_key_prefix = "merge_send_";
	private void doing(Invocation inv, Portal portal, User host, CacheManager cm) {
		//send email vcode
		Integer willsend = (Integer)mem.get(merge_key_prefix+host.getId());
		GuideLogger.printLog(" host:"+host.getId()+" willsend:"+willsend);
		if(willsend!=null && willsend == 1){
			mem.incr(merge_key_prefix+host.getId());
			RegEmailLogic.getInstance().sendMergeEmail(host);
		}
		GuideLogger.printLog(" host:"+host.getId()+" willmerge!",GuideLogger.WARN);
	}
	

	@Override
	public AbstractLeadPageMapper getMapper(User host) {
		return mapper;
	}
	
    
}

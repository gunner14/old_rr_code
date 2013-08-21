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
import com.xiaonei.reg.guide.flows.xfive.controllers.ImportmsnInitController;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

@LoginRequired
@Path( { "importnew" })
public class ImportCommonController extends LeadBaseController{
	private final AbstractLeadPageMapper mapper = MapperFactory.getMapper("Import");
	
	@Override
	public String doIndex(Invocation inv, Portal portal, User host,CacheManager cm) {
		String lead_flag = (String)inv.getModel(STRING_LEAD_TYPE_FLAG);
		String initmethodkey = LeadConfigUtil.getPageInit(host, lead_flag, ILeadConstants.STEP_KEY_PORTRAIT);
		GuideLogger.printLog(" host:"+host.getId()+" lead_type_flag:"+lead_flag+" mk:"+initmethodkey);
		if(!MyStringUtils.isNullOrEmpty(initmethodkey)){
			this.doingCustomMethod(inv, portal, host, cm, initmethodkey);
		}
		else{
			this.doing(inv,portal, host, cm);
		}
		
		return "";
	}

	protected void doingKouxiangtang(Invocation inv, Portal portal, User host, CacheManager cm) {
		try {
        	
			ImportmsnInitController.doDefault(inv.getRequest(), host);
        } catch (Exception e) {
        	GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        } 
        
	}
	private void doing(Invocation inv, Portal portal, User host, CacheManager cm) {
		
	}
	

	@Override
	public AbstractLeadPageMapper getMapper(User host) {
		return mapper;
	}
	
    
}

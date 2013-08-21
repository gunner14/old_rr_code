package com.renren.sns.lead.controllers;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.lead.core.ILeadConstants;
import com.renren.sns.lead.core.LeadConfigUtil;
import com.renren.sns.lead.core.MapperFactory;
import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

@LoginRequired
@Path( { "fillinfonew" })
public class FillinfoCommonController extends LeadBaseController{
	private final AbstractLeadPageMapper mapper = MapperFactory.getMapper("Fillinfo");
	
	@Override
	public String doIndex(Invocation inv, Portal portal, User host,CacheManager cm) {
		String lead_flag = (String)inv.getModel(STRING_LEAD_TYPE_FLAG);
		String initmethodkey = LeadConfigUtil.getPageInit(host, lead_flag, ILeadConstants.STEP_KEY_FILLINFO);
		GuideLogger.printLog(" host:"+host.getId()+" lead_type_flag:"+lead_flag+" mk:"+initmethodkey);
		if(!MyStringUtils.isNullOrEmpty(initmethodkey)){
			this.doingCustomMethod(inv, portal, host, cm, initmethodkey);
		}
		else{
			this.doing(inv,portal, host, cm);
		}
		
		return "";
	}

	
	private void doing(Invocation inv, Portal portal, User host, CacheManager cm) {
		
	}
	
	private void doingStageInit(Invocation inv, Portal portal, User host, CacheManager cm){
		if(host == null){
			GuideLogger.printLog(" host null",GuideLogger.WARN);
		}
		else{
			try {
				inv.addModel("lead_fillinfo_stage", host.getStage());
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString()+" ",GuideLogger.ERROR);
			}
		}
	}
	protected void doingMSNFillinfoInit(Invocation inv, Portal portal, User host, CacheManager cm) {
		inv.addModel("test_mod","test_modtest_modtest_modtest_modtest_mod");
	}
	
	public static void main(String[] args) throws SecurityException, NoSuchMethodException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {
		/*Method[] ms = FillinfoCommonController.class.getDeclaredMethods();
		for (Method method : ms) {
			System.out.println(method.getName());
		}*/
		Method m = FillinfoCommonController.class.getDeclaredMethod("doingMSNFillinfoInit",Invocation.class, Portal.class,User.class,CacheManager.class);
		System.out.println(m.getName());
		FillinfoCommonController c = new FillinfoCommonController();
		m.invoke(c, null,null,null,null);
		c.doingStageInit(null,null,null,null);
	}
	

	@Override
	public AbstractLeadPageMapper getMapper(User host) {
		return mapper;
	}
	
    
}

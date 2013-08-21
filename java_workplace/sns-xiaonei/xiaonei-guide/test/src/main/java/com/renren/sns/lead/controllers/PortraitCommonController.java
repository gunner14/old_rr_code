package com.renren.sns.lead.controllers;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.lead.core.ILeadConstants;
import com.renren.sns.lead.core.LeadConfigUtil;
import com.renren.sns.lead.core.MapperFactory;
import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.renren.sns.lead.dataproviders.AbstractLeadDataCallable;
import com.renren.sns.lead.dataproviders.LeadDataCallableSubmitter;
import com.renren.sns.lead.dataproviders.fact.LeadDataCallableFactory;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveCommonCacheLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

@LoginRequired
@Path( { "portraitnew" })
public class PortraitCommonController extends LeadBaseController{
	private final AbstractLeadPageMapper mapper = MapperFactory.getMapper("Portrait");
	
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

	
	private static final LeadDataCallableSubmitter DataSubmitter = new LeadDataCallableSubmitter(PortraitCommonController.class);
	private static final String lead_more_friend_confirmlist = "lead_more_friend_confirmlist";
	@SuppressWarnings("unchecked")
	protected void doingKouxiangtang(Invocation inv, Portal portal, User host, CacheManager cm) {
		try {
        	//List<Integer> list = FriendsHome.getInstance().getApplyFriendList(host.getId());
        	List<Integer> list = GuideXFiveCommonCacheLogic.getInstance().loadConfirmList(host);
        	if(list == null){
        		GuideLogger.printLog(" host:"+host.getId()+" cache null");
        		AbstractLeadDataCallable<List<Integer>> call_applylist = LeadDataCallableFactory.getMyApplyListCallable(host, 20);
        		List<AbstractLeadDataCallable<? extends Object>> tasks = new ArrayList<AbstractLeadDataCallable<? extends Object>>();
        		tasks.add(call_applylist);
        		Map<String, Object>  retmap = DataSubmitter.getDatas(host, 200, tasks, true);
				list = (List<Integer>) retmap.get(call_applylist.getKey());
        		//list = FriendsHome.getInstance().getApplyFriendList(host.getId());
        	}
        	if(list!=null){
        		String json = GuideUtil.getJSONForFriendSelectorFromIdsForPortrait(list);
            	inv.addModel(lead_more_friend_confirmlist, json);
            }
        	else{
        		inv.addModel(lead_more_friend_confirmlist, "{\"candidate\":[]}");
        		
        	}
        	
        } catch (Exception e) {
        	GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        } 
        
	}
	private void doing(Invocation inv, Portal portal, User host, CacheManager cm) {
		
	}
	
	public static void main(String[] args) throws SecurityException, NoSuchMethodException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {
		/*Method[] ms = FillinfoCommonController.class.getDeclaredMethods();
		for (Method method : ms) {
			System.out.println(method.getName());
		}*/
		Method m = PortraitCommonController.class.getDeclaredMethod("doingMSNFillinfoInit",Invocation.class, Portal.class,User.class,CacheManager.class);
		System.out.println(m.getName());
		PortraitCommonController c = new PortraitCommonController();
		m.invoke(c, null,null,null,null);
		//c.doingMSNFillinfoInit(null,null,null,null);
	}
	

	@Override
	public AbstractLeadPageMapper getMapper(User host) {
		return mapper;
	}
	
    
}

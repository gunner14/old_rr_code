package com.renren.sns.lead.controllers;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.portal.Portal;

import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.guide.utils.GuideUtils;
import com.renren.sns.lead.core.ILeadConstants;
import com.renren.sns.lead.core.LeadConfigUtil;
import com.renren.sns.lead.core.MapperFactory;
import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.renren.sns.lead.dataproviders.AbstractLeadDataCallable;
import com.renren.sns.lead.dataproviders.ILeadInitMehtod;
import com.renren.sns.lead.dataproviders.LeadDataCallableSubmitter;
import com.renren.sns.lead.dataproviders.fact.LeadDataCallableFactory;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveRelationsPreviewLogic;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.reg.guide.util.GuideUtil;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.ObjectProvider;

class PreviewInitKouxiangtang implements ILeadInitMehtod{

	@Override
	public void doing(Invocation inv, Portal portal, User host, CacheManager cm) {
		// TODO Auto-generated method stub
		GuideTimeCost tc = GuideTimeCost.begin();
		PreviewCommonController.doingKouxiangtang(inv,portal,host,cm);//List<Integer> list = LeadDataProviderFactory.getPreviewOfferfriendProvider(host, 49).getData(host, 10);
		tc.endFinally();
	}
	
}

@LoginRequired
@Path( { "previewnew","p.do" })
public class PreviewCommonController extends LeadBaseController{
	private final AbstractLeadPageMapper mapper = MapperFactory.getMapper("Preview");
	private static final Map<String,ILeadInitMehtod>  methodmap = new HashMap<String, ILeadInitMehtod>();
	static{
		methodmap.put("Kouxiangtang", new PreviewInitKouxiangtang());
	}
	
	private static final String lead_user_type = "lead_user_type";
	private static final String lead_preview_friend = "lead_preview_friend";
	private static final String lead_preview_stage = "lead_preview_stage";
	private static final String lead_preview_friend_count = "lead_preview_friend_count";
	
	private static final LeadDataCallableSubmitter DataSubmitter_OfferFriend = new LeadDataCallableSubmitter(PreviewCommonController.class+".OfferFriend"); 
	private static final LeadDataCallableSubmitter DataSubmitter_ProfileFriend = new LeadDataCallableSubmitter(PreviewCommonController.class+".ProfileFriend"); 
	
	@Override
	public String doIndex(Invocation inv, Portal portal, User host,CacheManager cm) {
		GuideTimeCost tc = GuideTimeCost.begin();
		portal.setTimeout(300);//超时300ms就切除
		tc.endStep();
		String lead_flag = (String)inv.getModel(STRING_LEAD_TYPE_FLAG);
		tc.endStep();
		inv.addModel(lead_user_type,lead_flag);
		tc.endStep();
		
		if(host!=null && host.getStatus()==10 && host.getDomain().contains("imop")){
            GuideLogger.printLog("returntoregister id:"+host.getId()+" domain:"+host.getDomain()+" status:"+host.getStatus());
            return "r:http://reg.renren.com/sb/fillemail";
        }
		
		GuideUtils.getInstance().setPassportStatusCutDown(host);
		if("_AAI".equals(lead_flag)){
			doingAAI(inv,portal, host, cm);
			tc.endStep();
		}
		if("_AAJ".equals(lead_flag)){
			doingAAJ(inv,portal, host, cm);
			tc.endStep();
		}
		else{
			String initmethodkey = LeadConfigUtil.getPageInit(host, lead_flag, ILeadConstants.STEP_KEY_PREVIEW);
			tc.endStep();
			if(!MyStringUtils.isNullOrEmpty(initmethodkey)){
				this.doingCustomMethod(inv, portal, host, cm, initmethodkey);
				/*ILeadInitMehtod init = methodmap.get(initmethodkey);
				if(init != null){
					init.doing(inv, portal, host, cm);
				}*/
				tc.endStep("doing it");
			}
			else{
				this.doing(inv,portal, host, cm);
				tc.endStep();
			}
		}
		/*if(mapper.getUserLeadType(host) == IGuideUserLeadType.Lead_AAI){
			addWindow(portal, "get_lead_preview_offer", "guideGetOfferFriends"); //可能认识的人
		}*/
		//addWindow(portal, "lead_preview_more", "guideGetMoreFriends"); //二度好友
		
		tc.endFinally();
		return "";
	}

	private void doingAAI(Invocation inv,Portal portal, User host, CacheManager cm){
		User af = GuideUtil.getAInviter(host); 
		ObjectProvider.getInstance().set(cm, "predata_ainviter", af);
		addWindow(portal, "get_guideGetProfileFriends", "guideGetProfileFriends");
		addWindow(portal, "get_guideGetOfferFriends", "guideGetOfferFriends");
	}
	private void doingAAJ(Invocation inv,Portal portal, User host, CacheManager cm){
		User af = GuideUtil.getAInviter(host); 
		ObjectProvider.getInstance().set(cm, "predata_ainviter", af);
		addWindow(portal, "get_guideGetInviterFriends", "guideGetInviterFriends");
		addWindow(portal, "get_guideGetInviterFriendsCluster", "guideGetInviterFriendsCluster");
	}
	public static void doingKouxiangtang(Invocation inv,Portal portal, User host, CacheManager cm){
		AbstractLeadDataCallable<List<Integer>> po_task = LeadDataCallableFactory.getPreviewOfferfriendCallable(host, 49);
		List<Integer> po_idlist = DataSubmitter_OfferFriend.getData(host, 100, po_task);
		
		String retjson = "";
		int retcount = 0;
		int cachecount = 0;
		String friend_srouce_flag = "";
		List<Integer> l = null;
		if(po_idlist != null && po_idlist.size() >= 0){
			l = po_idlist;
			friend_srouce_flag = "mayknow";
			Map<Integer, WUserCache> ucl = GuideUtil.getUserCacheMap(l);
			retjson = GuideUtil.getJSONForFriendSelectorFromCacheMapBySort(host.getId(), ucl);
			cachecount = ucl.size(); 
		}
		else{
			AbstractLeadDataCallable<List<GuideIRelation>> pf_task = LeadDataCallableFactory.getPreviewProfileCallable(host, 49);
			List<GuideIRelation> pf_rlist = DataSubmitter_ProfileFriend.getData(host, 200, pf_task);
			List<Integer> pf_idlist = GuideXFiveRelationsPreviewLogic.getInstance().getIdsFromGuideIRelationList(pf_rlist);
			if(pf_idlist != null && pf_idlist.size()>0){
				l = pf_idlist;
				friend_srouce_flag = "profrid";
				retjson = GuideXFiveRelationsPreviewLogic.getInstance().getJSONForFriendSelector(pf_rlist);
				cachecount = l.size();
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" ids:"+l,GuideLogger.WARN);
			}
		}
		retcount = l == null ? 0 :l.size();
		inv.addModel(lead_preview_friend,retjson);
		inv.addModel(lead_preview_friend_count, retcount);
		inv.addModel(lead_preview_stage,host.getStage());
		GuideLogger.printLog(" host:"+host.getId()+" retcount:"+retcount+" cache:" +cachecount+" flag:"+friend_srouce_flag+" list:"+l);
	}
	@SuppressWarnings({ "unchecked", "unused" })
	private static void doingAAF(Invocation inv,Portal portal, User host, CacheManager cm){
		/*List<GuideIRelation> list = GuideXFiveRelationsPreviewLogic.getInstance().getProfileResults(48, host, -1);
		List<Integer> ids = GuideXFiveRelationsPreviewLogic.getInstance().getIdsFromGuideIRelationList(list);
		Map<Integer, WUserCache> cmap = GuideUtil.getUserCacheMap(ids);
		String json = GuideUtil.getJSONForFriendSelectorFromCacheMapBySort(host.getId(), cmap);
		inv.addModel("lead_preview_friend", json);
		//inv.addModel(lead_preview_stage,host.getStage());
    	inv.addModel("lead_preview_friend_count", 10);
    	//inv.addModel(lead_user_type,user_type);*/
		GuideTimeCost tc = GuideTimeCost.begin();
		AbstractLeadDataCallable<List<Integer>> previewoffer = LeadDataCallableFactory.getPreviewOfferfriendCallable(host, 49);
    	AbstractLeadDataCallable<List<GuideIRelation>> previewprofile = LeadDataCallableFactory.getPreviewProfileCallable(host, 49);
    	List<AbstractLeadDataCallable<? extends Object>> tasks = new ArrayList<AbstractLeadDataCallable<? extends Object>>();
    	tasks.add(previewoffer);
    	tasks.add(previewprofile);
    	tc.endStep();
		Map<String, Object> retdatas = DataSubmitter_OfferFriend.getDatas(host, 10, tasks, true);
		tc.endStep();
		List<GuideIRelation> pf_rlist = (List<GuideIRelation>)retdatas.get(previewprofile.getKey());
		tc.endStep();
		//List<Integer> pf_idlist = null;
		List<Integer> pf_idlist = GuideXFiveRelationsPreviewLogic.getInstance().getIdsFromGuideIRelationList(pf_rlist);
		tc.endStep();
		
		List<Integer> po_idlist = (List<Integer>)retdatas.get(previewoffer.getKey());po_idlist=new ArrayList<Integer>();po_idlist.add(81120);
		tc.endStep();
		
		String retjson = "";
		int retcount = 0;
		int cachecount = 0;
		String friend_srouce_flag = "";
		List<Integer> l = null;
		if(po_idlist != null && po_idlist.size()>0){
			l = po_idlist;
			friend_srouce_flag = "profrid";
			Map<Integer, WUserCache> ucl = GuideUtil.getUserCacheMap(l);
			retjson = GuideUtil.getJSONForFriendSelectorFromCacheMapBySort(host.getId(), ucl);
			cachecount = ucl.size(); 
			tc.endStep("po!");
		}
		else{
			if(pf_idlist != null && pf_idlist.size()>0){
				l = pf_idlist;
				friend_srouce_flag = "mayknow";
				//retjson = GuideXFiveRelationsPreviewLogic.getInstance().getJSONForFriendSelector(pf_rlist);
				cachecount = l.size();
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" ids:"+l,GuideLogger.WARN);
			}
			tc.endStep("pf!");
		}
		
		retcount = l.size();//Null
		inv.addModel(lead_preview_friend,retjson);
		inv.addModel(lead_preview_friend_count, retcount);
		inv.addModel(lead_preview_stage,host.getStage());
		tc.endFinally();
		GuideLogger.printLog(" host:"+host.getId()+" ids:"+l.size()+" cache:" +cachecount+" flag:"+friend_srouce_flag);
	}
	private void doing(Invocation inv,Portal portal, User host, CacheManager cm) {
		addWindow(portal, "get_guideGetProfileFriends", "guideGetProfileFriends"); //搜索结果
	}

	@Override
	public AbstractLeadPageMapper getMapper(User host) {
		return mapper;
	}
	
	public static void main(String[] args) throws SecurityException, NoSuchMethodException {
		Method m = PreviewCommonController.class.getDeclaredMethod("doingAAF",Invocation.class, Portal.class,User.class,CacheManager.class);
		System.out.println(m.getName());
//		PreviewCommonController c = new PreviewCommonController();
//		c.doingAAF(null, null, null, null);
	}

    
}

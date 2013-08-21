package com.renren.sns.lead.dataproviders.fact;

import java.util.ArrayList;
import java.util.List;

import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.lead.dataproviders.AbstractLeadDataCallable;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveRelationsPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.BuddyByApplicantCacheAdapter;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheAccepterInfo;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheData;
import com.xiaonei.xce.buddybyapplicantcache.DefaultBuddyByApplicantCacheFactory;
import com.xiaonei.xce.offerfriends.DefaultOffer;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

public class LeadDataCallableFactory {
	private static final String KEY_PREVIEW_PROFILE = "k_p_p";
	private static final String KEY_PREVIEW_OFFERFRIEND = "k_p_o";
	private static final String KEY_MY_APPLY_LIST = "k_m_a_l";
	
	
	/**
	 * 我申请过谁
	 * @param host
	 * @return
	 */
	public static AbstractLeadDataCallable<List<Integer>> getMyApplyListCallable(final User host, final int fcount){ 
		AbstractLeadDataCallable<List<Integer>> ret = new AbstractLeadDataCallable<List<Integer>>(KEY_MY_APPLY_LIST, host.getId()){

			@Override
			protected List<Integer> doCall() throws Exception {
				List<Integer> list = new ArrayList<Integer>();
				BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData> _adapter = new BuddyByApplicantCacheAdapter<DefaultBuddyByApplicantCacheAccepterInfo, DefaultBuddyByApplicantCacheData>(new DefaultBuddyByApplicantCacheFactory());
				DefaultBuddyByApplicantCacheData _data = (DefaultBuddyByApplicantCacheData) _adapter.getBuddyByApplicantCacheData(host.getId(), 0, fcount);
				for (BuddyByApplicantCacheAccepterInfo el : _data.getAccepters()) {
					list.add(el.getAccepter());
				}
				
				return list;
			}
			
		};
		return ret;
	}
	
	/**
	 * 按资料推人
	 * @param host
	 * @param fcount
	 * @return
	 */
	public static AbstractLeadDataCallable<List<GuideIRelation>> getPreviewProfileCallable(final User host, final int fcount){
		AbstractLeadDataCallable<List<GuideIRelation>> ret = new AbstractLeadDataCallable<List<GuideIRelation>>(KEY_PREVIEW_PROFILE,host.getId()){
			@Override
			public List<GuideIRelation> doCall() throws Exception {
				List<GuideIRelation> list = GuideXFiveRelationsPreviewLogic.getInstance().getProfileResults(fcount<=0?48:fcount, host, -1);
				return list;
			}};
		return ret;
	}
	
	/**
	 * 推可能认识的人
	 * @param host
	 * @param fcount
	 * @return
	 */
	public static AbstractLeadDataCallable<List<Integer>> getPreviewOfferfriendCallable(final User host, final int fcount){
		
		AbstractLeadDataCallable<List<Integer>> ret = new AbstractLeadDataCallable<List<Integer>>(KEY_PREVIEW_OFFERFRIEND,host.getId()){
			@Override
			public List<Integer> doCall() throws Exception {
				GuideTimeCost tc = GuideTimeCost.begin();
				List<Integer> ids = new ArrayList<Integer>();
				try {
					OfferFriendsAdapter<DefaultOffer> adapter = OfferFriendsAdapter.getInstance();
					if(adapter!=null){
					    adapter.rebuild(host.getId());   
					    List<DefaultOffer> offers = adapter.getBestOfferByStage(host.getStage(), host.getId(), fcount<=0?48:fcount);
					    if (offers != null) {
					        for (int i = 0; i < offers.size(); i++) {
					            ids.add(offers.get(i).getUserId());
					        }
					    }
					}
				} catch (Exception e) {
					GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
				}
				tc.endFinally();
				return ids;
			}
		};
		
		return ret;
	}
	
}

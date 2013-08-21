package com.renren.sns.lead.dataproviders.impl;

import java.util.ArrayList;
import java.util.List;

import com.renren.sns.lead.dataproviders.AbstractLeadDataProvider;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.xce.offerfriends.DefaultOffer;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

public class PreviewOfferfriendProvider extends AbstractLeadDataProvider{

	private static final int RESULT_LIMIT = 45;

	@SuppressWarnings("unchecked")
	@Override
	protected <T>T doGetData(User host, long timeout,final Object ...param) {
		List<Integer> ids = new ArrayList<Integer>();
		try {
			OfferFriendsAdapter<DefaultOffer> adapter = OfferFriendsAdapter.getInstance();
			if(adapter!=null){
			    adapter.rebuild(host.getId());   
			    List<DefaultOffer> offers = adapter.getBestOfferByStage(host.getStage(), host.getId(), RESULT_LIMIT);                             
			    if (offers != null) {
			        for (int i = 0; i < offers.size(); i++) {
			            ids.add(offers.get(i).getUserId());
			        }
			    }
			}
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
		}
		GuideLogger.printLog(" host:"+host.getId()+" list:"+ids.size()+" RESULT_LIMIT:"+RESULT_LIMIT);
		return (T) ids;
	}


}

package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;

import com.xiaonei.sns.platform.core.opt.ice.IOfferFriendsAdapter;
import com.xiaonei.xce.offerfriends.DefaultOffer;
import com.xiaonei.xce.offerfriends.OfferFriendsAdapter;

/**
 * 
 * @author Michael(liyong@opi-corp.com)
 * @author tai.wang@opi-corp.com Sep 14, 2010 - 10:51:59 AM
 */
public class WOfferFriendsAdapter extends
		BaseUsersAdapter<OfferFriendsAdapter<DefaultOffer>> implements
		IOfferFriendsAdapter {
 
	protected WOfferFriendsAdapter() {

	}

	@Override
	public List<DefaultOffer> getBestOffer(int userId,  int limit) {
		return getAdapter().getBestOffer(userId, limit);
	}
	
	
	@Override
	protected String getServiceName() {
		return "friend finder replica";
	}

    @Override
    protected OfferFriendsAdapter<DefaultOffer> makeObject() {
        return OfferFriendsAdapter.getInstance();
    }


}

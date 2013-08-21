package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;

import com.xiaonei.sns.platform.core.opt.ice.IBuddyApplyCacheAdapterN;
import com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheDataN;
import com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheWithTotalN;
import com.xiaonei.xce.buddyapplycache.BuddyApplyCacheAdapterN;
import com.xiaonei.xce.buddyapplycache.BuddyApplyCacheFactoryN;

public class WBuddyApplyCacheAdapterN
		extends
		BaseUsersAdapter<BuddyApplyCacheAdapterN<SnsBuddyApplyCacheDataN, SnsBuddyApplyCacheWithTotalN>>
		implements
		IBuddyApplyCacheAdapterN<SnsBuddyApplyCacheDataN, SnsBuddyApplyCacheWithTotalN> {

	protected WBuddyApplyCacheAdapterN() {
	}

	@Override
	public void clear(int userId) {
		getAdapter().clear(userId);

	}

	@Override
	public List<SnsBuddyApplyCacheDataN> getBuddyApplyCache(int userId,
			int begin, int limit) {
		return getAdapter().getBuddyApplyCache(userId, begin, limit);
	}

	@Override
	public SnsBuddyApplyCacheWithTotalN getBuddyApplyCacheWithTotal(int userId,
			int begin, int limit) {
		return getAdapter().getBuddyApplyCacheWithTotal(userId, begin, limit);
	}

	@Override
	public int getHardLimit() {
		return getAdapter().getHardLimit();
	}

	@Override
	public void rebuildBuddyApplyCache(int userId) {
		getAdapter().rebuildBuddyApplyCache(userId);
	}

	@Override
	public void removeBuddyApplyCache(int userId, int applicant) {
		getAdapter().removeBuddyApplyCache(userId, applicant);
	}

	@Override
	protected String getServiceName() {
		return "buddy apply cache";
	}

	@Override
	protected BuddyApplyCacheAdapterN<SnsBuddyApplyCacheDataN, SnsBuddyApplyCacheWithTotalN> makeObject() {
		return new BuddyApplyCacheAdapterN<SnsBuddyApplyCacheDataN, SnsBuddyApplyCacheWithTotalN>(
				new BuddyApplyCacheFactoryN<SnsBuddyApplyCacheDataN, SnsBuddyApplyCacheWithTotalN>() {

					@Override
					public SnsBuddyApplyCacheDataN createBuddyApplyCacheDataN() {
						return new SnsBuddyApplyCacheDataN();
					}

					@Override
					public SnsBuddyApplyCacheWithTotalN createBuddyApplyCacheWithTotalN() {
						return new SnsBuddyApplyCacheWithTotalN();
					}
				});
	}
}

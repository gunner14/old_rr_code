package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.adapter.BuddyCoreAdapter;
import mop.hi.oce.domain.buddy.BuddyApply;
import mop.hi.oce.domain.buddy.BuddyApplyWithInfo;
import mop.hi.oce.domain.buddy.BuddyBlock;
import mop.hi.oce.domain.buddy.BuddyRelation;

import com.xiaonei.sns.platform.core.opt.ice.IBuddyCoreAdapter;

/**
 * 
 * @author Michael(liyong@opi-corp.com)
 * 
 */
public class WBuddyCoreAdapter extends BaseUsersAdapter<BuddyCoreAdapter>
		implements IBuddyCoreAdapter {

	protected WBuddyCoreAdapter() {
	}

	@Override
	public void acceptAllApply(int accepter) {
		getAdapter().acceptAllApply(accepter);
	}

	@Override
	public void acceptApply(BuddyApply request) {
		getAdapter().acceptApply(request);
	}

	@Override
	public void addApply(BuddyApplyWithInfo request, String ref) {
		getAdapter().addApply(request, ref);
	}

	@Override
	public void addApplyBatch(List<BuddyApplyWithInfo> requests, String ref) {
		getAdapter().addApplyBatch(requests, ref);
	}

	@Override
	public void addBlock(BuddyBlock block) {
		getAdapter().addBlock(block);
	}

	@Override
	public void addFriend(int fromId, int toId) {
		// getAdapter().addFriend(fromId, toId);
	}

	@Override
	public void addFriendDirectly(int fromId, int toId) {
		getAdapter().addFriendDirectly(fromId, toId);
	}

	@Override
	public void denyAllApply(int accepter) {
		getAdapter().denyAllApply(accepter);
	}

	@Override
	public void denyApply(BuddyApply request) {
		getAdapter().denyApply(request);
	}

	@Override
	public BuddyRelation getRelation(int fromId, int toId) {
		return getAdapter().getRelation(fromId, toId);
	}

	@Override
	public void removeBlock(BuddyBlock block) {
		getAdapter().removeBlock(block);
	}

	@Override
	public void removeFriend(BuddyRelation relation) {
		this.removeFriend(relation);
	}

	@Override
	public List<BuddyRelation> getApplyList(int hostId, int begin, int limit) {
		return getAdapter().getApplySeq(hostId, begin, limit);
	}

	@Override
	protected String getServiceName() {
		return "buddy core";
	}

	@Override
	protected BuddyCoreAdapter makeObject() {
		return AdapterFactory.getBuddyCoreAdapter();
	}

}

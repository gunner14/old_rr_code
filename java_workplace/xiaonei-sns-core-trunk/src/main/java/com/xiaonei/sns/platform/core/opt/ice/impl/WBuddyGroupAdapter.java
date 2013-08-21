package com.xiaonei.sns.platform.core.opt.ice.impl;

import java.util.List;
import java.util.Map;

import com.xiaonei.sns.platform.core.opt.ice.IBuddyGroupAdapter;
import com.xiaonei.xce.buddygroup.BuddyGroupAdapter;

/**
 * 
 * @author Michael(liyong@opi-corp.com)
 * 
 */
public class WBuddyGroupAdapter extends BaseUsersAdapter<BuddyGroupAdapter>
		implements IBuddyGroupAdapter {

	protected WBuddyGroupAdapter() {
	}

	@Override
	public void addBuddiesToGroup(int host, String groupName, int[] buddies) {
		getAdapter().addBuddiesToGroup(host, groupName, buddies);
	}

	@Override
	public void addBuddyToGroup(int host, String groupName, int buddy) {
		getAdapter().addBuddyToGroup(host, groupName, buddy);
	}

	@Override
	public void addGroup(int host, String groupName) {
		getAdapter().addGroup(host, groupName);
	}

	@Override
	public void changeBuddyGroups(int host, int buddy, String[] nameSeq) {
		getAdapter().changeBuddyGroups(host, buddy, nameSeq);
	}

	@Override
	public void cleanGroup(int host, String groupName) {
		getAdapter().cleanGroup(host, groupName);
	}

	@Override
	public Map<String, Integer> getAllGroupsBuddyCount(int host) {
		return getAdapter().getAllGroupsBuddyCount(host);
	}

	@Override
	public Map<Integer, List<String>> getBuddiedGroupsMap(int host) {
		return getAdapter().getBuddiedGroupsMap(host);
	}

	@Override
	public List<String> getBuddyGroups(int host, int buddy) {
		return getAdapter().getBuddyGroups(host, buddy);
	}

	@Override
	public List<Integer> getBuddyList(int host, String groupName, int begin,
			int limit) {
		return getAdapter().getBuddyList(host, groupName, begin, limit);
	}

	@Override
	public int getCountByGroup(int host, String groupName) {
		return getAdapter().getCountByGroup(host, groupName);
	}

	@Override
	public List<String> getGroupList(int host) {
		return getAdapter().getGroupList(host);
	}

	@Override
	public void removeBuddyFromGroup(int host, String groupName, int buddy) {
		getAdapter().removeBuddyFromGroup(host, groupName, buddy);
	}

	@Override
	public void removeGroup(int host, String groupName) {
		getAdapter().removeGroup(host, groupName);
	}

	@Override
	public void renameGroup(int host, String oldName, String newName) {
		getAdapter().renameGroup(host, oldName, newName);
	}

	@Override
	protected String getServiceName() {
		return "buddy group";
	}

	@Override
	protected BuddyGroupAdapter makeObject() {
		return BuddyGroupAdapter.getInstance();
	}

}

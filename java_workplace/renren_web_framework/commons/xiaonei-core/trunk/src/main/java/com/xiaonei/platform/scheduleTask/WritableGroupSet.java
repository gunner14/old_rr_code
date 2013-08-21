package com.xiaonei.platform.scheduleTask;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class WritableGroupSet {
	private Object[] sortedGroups;
	private ArrayList<Group> groups;
	private Random rand = new Random();
	
	public WritableGroupSet(ArrayList<Group> groups) {
		this.groups = groups;
		this.sortedGroups = groups.toArray();
		Arrays.sort(sortedGroups);
	}
	
	public int size() {
		return groups.size();
	}
	
	public synchronized Group getGroup(int rankLevel, int type) {
		ArrayList<Group> gs = new ArrayList<Group>();
		for (int i = 0; i < sortedGroups.length; ++i) {
			Group group = (Group)sortedGroups[i];
			if (group.getType() != type) {
				continue;
			}
			int rank = group.rank();
			if (rank <= rankLevel) {
				gs.add(group);
			} else if (rank > rankLevel) {
				break;
			}
		}
		if (!gs.isEmpty()) {
			return gs.get(rand.nextInt(gs.size()));
		} else {
			for (int i = 0; i < sortedGroups.length; ++i) {
				Group group = (Group)sortedGroups[i];
				if (group.getType() != type) {
					continue;
				}
				gs.add(group);
			}	
			if (!gs.isEmpty()) {
				return gs.get(rand.nextInt(gs.size()));	
			} else {
				return null;
			}
		}
	}
	
	public synchronized void rank(Object[] allGroups) {
		groups.clear();
		for (int i = 0; i < allGroups.length; ++i) {
			Group group = (Group)allGroups[i];
			if (group.isWritable()) {
				groups.add(group);
			}
		}
		sortedGroups = groups.toArray();
		Arrays.sort(sortedGroups);
		for (int i = 0; i < sortedGroups.length; ++i) {
			Group group = (Group)sortedGroups[i];
			System.err.println("#### Storage:WriteableGroupSet group[" + group.getName()
					+ "] writable sorted, load->" + group.getLoad());
		}
	}	
}

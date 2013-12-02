package com.renren.socialgraph;

public class BlockDataItem {
	public int userId;
	public int friendBlockId;
	
	public BlockDataItem(int userId, int friendBlockId) {
		this.userId = userId;
		this.friendBlockId = friendBlockId;
	}
}

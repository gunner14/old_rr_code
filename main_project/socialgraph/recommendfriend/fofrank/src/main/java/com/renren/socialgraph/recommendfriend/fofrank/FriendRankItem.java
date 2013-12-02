/**************************************************************************************
 *  Function: FriendRank ranking structure item 
 *  Developer: junwu.xiong@renren-inc.com
 *  Date: 2012-11-26 18:05
 *************************************************************************************/


package com.renren.socialgraph.recommendfriend.fofrank;

import org.apache.hadoop.hbase.util.Bytes;

public class FriendRankItem {	
	private byte[] friendId;
	private byte[] activeWeight;
	private byte[] staticWeight;
	public FriendRankItem() {
		this.setFriendId(new byte[Bytes.SIZEOF_INT]);
		this.setActiveWeight(new byte[Bytes.SIZEOF_FLOAT]);
		this.setStaticWeight(new byte[Bytes.SIZEOF_FLOAT]);
	}
	public byte[] getFriendId() {
		return friendId;
	}
	public void setFriendId(byte[] friendId) {
		this.friendId = friendId;
	}
	public byte[] getStaticWeight() {
		return staticWeight;
	}
	public void setStaticWeight(byte[] staticWeight) {
		this.staticWeight = staticWeight;
	}
	public byte[] getActiveWeight() {
		return activeWeight;
	}
	public void setActiveWeight(byte[] activeWeight) {
		this.activeWeight = activeWeight;
	}
}

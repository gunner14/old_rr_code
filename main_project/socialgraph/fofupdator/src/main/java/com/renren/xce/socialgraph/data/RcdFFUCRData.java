/*
 * Function: SocialGraph friends recommendation of friend cluster algorithm data items, 
 * including hostId, fofId, rank, mutualFriendNumber, mutualFriendIds 
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-12-07
 */

package com.renren.xce.socialgraph.data;

import java.util.ArrayList;
import java.util.List;

public class RcdFFUCRData {

	private int hostId = 0;
	private int rcdFriendId = 0;
	private int clusterSize = 0;
	private List<Integer> clusterMemberList = null; // cluster member list to be
													// displayed
	private int explanFlag = 0; // recommendation explanation mapping flag

	public RcdFFUCRData() {
		this.clusterMemberList = new ArrayList<Integer>();
	}

	public int getHostId() {
		return this.hostId;
	}

	void setHostId(int hostId) {
		this.hostId = hostId;
	}

	public int getRcdFriendId() {
		return rcdFriendId;
	}

	public void setRcdFriendId(int rcdFriendId) {
		this.rcdFriendId = rcdFriendId;
	}

	public int getClusterSize() {
		return clusterSize;
	}

	public void setClusterSize(int clusterSize) {
		this.clusterSize = clusterSize;
	}

	public List<Integer> getClusterMemberList() {
		return clusterMemberList;
	}

	public void setClusterMemberList(List<Integer> clusterMemberList) {
		this.clusterMemberList = clusterMemberList;
	}

	public int getExplanFlag() {
		return explanFlag;
	}

	public void setExplanFlag(int explanFlag) {
		this.explanFlag = explanFlag;
	}
}

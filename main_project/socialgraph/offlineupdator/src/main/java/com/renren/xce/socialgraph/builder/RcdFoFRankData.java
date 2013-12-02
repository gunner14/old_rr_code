/*
 * Function: SocialGraph group SocialGraphFoF friends recommendation service data items
 * accessing interfaces, including hostId, fofId, rank, mutualFriendNumber, mutualFriendIds 
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-06-25
 */

package com.renren.xce.socialgraph.builder;

import java.util.ArrayList;
import java.util.List;

public class RcdFoFRankData {	

	private int hostId = 0;
	private int fofId = 0; 
	private float rank = 0;   //ranking value of the fofId recocommended to hostId
	private int mutualFriendNumber = 0;
    private List<Integer> mutualFriendIdList = null;
    
    public RcdFoFRankData() {
    	this.mutualFriendIdList = new ArrayList<Integer>();
    }
    
    public int getHostId() {
    	return this.hostId;
    }
    
    void setHostId(int hostId) {
    	this.hostId = hostId;
    }
    
    public int getFoFId() {
    	return this.fofId;
    }
    
    void setFoFId(int fofId) {
    	this.fofId = fofId;
    }
    
    public float getRank() {
    	return this.rank;
    }
    
    void setRank(float rank) {
    	this.rank = rank;
    }
    
    public int getMutualFriendNumber() {
    	return this.mutualFriendNumber;
    }
    
    void setMutualFriendNumber(int mutualFriendNumber) {
    	this.mutualFriendNumber = mutualFriendNumber;
    }
    
    public List<Integer> getmutualFriendIdList() {
    	return this.mutualFriendIdList;
    }
    
    void setmutualFriendIdList(List<Integer> mutualFriendIds) {
    	for (Integer id : mutualFriendIds) {
    		if (id == 0) {
    			break;
    		}
    		this.mutualFriendIdList.add(id); 
    	}
    }

//	public int compareTo(Object o) {
//		// TODO Auto-generated method stub
//		RcdFoFRankData rcdFoFRankData = (RcdFoFRankData)o;
//		return ((this.mutualFriendNumber < rcdFoFRankData.mutualFriendNumber) ? 1 : 0);
//	}

}

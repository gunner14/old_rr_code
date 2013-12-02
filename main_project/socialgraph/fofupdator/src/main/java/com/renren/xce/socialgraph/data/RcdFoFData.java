/*
 * Function: SocialGraph group SocialGraphFoF friends recommendation service data items
 * accessing interfaces, including hostId, fofId, rank, mutualFriendNumber, mutualFriendIds 
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-06-25
 */

package com.renren.xce.socialgraph.data;

import java.util.ArrayList;
import java.util.List;

public class RcdFoFData {	

	private int hostId = 0;
	private int fofId = 0; 
	private float rank = 0;   //ranking value of the fofId recocommended to hostId
	private int mutualFriendNumber = 0;
    private List<Integer> mutualFriendIdList = null;
    
    public RcdFoFData() {
    	this.mutualFriendIdList = new ArrayList<Integer>();
    }
    
    public int getHostId() {
    	return this.hostId;
    }
    
    public void setHostId(int hostId) {
    	this.hostId = hostId;
    }
    
    public int getFoFId() {
    	return this.fofId;
    }
    
    public void setFoFId(int fofId) {
    	this.fofId = fofId;
    }
    
    public float getRank() {
    	return this.rank;
    }
    
    public void setRank(float rank) {
    	this.rank = rank;
    }
    
    public int getMutualFriendNumber() {
    	return this.mutualFriendNumber;
    }
    
    public void setMutualFriendNumber(int mutualFriendNumber) {
    	this.mutualFriendNumber = mutualFriendNumber;
    }
    
    public List<Integer> getmutualFriendIdList() {
    	return this.mutualFriendIdList;
    }
    
    public void setmutualFriendIdList(List<Integer> mutualFriendIds) {
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

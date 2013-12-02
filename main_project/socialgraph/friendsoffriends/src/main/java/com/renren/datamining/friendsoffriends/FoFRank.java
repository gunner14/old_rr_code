/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:      junwu.xiong@renren-inc.com
 * Function:   fof ranking data structure store result computing by the facebook ranking model 
 * Date:  	   2012-04-18
***************************************************************************************************/

package com.renren.datamining.friendsoffriends;


public class FoFRank implements Comparable<Object>{	
	public int fofId;
	public float rank;
	public int mutualFriendNumber;
	
	// Change to int from Integer to avoid box/unbox
    public int[] mutualFriendIdList = new int[FoFRankModel.kDispMutFrdNum];
    public int mutualFriendIdListSize;
    
	public int compareTo(Object o) {
		final FoFRank ffr = (FoFRank)o;
		return ((this.rank < ffr.rank) ? 1 : 0);
	}
	
	public void addtoMutalFriendIdList(int x){
	  mutualFriendIdList[mutualFriendIdListSize++] = x;
	}
}

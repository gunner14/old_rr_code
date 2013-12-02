/**************************************************************************************************
 * Developer: xiongjunwu
 * Email:     junwu.xiong@renren-inc.com
 * Date:      2012-04-16
 * Function:  Friends recommendation service ranking formula data items, including hostId, fofId, weight
 * mutualFriendNumber and mutualFriendIds 
***************************************************************************************************/

package com.renren.datamining.friendsoffriends;

public class FoFRankFormula {
	public int fiId;  		//host's friend i' Id
	public int fiFrdNum;    //friend i's friend number
	public int timeHostFi;  //adding friends time between host and friend i
	public int timeFiFoF;   //adding friends time between friend i and host'f friend of friend fof
	public FoFRankFormula() {

	}
}

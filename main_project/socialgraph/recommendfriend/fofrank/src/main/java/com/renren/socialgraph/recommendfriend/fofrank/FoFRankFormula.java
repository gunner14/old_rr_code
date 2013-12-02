/**************************************************************************************************
 * Developer: xiongjunwu
 * Email:     junwu.xiong@renren-inc.com
 * Date:      2012-12-02
 * Function:  Friends recommendation service ranking formula data items, userId, friendId, adding friends time 
 * among them, both static and active ranking values among the user, recommend friend and fof 
***************************************************************************************************/

package com.renren.socialgraph.recommendfriend.fofrank;

public class FoFRankFormula {
	public int fiId;  		//host's friend i' Id
	public int fiFrdNum;    //friend i's friend number
	public int timeHostFi;  //adding friends time between host and friend i
	public int timeFiFoF;   //adding friends time between friend i and host'f friend of friend fof
	
	public float rankHostFi;//relation ranking value between host and friend i
	public float rankFiFoF; //relation ranking value between friend i and host'f friend of friend fof
	
	public FoFRankFormula() {

	}
}

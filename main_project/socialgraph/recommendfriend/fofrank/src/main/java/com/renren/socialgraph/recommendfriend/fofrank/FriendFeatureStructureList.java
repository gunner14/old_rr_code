/**************************************************************************************************
 * Function:   Sequence File key-value writable IO data structrure
 * Developer:  Xiongjunwu
 * Email:  	   junwu.xiong@renren-inc.com
 * Date:  2012-04-16
 *  Sequence file record format:
 *  key                                               value
 *  hostId         friendNumber IdTimeStructureList(IdTimeStructure: friendId time fofNumber friendIdList)
*********************************************************************************************************************/
 

package com.renren.socialgraph.recommendfriend.fofrank;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.io.Writable;

public class FriendFeatureStructureList implements Writable{

	private List<FriendFeatureStructure> friendFeatureStructureList;
	private int friendNumber;
   
	public FriendFeatureStructureList(List<FriendFeatureStructure> friendFeatureStructureList, int friendNumber) {
		this.friendFeatureStructureList = friendFeatureStructureList;
		this.friendNumber = friendNumber;
	}
	
	public FriendFeatureStructureList(){
	  
	}
	
	public void write(DataOutput out) throws IOException {
		out.writeInt(this.friendNumber);
		for (int i = 0; i < this.friendNumber; ++i) {
			FriendFeatureStructure item = friendFeatureStructureList.get(i);		
			item.write(out);
		}		
	}

	public void readFields(DataInput in) throws IOException {
		friendFeatureStructureList = new ArrayList<FriendFeatureStructure>();
		this.friendNumber = in.readInt();
		for (int i= 0; i < this.friendNumber; ++i) {
			FriendFeatureStructure item = new FriendFeatureStructure();
			item.readFields(in);
			friendFeatureStructureList.add(item);			
 		}				
	}

	public List<FriendFeatureStructure> getFriendFeatureStructureList() {
		return friendFeatureStructureList;
	}

	public void setFriendFeatureStructureList(
			List<FriendFeatureStructure> friendFeatureStructureList) {
		this.friendFeatureStructureList = friendFeatureStructureList;
	}

	public int getFriendNumber() {
		return friendNumber;
	}

	public void setFriendNumber(int friendNumber) {
		this.friendNumber = friendNumber;
	}	
} 
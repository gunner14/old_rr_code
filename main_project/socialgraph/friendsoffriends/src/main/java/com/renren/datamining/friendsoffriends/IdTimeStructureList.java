/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  	   junwu.xiong@renren-inc.com
 * Function:   Sequence File key-value writable IO data structrure
 * Date:  2012-04-16
 *  Sequence file record format:
 *  key                                               value
 *  hostId         friendNumber IdTimeStructureList(IdTimeStructure: friendId time fofNumber friendIdList)
*********************************************************************************************************************/
 

package com.renren.datamining.friendsoffriends;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.hadoop.io.Writable;

public class IdTimeStructureList implements Writable{

	private List<IdTimeStructure> idTimeStructureList;
	private int friendNumber;
   
	public IdTimeStructureList(List<IdTimeStructure> idTimeStructureList, int friendNumber) {
		this.idTimeStructureList = idTimeStructureList;
		this.friendNumber = friendNumber;
	}
	
	public IdTimeStructureList(){
	  
	}
	
	public void write(DataOutput out) throws IOException {
		out.writeInt(this.friendNumber);
		for (int i = 0; i < this.friendNumber; ++i) {
			IdTimeStructure item = idTimeStructureList.get(i);		
			item.write(out);
		}		
	}

	public void readFields(DataInput in) throws IOException {
		idTimeStructureList = new ArrayList<IdTimeStructure>();
		this.friendNumber = in.readInt();
		for (int i= 0; i < this.friendNumber; ++i) {
			IdTimeStructure item = new IdTimeStructure();
			item.readFields(in);
			idTimeStructureList.add(item);			
 		}				
	}

	public List<IdTimeStructure> getIdTimeStructureList() {
		return idTimeStructureList;
	}

	public void setIdTimeStructureList(List<IdTimeStructure> idTimeStructureList) {
		this.idTimeStructureList = idTimeStructureList;
	}

	public int getFriendNumber() {
		return friendNumber;
	}

	public void setFriendNumber(int friendNumber) {
		this.friendNumber = friendNumber;
	}

	
} 
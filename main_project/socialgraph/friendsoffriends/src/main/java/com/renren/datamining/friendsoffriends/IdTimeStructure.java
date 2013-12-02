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

import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.Writable;

public class IdTimeStructure implements Writable {
	private byte[] friendId;
	private byte[] time;	
	private int fofNumber;
	private byte[] bytes;
	
    public IdTimeStructure() {
	}    
	
    public IdTimeStructure(final int fofNumber) {	
		this.friendId = new byte[Bytes.SIZEOF_INT];
		this.time = new byte[Bytes.SIZEOF_INT];
		this.fofNumber = fofNumber;
		this.bytes = new byte[fofNumber * 2];
	}
    
    public IdTimeStructure(final IdTimeStructure item) {
		this(item.friendId, item.time, item.fofNumber, item.bytes);
	}
	
	public IdTimeStructure(final byte[] friendId, final byte[] time, 
			final int fofNumber, final byte[] bytes) {	
		this.friendId = friendId;
		this.time = time;
		this.fofNumber = fofNumber;
		this.bytes = bytes;
	}
   	
	public void write(final DataOutput out) throws IOException {
		out.write(this.friendId);
		out.write(this.time);
		out.writeInt(this.fofNumber);
		out.write(this.bytes, 0, this.bytes.length); 	
	}
    
	public void readFields(final DataInput in) throws IOException {
		this.friendId = new byte[Bytes.SIZEOF_INT];
		in.readFully(this.friendId, 0, this.friendId.length);
		this.time = new byte[Bytes.SIZEOF_INT];		
		in.readFully(this.time, 0, this.time.length);
		this.fofNumber = in.readInt();
		this.bytes = new byte[this.fofNumber * 2 * Bytes.SIZEOF_INT];
		in.readFully(this.bytes, 0, this.bytes.length); 
 	}


	public byte[] getFriendId() {
		return friendId;
	}

	public void setFriendId(byte[] friendId) {
		this.friendId = friendId;
	}

	public byte[] getTime() {
		return time;
	}

	public void setTime(byte[] time) {
		this.time = time;
	}

	public byte[] getBytes() {
		return bytes;
	}

	public void setBytes(byte[] bytes) {
		this.bytes = bytes;
	}

	public int getFofNumber() {
		return fofNumber;
	}

	public void setFofNumber(int fofNumber) {
		this.fofNumber = fofNumber;
	}
}

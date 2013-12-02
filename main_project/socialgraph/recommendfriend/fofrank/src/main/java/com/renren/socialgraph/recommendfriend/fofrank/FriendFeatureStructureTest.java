/**************************************************************************************************
 * Function:   Unit test case of FriendFeatureStructure class 
 * Developer:  Xiongjunwu
 * Email:  	   junwu.xiong@renren-inc.com
 * Date:  2012-12-10
*********************************************************************************************************************/
package com.renren.socialgraph.recommendfriend.fofrank;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Writable;

public class FriendFeatureStructureTest implements Writable {
	private static org.apache.commons.logging.Log logger = org.apache.commons.logging.LogFactory
			.getLog(FriendFeatureStructureTest.class);	
	private byte[] friendId;   //int raw format 
	private byte[] time;	   //int raw format 
	private byte [] staticWeight; //float raw format 
	private byte [] activeWeight; //float raw format 
	private int friendFriendNumber;
	private byte[] bytes;
	private final int kFriendFeatureUnitSize = 2 * (Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT); 
    public FriendFeatureStructureTest() {
	}    
	
    public FriendFeatureStructureTest(final int friendFriendNumber) {	
		this.friendId = new byte[Bytes.SIZEOF_INT];
		this.time = new byte[Bytes.SIZEOF_INT];
		this.staticWeight = new byte[Bytes.SIZEOF_FLOAT];
		this.activeWeight = new byte[Bytes.SIZEOF_FLOAT];
		this.friendFriendNumber = friendFriendNumber;
		this.bytes = new byte[friendFriendNumber * kFriendFeatureUnitSize];
	}
    
    public FriendFeatureStructureTest(final FriendFeatureStructureTest item) {
		this(item.friendId, item.time, item.staticWeight, item.activeWeight, item.friendFriendNumber, item.bytes);
	}

    public FriendFeatureStructureTest(final byte[] friendId, final byte[] time, final byte[] staticWeight,
			final byte[] activeWeight, final int friendFriendNumber, final byte[] bytes) {	
		this.friendId = friendId;
		this.time = time;
		this.staticWeight = staticWeight;
		this.activeWeight = activeWeight;
		this.friendFriendNumber = friendFriendNumber;
		this.bytes = bytes;
	}
   	
	public void write(final DataOutput out) throws IOException {
		out.write(this.friendId);
		out.write(this.time);
		out.write(this.staticWeight);
		out.write(activeWeight);
		out.writeInt(this.friendFriendNumber);
		out.write(this.bytes, 0, this.bytes.length); 			
		logger.info("writer: hostId " + Bytes.toInt(this.friendId) + " time " + Bytes.toInt(this.time) + 
				" staticWeight " + Bytes.toFloat(this.staticWeight) + " activeWeight " + Bytes.toFloat(this.activeWeight));
		int friendNumber = this.bytes.length / kFriendFeatureUnitSize;
		for (int i = 0; i < friendNumber; ++i) {
			byte[] friendId = new byte[Bytes.SIZEOF_INT];
			byte[] time = new byte[Bytes.SIZEOF_INT];  
			byte[] staticWeight = new byte[Bytes.SIZEOF_FLOAT];
			byte[] activeWeight = new byte[Bytes.SIZEOF_FLOAT];
			System.arraycopy(bytes, i * kFriendFeatureUnitSize, friendId, 0, Bytes.SIZEOF_INT);
			System.arraycopy(bytes, i * kFriendFeatureUnitSize + Bytes.SIZEOF_INT, time, 0, Bytes.SIZEOF_INT);
			System.arraycopy(bytes, i * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT, staticWeight, 0, Bytes.SIZEOF_FLOAT);
			System.arraycopy(bytes, i * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT, activeWeight, 0, 
					Bytes.SIZEOF_FLOAT);	
			logger.info("writer: friendId " + Bytes.toInt(friendId) + " time " + Bytes.toInt(time) + 
					" staticWeight " + Bytes.toFloat(staticWeight) + " activeWeight " + Bytes.toFloat(activeWeight));
		}				
	}
    
	public void readFields(final DataInput in) throws IOException {
		this.friendId = new byte[Bytes.SIZEOF_INT];
		in.readFully(this.friendId, 0, this.friendId.length);
		this.time = new byte[Bytes.SIZEOF_INT];		
		in.readFully(this.time, 0, this.time.length);
		this.staticWeight = new byte[Bytes.SIZEOF_FLOAT];
		in.readFully(this.staticWeight, 0, this.staticWeight.length);
		this.activeWeight = new byte[Bytes.SIZEOF_FLOAT];
		in.readFully(this.activeWeight, 0, this.activeWeight.length);
		this.friendFriendNumber = in.readInt();
		this.bytes = new byte[this.friendFriendNumber * 2 * (Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT)];
		in.readFully(this.bytes, 0, this.bytes.length); 
		
		logger.info("reader: hostId " + Bytes.toInt(this.friendId) + " time " + Bytes.toInt(this.time) + 
				" staticWeight " + Bytes.toFloat(this.staticWeight) + " activeWeight " + Bytes.toFloat(this.activeWeight));
		int friendNumber = this.bytes.length / kFriendFeatureUnitSize;
		for (int i = 0; i < friendNumber; ++i) {
			byte[] friendId = new byte[Bytes.SIZEOF_INT];
			byte[] time = new byte[Bytes.SIZEOF_INT];  
			byte[] staticWeight = new byte[Bytes.SIZEOF_FLOAT];
			byte[] activeWeight = new byte[Bytes.SIZEOF_FLOAT];
			System.arraycopy(bytes, i * kFriendFeatureUnitSize, friendId, 0, Bytes.SIZEOF_INT);
			System.arraycopy(bytes, i * kFriendFeatureUnitSize + Bytes.SIZEOF_INT, time, 0, Bytes.SIZEOF_INT);
			System.arraycopy(bytes, i * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT, staticWeight, 0, Bytes.SIZEOF_FLOAT);
			System.arraycopy(bytes, i * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT, activeWeight, 0, 
					Bytes.SIZEOF_FLOAT);	
			logger.info("reader: friendId " + Bytes.toInt(friendId) + " time " + Bytes.toInt(time) + 
					" staticWeight " + Bytes.toFloat(staticWeight) + " activeWeight " + Bytes.toFloat(activeWeight));
		}			
		
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

	public int getFriendFriendNumber() {
		return friendFriendNumber;
	}

	public void setFriendFriendNumber(int friendFriendNumber) {
		this.friendFriendNumber = friendFriendNumber;
	}
}

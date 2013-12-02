package com.xiaonei.xce.socialgraph.puk;

import java.util.ArrayList;
import java.util.List;

import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;

/**
 * Struct used by PukRecommendAdapter
 * @author Administrator
 */
public class PukFriend {
	private int userId;
	private int shareCount;
	private String type;
	private String schoolName = "";
	List<Integer> shares = new ArrayList<Integer>();
	
	public PukFriend(Item comm) {
		this.userId = comm.getId();
		this.type = comm.getMessage();
		if (comm.getMessagesCount() == 1)		//if count of messages is 1, the value in position 0 is schoolName
			this.schoolName = comm.getMessagesList().get(0);
		if (comm.getFieldsCount() > 0) {
			this.shareCount = comm.getFields(0);		//if fieldscount > 0, the value stored in position 0 is shareCount
			for (int i = 1; i < comm.getFieldsCount(); ++i) {
				this.shares.add(comm.getFields(i));
			}
		}
	}
	/**
	 * Get Userid
	 * @return
	 */
	public int getUserId() {
		return userId;
	}
	
	/**
	 * Get number of common friends
	 * @return
	 */
	public int getShareCount() {
		return shareCount;
	}
	
	/**
	 * Get common friends
	 * @return
	 */
	public List<Integer> getShares() {
		return shares;
	}
	
	/**
	 * Get type of Puk
	 * @return
	 */
	public String getType() {
		return type;
	}
	
	/**
	 * Get School Name of Puk
	 * @return
	 */
	public String getSchoolName() {
		return schoolName;
	}
}

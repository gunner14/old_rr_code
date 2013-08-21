/**
 * xiaonei-reg
 * com.xiaonei.reg.common.users
 * UserInfoIdQueue.java
 * Sep 11, 2009 - 7:09:57 PM
 */
package com.xiaonei.reg.common.users;

import java.util.concurrent.ConcurrentLinkedQueue;

/**
 * UserInfoIdQueue
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 11, 2009 - 7:09:57 PM
 * 
 */
public class UserInfoIdQueue extends ConcurrentLinkedQueue<Integer> {

	private static final long serialVersionUID = -8121445807888889197L;

	public static final int NO_POP_ID = -1;
	private static int maxSize = 1000;
	private static int minSize = 0;

	private int size;
	private int popUserInfoId = NO_POP_ID;

	public UserInfoIdQueue(int size) {
		if (size > maxSize) {
			size = maxSize;
		}
		if (size < minSize) {
			size = minSize;
		}
		this.size = size;
	}

	@Override
	public boolean add(Integer userInfoId) {
		popUserInfoId = NO_POP_ID;
		for (;;) {
			if(0 >= size){
				break;
			}
			if (this.size() >= size) {
				if (!this.isEmpty()) {
					popUserInfoId = this.remove();
				}
			}else{
				break;
			}
		}
		return super.add(userInfoId);
	}

	public int getPopUserInfoId() {
		int id = popUserInfoId;
		popUserInfoId = NO_POP_ID;
		return id;
	}
}

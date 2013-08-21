/**
 * 
 */
package com.xiaonei.platform.core.model;

import java.util.List;

import mop.hi.oce.domain.model.FriendShares;

/**
 * @author lookis
 * 
 */
public class WFriendShares extends FriendShares {
	private int count;
	private List<WFriendShare> shares;

	/**
	 * @return the count
	 */
	public int getCount() {
		return count;
	}

	/**
	 * @return the shares
	 */
	public List<WFriendShare> getShares() {
		return shares;
	}

	/**
	 * @param count
	 *            the count to set
	 */
	public void setCount(final int count) {
		this.count = count;
	}

	/**
	 * @param shares
	 *            the shares to set
	 */
	public void setShares(final List<WFriendShare> shares) {
		this.shares = shares;
	}
}

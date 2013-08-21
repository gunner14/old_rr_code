package com.xiaonei.reg.register.pojo;

import com.xiaonei.platform.core.model.User;

/**
 * @author wang-tai
 */
public class RegUserConfigPojo {

	private User user;

	private int privacyHomepage;

	public User getUser() {
		return user;
	}

	public void setUser(User user) {
		this.user = user;
	}

	public int getPrivacyHomepage() {
		return privacyHomepage;
	}

	public void setPrivacyHomepage(int privacyHomepage) {
		this.privacyHomepage = privacyHomepage;
	}

	/**
	 * @param userId
	 * @param userName
	 * @param privacyHomepage
	 * @deprecated
	 */
	public RegUserConfigPojo(User user, int privacyHomepage) {
		this.user = user;
		this.privacyHomepage = privacyHomepage;
	}

	/**
	 * @param userId
	 * @param userName
	 */
	public RegUserConfigPojo(User user) {
		this.user = user;
	}

	@Override
	public String toString() {
		return super.toString() + this.user + this.privacyHomepage;
	}

}

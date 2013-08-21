package com.xiaonei.platform.core.userswitch;

/**
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-6-14 下午12:38:23
 */
class ControlCondition {

	/**
	 * 除数
	 */
	private int mod;

	/**
	 * 余数
	 */
	private int[] remainders;

	/**
	 * @param con
	 *            控制条件，格式为"10:3,5"，表示除以10余3或者5
	 */
	ControlCondition(String con) {
		String[] ss = con.split(":");
		mod = Integer.parseInt(ss[0]);
		String[] rs = ss[1].split(",");
		remainders = new int[rs.length];
		for (int i = 0; i < rs.length; i++) {
			remainders[i] = Integer.parseInt(rs[i]);
		}
	}

	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(mod);
		sb.append(":");
		for (int r : remainders) {
			sb.append(r);
			sb.append(",");
		}
		if (sb.charAt(sb.length() - 1) == ',') {
			sb.deleteCharAt(sb.length() - 1);
		}
		return sb.toString();
	}

	public int getMod() {
		return mod;
	}

	public void setMod(int mod) {
		this.mod = mod;
	}

	public int[] getRemainders() {
		return remainders;
	}

	public void setRemainders(int[] remainders) {
		this.remainders = remainders;
	}

}

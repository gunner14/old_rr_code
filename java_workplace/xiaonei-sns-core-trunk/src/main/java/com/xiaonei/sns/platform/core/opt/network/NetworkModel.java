package com.xiaonei.sns.platform.core.opt.network;

import java.io.Serializable;

/**
 * NetworkModel <br>
 * network model
 * 
 * @author tai.wang@opi-corp.com Jul 19, 2010 - 11:05:40 AM
 */
public class NetworkModel implements Serializable {

	private static final long serialVersionUID = -5781006166387415055L;
	/** 用户id */
	private int userId;
	/** 网络id */
	private int typeId;
	/** 网络类型描述 */
	private String type;
	/** 网络名 */
	private String name;

	public int getUserId() {
		return userId;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}

	public int getTypeId() {
		return typeId;
	}

	public void setTypeId(int typeId) {
		this.typeId = typeId;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	@Override
	public String toString() {
		return "NetworkModel [name=" + name + ", type=" + type + ", typeId="
				+ typeId + ", userId=" + userId + "]";
	}

}

package com.xiaonei.platform.core.model;

import java.io.Serializable;

public class Network implements Serializable {
	/**
     * 
     */
	private static final long serialVersionUID = -2746683598389553645L;
	private int id;
	private String name;
	private int tribeId;
	private int type;

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public int getTribeId() {
		return tribeId;
	}

	public void setTribeId(int tribeId) {
		this.tribeId = tribeId;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String toString() {
		return "{id:" + id + ",name:" + name + "}";
	}

	@Override
	public boolean equals(Object obj) {
		if (!(obj instanceof Network))
			return false;
		Network to = (Network) obj;
		return ((this.id != 0 && this.id == to.id) || ((this.name != null) && this.name
				.equalsIgnoreCase(to.name)));
	}
}

package com.xiaonei.platform.taglib.model;

import java.io.Serializable;

/**
 * 表taglib_info对应的model对象
 * @author lifs
 *
 */
public class TaglibInfo implements Serializable{
	
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -5191913418367437047L;

	private int id;
	
	private String name;

	
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
}

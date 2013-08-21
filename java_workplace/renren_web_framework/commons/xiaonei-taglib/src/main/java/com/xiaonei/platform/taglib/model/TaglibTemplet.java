package com.xiaonei.platform.taglib.model;

import java.io.Serializable;


public class TaglibTemplet implements Serializable{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 7575286361382954558L;

	private int id;
	private int tagId;
	private String templet;
	private String alias;
	private String description;
	private String cutline;
	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getTagId() {
		return tagId;
	}

	public void setTagId(int tagId) {
		this.tagId = tagId;
	}

	public String getTemplet() {
		return templet;
	}

	public void setTemplet(String templet) {
		this.templet = templet;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getCutline() {
		return cutline;
	}

	public void setCutline(String cutline) {
		this.cutline = cutline;
	}

	public String getAlias() {
		return alias;
	}

	public void setAlias(String alias) {
		this.alias = alias;
	}

	@Override
	public String toString() {
		
		return templet;
	}
	
}

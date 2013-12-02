package com.xiaonei.xce.socialgraph.sortbycluster;

import java.util.ArrayList;
import java.util.List;

import scala.reflect.generic.Trees.This;
import scala.runtime.Int;

public class CommonInfo {
	private List<Integer> rcdIds = null;
	private int searchResultSize = 0;
	private String searchKey = null;
	
	public static String workType = "Workplace";
	public static String univType = "University";
	public static String highType = "Highschool";
	public static String juniType = "JuniorHighSchool";
	public static String elemType = "ElementarySchool";
	
	public CommonInfo() {
	}
	
	public CommonInfo(String key, List<Integer> rcdIds, int searchSize) {
		this.rcdIds = rcdIds;
		this.searchResultSize = searchSize;
		this.searchKey = key;
	}
	
	public List<Integer> getRcdIds() {
		return this.rcdIds;
	}
	
	public int getSearchResultSize() {
		return this.searchResultSize;
	}
	
	public String getSearchKey() {
		return this.searchKey;
	}
	
	public void setRcdIds(List<Integer> rcdIds) {
		this.rcdIds = rcdIds;
	}
	public void setSearchResultSize(int size) {
		this.searchResultSize = size;
	}
	public void setSearchKey(String key) {
		this.searchKey = key;
	}
	public String toSting() {
		StringBuffer sb = new StringBuffer();
		sb.append(searchKey).append(" ");
		sb.append("rcdIds size:").append(rcdIds.size());
		sb.append(" rcdIds: ");
		for(int id : rcdIds) {
			sb.append(id).append(" ");
		}
		sb.append("searchSize: ").append(searchResultSize);
		sb.append("\n");
		return sb.toString();
	}
}

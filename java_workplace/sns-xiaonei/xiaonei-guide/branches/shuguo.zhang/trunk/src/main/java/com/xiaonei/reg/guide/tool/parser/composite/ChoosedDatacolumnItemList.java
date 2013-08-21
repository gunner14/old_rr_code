package com.xiaonei.reg.guide.tool.parser.composite;

import java.util.ArrayList;

public class ChoosedDatacolumnItemList {
	private ArrayList<ChoosedDatacolumnItem> itemlist = new ArrayList<ChoosedDatacolumnItem>();
	public void add(ChoosedDatacolumnItem item){
		if(!contains(item)){
			itemlist.add(item);
			item.setContainer(this);
		}
	}
	
	public boolean contains(ChoosedDatacolumnItem item){
	
		return false;
	}
	
	public Object [] toArray(){
		return itemlist.toArray();
	}
	
	public void remove(ChoosedDatacolumnItem item){
		itemlist.remove(item);
	}

	public ArrayList<ChoosedDatacolumnItem> getItemlist() {
		return itemlist;
	}

	public void setItemlist(ArrayList<ChoosedDatacolumnItem> itemlist) {
		this.itemlist = itemlist;
	}
}

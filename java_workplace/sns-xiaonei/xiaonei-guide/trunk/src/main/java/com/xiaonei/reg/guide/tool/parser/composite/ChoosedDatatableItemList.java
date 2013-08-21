package com.xiaonei.reg.guide.tool.parser.composite;

import java.util.ArrayList;

public class ChoosedDatatableItemList {
	private ArrayList<ChoosedDatatableItem> itemlist = new ArrayList<ChoosedDatatableItem>();
	public void add(ChoosedDatatableItem item){
		itemlist.add(item);
		item.setContainer(this);
	}
	
	public void remove(ChoosedDatatableItem item){
		itemlist.remove(item);
	}
	public void removeAll(){
		itemlist.clear();
	}
	public boolean contains(ChoosedDatatableItem item){
		for(ChoosedDatatableItem curItem : itemlist){
			if(!item.getAliasname().equals("") && item.getAliasname().equals(curItem.getAliasname())){
				return true;
			}
//			if(item.getDatatable().getName().equals(curItem.getDatatable().getName())){
//				return true;
//			}
		}
		return false;
	}
	
	public boolean containsAliasname(String name){
		for(ChoosedDatatableItem curItem : itemlist){
			if(!name.equals("") && name.equals(curItem.getAliasname())){
				return true;
			}
		}
		return false;
	}
	
	public Object [] toArray(){
		return itemlist.toArray();
	}
	
	public void doChange(){
		
	}

	public ArrayList<ChoosedDatatableItem> getItemlist() {
		return itemlist;
	}
}

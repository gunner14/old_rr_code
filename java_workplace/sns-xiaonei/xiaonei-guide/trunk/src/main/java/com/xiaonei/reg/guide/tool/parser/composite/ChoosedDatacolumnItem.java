package com.xiaonei.reg.guide.tool.parser.composite;

import com.xiaonei.reg.guide.tool.model.Datacolumn;

public class ChoosedDatacolumnItem  {
	
	
	private ChoosedDatatableItem chooseddatatableitem;
	private Datacolumn datacolumn;
	private String aliasname;
	private ChoosedDatacolumnItemList container;
	private String function;
	
	public ChoosedDatacolumnItem(Datacolumn datacolumn,String aliasname,ChoosedDatatableItem chooseddatatableitem){
		this.datacolumn = datacolumn;
		this.aliasname = aliasname;
		this.chooseddatatableitem = chooseddatatableitem;
	}
	
	public String getUsingName(){
		if(aliasname!=null && !aliasname.equals("")){
			return aliasname;
		}
		return getFullName();
	}
	
	public String getFullName(){
		if(chooseddatatableitem==null){
			return datacolumn.getName();
		}
		if(chooseddatatableitem.getAliasname()==null || chooseddatatableitem.getAliasname().equals("") ) return chooseddatatableitem.getUsingName() +"."+ datacolumn.getName();
		return chooseddatatableitem.getAliasname()+"."+datacolumn.getName();
	}

	public String getAliasname() {
		return aliasname;
	}
	public void setAliasname(String aliasname) {
		this.aliasname = aliasname;
	}
	public ChoosedDatacolumnItemList getContainer() {
		return container;
	}
	public void setContainer(ChoosedDatacolumnItemList container) {
		this.container = container;
	}
	public Datacolumn getDatacolumn() {
		return datacolumn;
	}
	public void setDatacolumn(Datacolumn datacolumn) {
		this.datacolumn = datacolumn;
	}
	public String getColumn(int i) {
		switch(i){
		case 0:
			return getFullName();
		case 1:
			if(aliasname==null) return datacolumn.getType();
			return getAliasname();
		case 2:
			if(function == null || function.equalsIgnoreCase(""))return"Not Set";
			return getFunction();
		}
		return "Not Set";
	}
	public Object[] getElements() {
		// TODO Auto-generated method stub
		return null;
	}
	public ChoosedDatatableItem getDatatableItem() {
		return chooseddatatableitem;
	}
	public void setDatatable(ChoosedDatatableItem chooseddatatableitem) {
		this.chooseddatatableitem = chooseddatatableitem;
	}
	public Object[] getChildren() {
		// TODO Auto-generated method stub
		return null;
	}
	public String getText() {
		return datacolumn.getName();
	}
	public boolean hasChildren() {
		// TODO Auto-generated method stub
		return false;
	}
	public ChoosedDatatableItem getChooseddatatableitem() {
		return chooseddatatableitem;
	}
	public void setChooseddatatableitem(ChoosedDatatableItem chooseddatatableitem) {
		this.chooseddatatableitem = chooseddatatableitem;
	}

	public String getFunction() {
		return function;
	}

	public void setFunction(String function) {
		this.function = function;
	}
	

}

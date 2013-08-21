package com.xiaonei.reg.guide.tool.parser.composite;

import java.util.ArrayList;

import com.xiaonei.reg.guide.tool.model.Datacolumn;
import com.xiaonei.reg.guide.tool.model.Datatable;
import com.xiaonei.reg.guide.tool.parser.exeption.NoSuchDatatableException;

public class ChoosedDatatableItem implements ISQLGenerator{
	
	private ChoosedDatatableItemList container;
	private Datatable datatable;
	private String aliasname;
	
	private ArrayList<ChoosedDatacolumnItem> columns = new ArrayList<ChoosedDatacolumnItem>() ;
	
	public ChoosedDatatableItemList getContainer() {
		return container;
	}
	public void setContainer(ChoosedDatatableItemList container) {
		this.container = container;
	}
	public ChoosedDatatableItem(Datatable datatable, String aliasname) throws NoSuchDatatableException {
		super();
		this.datatable = datatable;
		this.aliasname = aliasname;
		if(datatable == null) return;
		if(datatable.getDatacolumns()==null){
			System.out.println("ChoosedDatatableItem::ChoosedDatatableItem() -- columns null");
			return;
		}
		for(Datacolumn col : datatable.getDatacolumns()){
			ChoosedDatacolumnItem item = new ChoosedDatacolumnItem(col, null, this);
			columns.add(item);
		}
	}
	public String getUsingName(){
		if(aliasname!=null && !aliasname.equals("")){
			return aliasname;
		}
		return datatable.getName();
	}
	
	
	public String getAliasname() {
		return aliasname;
	}
	public boolean setAliasname(String aliasname) {
		if(container!=null && !container.containsAliasname(aliasname)){
			this.aliasname = aliasname;
			return true;
		}
		else if(container == null){
			this.aliasname = aliasname;
		}
		return false;
	}
	public Datatable getDatatable() {
		return datatable;
	}
	public void setDatatable(Datatable datatable) {
		this.datatable = datatable;
	}

	public String getColumn(int i) {
		switch(i){
		case 0:
			return datatable.getName();
		case 1:
			return getAliasname();
		}
		return "Not right";
	}

	public Object[] getElements() {
		// TODO Auto-generated method stub
		return null;
	}
	public Object[] getChildren() {
		//return datatable.getDatacolumns().toArray();
		return columns.toArray();
	}
	public String getText() {
		String tempA = aliasname;
		if(aliasname==null || aliasname.equals(""))tempA = datatable.getName();
		return datatable.getName()+" ["+tempA+"]";
	}
	public boolean hasChildren() {
		return true;
	}
	public ArrayList<ChoosedDatacolumnItem> getColumns() {
		return columns;
	}
	public void setColumns(ArrayList<ChoosedDatacolumnItem> columns) {
		this.columns = columns;
	}
	public String getSQL() {
		if(aliasname!=null && !aliasname.equals("")){
			return datatable.getName()+" "+aliasname;
		}
		return datatable.getName();
	}
}

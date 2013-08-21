package com.xiaonei.reg.guide.tool.parser.composite;

import java.util.ArrayList;

import com.xiaonei.reg.guide.tool.model.Datatable;
import com.xiaonei.reg.guide.tool.parser.exeption.NoSuchDatatableException;

public class DatatableJointItem extends ChoosedDatatableItem {

	private ChoosedDatatableItem table1;
	private ChoosedDatatableItem table2;
	private String jointype = "JOIN";
	//private ArrayList<String> conditions = new ArrayList<String>();
	private ArrayList<JointConditionItem> conditions = new ArrayList<JointConditionItem>();
	
	public DatatableJointItem(Datatable datatable, String aliasname) throws NoSuchDatatableException {
		super(datatable, aliasname);
	}
	
	public ChoosedDatatableItem getLeafchild(){
		if(table1 instanceof DatatableJointItem){
			return ((DatatableJointItem)table1).getLeafchild();
		}
		else{
			return table1;
		}
	}
	
	public boolean hasSetCondition(JointConditionItem jci){
		for(JointConditionItem ci : conditions){
			if(ci.getSQL().equalsIgnoreCase(jci.getSQL())){
				return true;
			}
		}
		return false;
	}
	public boolean containsTableItem(ChoosedDatatableItem item){
		if(table1==item || table2==item)return true;
		if(table1 instanceof DatatableJointItem){
			if(table2 instanceof DatatableJointItem){
				return ((DatatableJointItem)table1).containsTableItem(item) && ((DatatableJointItem)table2).containsTableItem(item);
			}
			else{
				return ((DatatableJointItem)table1).containsTableItem(item);
			}
		}
		else if(table2 instanceof DatatableJointItem){
			return ((DatatableJointItem)table2).containsTableItem(item);
		}
		return false;
	}
	
	public void swap(){
		ChoosedDatatableItem table = table2;
		table2 = table1;
		table1 = table;
	}
	
	public String getSQL(){
		StringBuilder sb = new StringBuilder();		
		for (JointConditionItem item : conditions) {
			sb.append("AND "+item.colname1+" "+item.operation+" "+item.colname2+" ");
		} 
		sb.append("    ");
		String condStr = sb.toString().substring(4).trim();
		return "("+table1.getSQL()+" "+jointype+" "+table2.getSQL()+" ON "+condStr+")";
	}
	
	@Override
	public String getText() {
		return table1.getUsingName()+" "+jointype+" "+table2.getUsingName();
	}
	
	@Override
	public String getUsingName() {
		return table1.getUsingName();
	}
	
	@Override
	public Object[] getChildren() {
		return new Object[]{table1,table2};
	}
	
	@Override
	public String getColumn(int i) {
		switch(i){
		case 0:
			return "111";
		case 1:
			return "==";
		case 2:
			return "333";
		}
		return "Erro in JointItem{}";
	}
	
	public ChoosedDatatableItem getLeafTable1(){
		ChoosedDatatableItem ret = table1;
		while (ret instanceof DatatableJointItem){
			ret = ((DatatableJointItem)table1).getLeafTable1();
		}
		return ret;
	}
	
	public ChoosedDatatableItem getLeafTable2(){
		ChoosedDatatableItem ret = table2;
		while (ret instanceof DatatableJointItem){
			ret = ((DatatableJointItem)table2).getLeafTable2();
		}
		return ret;
	}
	
	@Override
	public Object[] getElements() {
		return conditions.toArray();
	}

	public ChoosedDatatableItem getTable1() {
		return table1;
	}

	public void setTable1(ChoosedDatatableItem table1) {
		this.table1 = table1;
	}

	public ChoosedDatatableItem getTable2() {
		return table2;
	}

	public void setTable2(ChoosedDatatableItem table2) {
		this.table2 = table2;
	}

	public ArrayList<JointConditionItem> getConditions() {

		return conditions;
	}

	public void setConditions(ArrayList<JointConditionItem> conditions) {
		this.conditions = conditions;
	}

	public String getJointype() {
		return jointype;
	}

	public void setJointype(String jointype) {
		this.jointype = jointype;
	}
	
}

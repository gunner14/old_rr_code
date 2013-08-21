package com.xiaonei.reg.guide.tool.parser.composite;


public class JointConditionItem {
	public String colname1;
	public String operation;
	public String colname2;
	public String getColumn(int i) {
		switch(i){
		case 0:
			return colname1;
		case 1:
			return operation;
		case 2:
			return colname2;
		}
		return "error in JointConditionItem";
	}
	public Object[] getElements() {
		// TODO Auto-generated method stub
		return null;
	}
	
	public String getSQL(){
		return colname1+" "+operation+" "+colname2+" ";
	}
	
}

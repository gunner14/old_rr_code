package com.xiaonei.reg.guide.tool.parser.sqlmodel;

public class SqlOperation implements ISqlObject {

	String operation;
	int id;

	public String getOperation() {
		return operation;
	}

	public void setOperation(String operation) {
		this.operation = operation;
	}

	public String getText() {
		return operation;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	@Override
	public String getType() {
		// TODO Auto-generated method stub
		return null;
	}

}

package com.xiaonei.reg.guide.tool.parser.sqlmodel;

public class SqlExpress implements ISqlObject {
	String attributename;
	String operation;
	String prefix;
	String not;
	String value;
	String datacoltype;
	public String getDatacoltype() {
		return datacoltype;
	}

	public void setDatacoltype(String datacoltype) {
		this.datacoltype = datacoltype;
	}

	int id;

	public SqlExpress() {
		attributename = "";
		operation = "";
		prefix = "";
		not = "";
		value = "";
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}
	public String getAttributename() {
		return attributename;
	}

	public void setAttributename(String attributename) {
		this.attributename = attributename;
	}

	public String getOperation() {
		return operation;
	}

	public void setOperation(String operation) {
		this.operation = operation;
	}

	public String getPrefix() {
		return prefix;
	}

	public void setPrefix(String prefix) {
		this.prefix = prefix;
	}

	public String getValue() {
		return value;
	}

	public void setValue(String value) {
		this.value = value;
	}

	public String getNot() {
		return not;
	}

	public void setNot(String not) {
		this.not = not;
	}

	public String getText() {
		return getPrefix()+" "+getNot()+" "+getAttributename()+" "+getOperation()+" "+getValue();
	}

	@Override
	public String getType() {
		return datacoltype;
	}

}

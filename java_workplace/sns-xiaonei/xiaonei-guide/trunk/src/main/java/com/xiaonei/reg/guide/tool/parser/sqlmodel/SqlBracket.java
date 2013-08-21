package com.xiaonei.reg.guide.tool.parser.sqlmodel;

public class SqlBracket implements ISqlObject {

	public int id;
	public String content;

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getText() {
		return content;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}

	@Override
	public String getType() {
		// TODO Auto-generated method stub
		return null;
	}
}

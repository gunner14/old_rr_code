package com.xiaonei.reg.guide.tool.parser.exeption;

public class NoSuchDatacolumnException extends Exception  {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Override
	public String toString() {
		return "NoSuchDatacolumnException: Target Columnname is '"+colname+"'";
	}
	
	private String colname;
	public NoSuchDatacolumnException(String colname){
		 this.colname = colname;
	}
}

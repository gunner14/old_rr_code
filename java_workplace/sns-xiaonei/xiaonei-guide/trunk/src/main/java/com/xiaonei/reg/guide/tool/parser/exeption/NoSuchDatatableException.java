package com.xiaonei.reg.guide.tool.parser.exeption;

public class NoSuchDatatableException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	@Override
	public String toString() {
		return "NoSuchDatatableException: Target Tablename is '"+tbname+"'";
	}
	
	private String tbname;
	public NoSuchDatatableException(String tbname){
		 this.tbname = tbname;
	}
}

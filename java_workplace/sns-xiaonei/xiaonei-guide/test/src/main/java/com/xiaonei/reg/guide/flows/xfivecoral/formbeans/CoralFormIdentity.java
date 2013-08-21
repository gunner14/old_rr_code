package com.xiaonei.reg.guide.flows.xfivecoral.formbeans;

import com.xiaonei.reg.guide.root.interfaces.IUserInfoStage;

public class CoralFormIdentity implements IUserInfoStage{

	private int stage;

	public int getStage() {
		return stage;
	}

	public void setStage(int stage) {
		this.stage = stage;
	}

	/**
	 * Constructs a <code>String</code> with all attributes
	 * in name = value format.
	 *
	 * @return a <code>String</code> representation 
	 * of this object.
	 */
	public String toString()
	{
	    final String TAB = ".";
	    
	    String retValue = "";
	    
	    retValue = "FormIdentity ( "
	        + super.toString() + TAB
	        + "stage = " + this.stage + TAB
	        + " )";
	
	    return retValue;
	}
	
	


}

package com.xiaonei.tools.ice.client.ice;

public abstract class LocalException extends RuntimeException implements
		Cloneable {
	public java.lang.Object clone() {
		java.lang.Object o = null;
		try {
			o = super.clone();
		} catch (CloneNotSupportedException ex) {
			assert false; // Impossible
		}
		return o;
	}

	public abstract String ice_name();

	public String toString() {
		java.io.StringWriter sw = new java.io.StringWriter();
		java.io.PrintWriter pw = new java.io.PrintWriter(sw);
		OutputBase out = new OutputBase(pw);
		out.setUseTab(false);
		out.print(getClass().getName());
		out.inc();
		pw.flush();
		return sw.toString();
	}
}

package com.xiaonei.platform.framework.view;

import java.util.HashMap;
import java.util.Map;

/**
 * 
 * 
 * @author lifs
 * 
 */
public class XnView implements View {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1846855969777859267L;
	
	private Map<String, Object> map;

	public XnView(boolean isNeedMap) {
		if (isNeedMap)
			map = new HashMap();
	}

	private String xml;
	public String getXml() {
		return xml;
	}
	public void setXml(String xml) {
		this.xml = xml;
	}
	
	public final void setAttr(final String name, final Object value) {
		this.map.put(name, value);
	}

	public final Object getAttr(final String name) {
		return map.get(name);
	}

	public final String getAttrFormatString(final String name) {
		if (map.get(name) != null)
			return (String) map.get(name);
		else
			return null;
	}

	public final Integer getAttrFormatInteger(final String name) {
		if (map.get(name) != null)
			return (Integer) map.get(name);
		else
			return 0;
	}
}

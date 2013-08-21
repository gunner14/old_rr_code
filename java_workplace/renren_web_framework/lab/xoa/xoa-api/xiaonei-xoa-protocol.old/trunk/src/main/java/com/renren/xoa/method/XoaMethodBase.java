package com.renren.xoa.method;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;

import com.renren.xoa.XoaMethod;

/**
 * {@link XoaMethod}的基类实现
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-3-3 下午04:35:46
 */
public abstract class XoaMethodBase implements XoaMethod {

	protected String domain;
	
	protected String path;
	
	protected Map<String, String[]> parameters;
	
	public XoaMethodBase(){}
	
	public XoaMethodBase(String domain, String path) {
		this.domain = domain;
		this.path = path;
	}
	
	@Override
	public String getDomain() {
		return domain;
	}

	@Override
	public String getPath() {
		return path;
	}

	@Override
	public void setDomain(String domain) {
		this.domain = domain;
	}

	@Override
	public void setPath(String path) {
		this.path = path;
	}
	
	@Override
	public void setParam(String name, String value) {
		if (name == null || value == null) {
			throw new NullPointerException();
		}
		if (parameters == null) {	//lazy init
			parameters = new HashMap<String, String[]>();
		}
		String[] values = parameters.get(name);
		if (values == null) {
			values = new String[]{value};
		} else {
			String[] newValues = new String[values.length + 1];
			System.arraycopy(values, 0, newValues, 0, values.length);
			newValues[newValues.length - 1] = value;	//put value at last
			values = newValues;
		}
		parameters.put(name, values);
	}
	
	protected String urlEncode(String s) {
		
		try {
			return URLEncoder.encode(s, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			
			//code never reach here
			e.printStackTrace();
			return null;
		}
		
	}
	
}

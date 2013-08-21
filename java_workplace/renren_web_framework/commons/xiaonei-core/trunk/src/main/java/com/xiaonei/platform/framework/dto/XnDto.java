package com.xiaonei.platform.framework.dto;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;



/**
 * Xiaonei数据传输对象
 * 
 * @author lifs
 * 
 */
public class XnDto extends DTO{

	/**
	 * 
	 */
	private static final long serialVersionUID = -6306198574332867416L;
	/**
	 * 一个map结构，子类Dto可以向这个map中放入任何参数。
	 */
	protected Map<String , Object> paramMap;
	
	private String dealMethod = "";

	/**
	 * 缺省的构造子函数
	 */
	public XnDto(){
		
	}
	/**
	 * 构造子函数
	 * @param isNeedMap 是否在构造时，就创建一个Map结构，用来装入各种参数
	 */
	public XnDto(boolean isNeedMap){
		if(isNeedMap)
			paramMap = new HashMap();
	}
	/**
	 * 构造子函数
	 * @param map 在构造时，利用传入的Map结构，为实例的成员map对象赋值。
	 */
	public XnDto(Map map){
		this.paramMap = map;
	}
	
	
	
	public Map<String, Object> getParamMap() {
		return paramMap;
	}
	public void setParamMap(Map<String, Object> paramMap) {
		this.paramMap = paramMap;
	}
	
	public final void setAttr(final String name,final Object value){
		this.paramMap.put(name, value);
	}
	public final Object getAttr(final String name){
		return paramMap.get(name);
	}

	public String getAttrFormatString(String name){
		if(paramMap.get(name)!=null)
			return (String)paramMap.get(name);
		else
			return null;
	}

	public Integer getAttrFormatInteger(String name){
		try {
			if(paramMap.get(name)!=null)
				return (Integer)paramMap.get(name);	
		} catch (Exception e) {
			// 
			e.printStackTrace(System.err);
		}
		return 0;
	}
	public Long getAttrFormatLong(String name){
		try {
			if(paramMap.get(name)!=null)
				return (Long)paramMap.get(name);	
		} catch (Exception e) {
			// 
			e.printStackTrace(System.err);
		}
		return 0L;
	}

	public Boolean getAttrFormatBoolean(String name){
		try {
			if(paramMap.get(name)!=null)
				return (Boolean)paramMap.get(name);	
		} catch (Exception e) {
			// 
			e.printStackTrace(System.err);
		}
		return null;
	}
	
	
	/**
	 * @return the dealMethod
	 */
	public String getDealMethod() {
		return dealMethod;
	}
	/**
	 * @param dealMethod the dealMethod to set
	 */
	public void setDealMethod(String dealMethod) {
		this.dealMethod = dealMethod;
	}
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		if(paramMap!=null){
			Set keyset = paramMap.keySet();
			if(!keyset.isEmpty()){
				StringBuffer sb = new StringBuffer();
				sb.append("              +-----XnDto begin ------");
				for (Iterator iter = keyset.iterator(); iter.hasNext();) {
					String key = (String) iter.next();
					sb.append("\n                    +---"+key);
					sb.append("=");
					sb.append(paramMap.get(key));
				} 
				sb.append("\n-------------------- XnDto end ------");
				return sb.toString();
			}
		}
		return null;
	}
	
}

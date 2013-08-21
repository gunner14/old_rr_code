package com.xiaonei.platform.core.model;

import java.io.Serializable;


/**
 * 
 * 高中实体对象
 * 
 * @author fusongli
 */
public class HighSchool extends Network implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 4630650027992112119L;

	

	/** 省 */
	private String province;

	/** 城市 */
	private String city;

	

	/** 所在区县编码 */
	private String code;

	/** 所在省编码 */
	private String code_pro;

	/** 所在城市编码 */
	private String code_city;

	/** 所在区县编码 */
	private String code_county;

	/** 学生数量 */
	private String student_count;

	/** 学校类型 */
	private String property;
	/** 0 系统学校，1 用户自定义学校 */
	private int flag;
	
	private String name_pro;
	
	private String name_city;
	
	private String name_county;
	

	public String getName_city() {
		return name_city;
	}

	public void setName_city(String name_city) {
		this.name_city = name_city;
	}

	public String getName_county() {
		return name_county;
	}

	public void setName_county(String name_county) {
		this.name_county = name_county;
	}

	public String getName_pro() {
		return name_pro;
	}

	public void setName_pro(String name_pro) {
		this.name_pro = name_pro;
	}

	public String getCity() {
		return city;
	}

	public void setCity(String city) {
		this.city = city;
	}

	

	public String getProvince() {
		return province;
	}

	public void setProvince(String province) {
		this.province = province;
	}

	public static long getSerialVersionUID() {
		return serialVersionUID;
	}

	public String getCode() {
		return code;
	}

	public void setCode(String code) {
		this.code = code;
	}

	public String getCode_city() {
		return code_city;
	}

	public void setCode_city(String code_city) {
		this.code_city = code_city;
	}

	public String getCode_county() {
		return code_county;
	}

	public void setCode_county(String code_county) {
		this.code_county = code_county;
	}

	public String getCode_pro() {
		return code_pro;
	}

	public void setCode_pro(String code_pro) {
		this.code_pro = code_pro;
	}

	public String getProperty() {
		return property;
	}

	public void setProperty(String property) {
		this.property = property;
	}

	public String getStudent_count() {
		return student_count;
	}

	public void setStudent_count(String student_count) {
		this.student_count = student_count;
	}

	public int getFlag() {
		return flag;
	}

	public void setFlag(int flag) {
		this.flag = flag;
	}

}
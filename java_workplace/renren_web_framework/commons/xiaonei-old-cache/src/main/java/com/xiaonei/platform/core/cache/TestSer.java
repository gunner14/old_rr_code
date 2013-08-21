package com.xiaonei.platform.core.cache;

public class TestSer implements java.io.Serializable{
	public String name;
	public int sex;
	public TestSer(String name,int sex){
		this.name = name;
		this.sex = sex;
	}
	public String getName(){
		return name;
	}
	public int getSex(){
		return sex;
	}
}

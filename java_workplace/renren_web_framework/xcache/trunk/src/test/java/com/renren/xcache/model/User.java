package com.renren.xcache.model;

import java.io.Serializable;

public class User implements Serializable {

	private static final long serialVersionUID = 1L;

	private int id;
	
	private String name;
	
	private int age;
	
	public User(int id, String name, int age) {
		this.id = id;
		this.name = name;
		this.age = age;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getAge() {
		return age;
	}

	public void setAge(int age) {
		this.age = age;
	}
	
	public boolean equals(Object obj) {
		if (obj instanceof User) {
			User anUser = (User) obj;
			return this.id == anUser.id && this.age == anUser.age
					&& this.name.equals(anUser.name);
		}
		return false;
	}
	
	public String toString() {
		return id + "," + name + "," + age;
	}
	
	public int hashCode() {
		throw new UnsupportedOperationException("hashCode not designed");
	}
	
}

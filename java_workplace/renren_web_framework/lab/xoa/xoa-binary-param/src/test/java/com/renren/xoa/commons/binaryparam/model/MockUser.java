package com.renren.xoa.commons.binaryparam.model;

import java.io.Serializable;
import java.util.List;


public class MockUser implements Serializable {

    private static final long serialVersionUID = 1L;

    private int id;
    
    private String name;
    
    private int age;

    private List<String> hobbies;
    
    
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
    
    public List<String> getHobbies() {
        return hobbies;
    }

    public void setHobbies(List<String> hobbies) {
        this.hobbies = hobbies;
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof MockUser)) {
            return false;
        }
        MockUser user2 = (MockUser)obj;
        if (this.getId() != user2.getId()) {
            return false;
        }
        if (!this.getName().equals(user2.getName())) {
            return false;
        }
        if (this.getAge() != user2.getAge()) {
            return false;
        }
        if (! this.getHobbies().equals(user2.getHobbies())) {
            return false;
        }
        return true;
    }
    
}

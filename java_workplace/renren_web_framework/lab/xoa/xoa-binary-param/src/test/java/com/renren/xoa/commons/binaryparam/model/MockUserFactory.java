package com.renren.xoa.commons.binaryparam.model;

import java.util.ArrayList;
import java.util.List;


public class MockUserFactory {

    private static int NAME_SEQ = 1;
    
    public static MockUser getUser() {
        
        MockUser u = new MockUser();
        u.setName("name-" + (NAME_SEQ++));
        u.setAge(21);
        u.setId(1000);
        u.setHobbies(getHobbies());
        return u;
    }
    
    public static List<MockUser> getUsers(int count) {
        
        return null;
    }
    
    
    public static List<String> getHobbies() {
        List<String> hobbies = new ArrayList<String>();
        hobbies.add("football");
        hobbies.add("smoke");
        return hobbies;
    }
}

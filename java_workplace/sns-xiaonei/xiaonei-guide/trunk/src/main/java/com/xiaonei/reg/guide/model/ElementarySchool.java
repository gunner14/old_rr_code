package com.xiaonei.reg.guide.model;

import java.io.Serializable;

public class ElementarySchool implements Serializable {

    private static final long serialVersionUID = -9032978372421719648L;

    private int id;

    private String name;

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

}

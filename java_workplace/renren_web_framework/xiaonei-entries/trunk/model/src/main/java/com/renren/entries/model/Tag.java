package com.renren.entries.model;

import java.io.Serializable;

/**
 * 
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 */
public class Tag implements Serializable {

    private static final long serialVersionUID = 1L;

    private int id;

    private String namespace = "/";

    private String value;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getNamespace() {
        return namespace;
    }

    public void setNamespace(String namespace) {
        this.namespace = namespace;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

}

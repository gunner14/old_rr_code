package com.xiaonei.ugc.entrysystem.locator;

import org.springframework.beans.factory.BeanNameAware;

import com.xiaonei.ugc.entrysystem.EntrySystem;

/**
 * {@link EntrySystemRef} 代表一个 {@link EntrySystem} 实例以及对她的一些说明。
 * <p>
 * 
 * @author zhiliang.wang [zhiliang.wang@opi-corp.com]
 * 
 */
public class EntrySystemRef implements BeanNameAware {

    private String id;

    private String note;

    private EntrySystem system;

    @Override
    public void setBeanName(String beanName) {
        this.setId(beanName);
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getNote() {
        return note;
    }

    public void setNote(String note) {
        this.note = note;
    }

    public EntrySystem getSystem() {
        return system;
    }

    public void setSystem(EntrySystem system) {
        this.system = system;
    }
}

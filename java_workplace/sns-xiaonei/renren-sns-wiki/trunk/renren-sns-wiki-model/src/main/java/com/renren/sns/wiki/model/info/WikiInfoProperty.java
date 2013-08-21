package com.renren.sns.wiki.model.info;

/**
 * wiki的每个属性对应这么一个bean
 * 
 * @author weiwei.wang@renren-inc.com since 2012-03-30
 */
public class WikiInfoProperty {

    private long id;

    private String name;

    private String value;

    private String fieldName;

    private int priority = 10; //默认值为10

    private int hide; //是否隐藏0是不隐藏 1是隐藏

    private int wikiId;

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    public String getFieldName() {
        return fieldName;
    }

    public void setFieldName(String fieldName) {
        this.fieldName = fieldName;
    }

    public int getPriority() {
        return priority;
    }

    public void setPriority(int priority) {
        this.priority = priority;
    }

    public int getHide() {
        return hide;
    }

    public void setHide(int hide) {
        this.hide = hide;
    }

    public int getWikiId() {
        return wikiId;
    }

    public void setWikiId(int wikiId) {
        this.wikiId = wikiId;
    }

    @Override
    public String toString() {
        return "WikiInfoProperty [name=" + name + ", value=" + value + "]";
    }
}

package com.renren.sns.wiki.model.info;

import java.util.List;

/**
 * 所有的类型类都继承这个接口,工厂类会返回WikiInfo,然后转化为对应的类型类
 * 
 * @author weiwei.wang@renren-inc.com since 2012-03-30
 * 
 */
public abstract class WikiInfo {

    private List<WikiInfoProperty> propertyList;

    private int wikiId;

    public List<WikiInfoProperty> getPropertyList() {
        return propertyList;
    }

    public void setPropertyList(List<WikiInfoProperty> propertyList) {
        this.propertyList = propertyList;
    }

    public int getWikiId() {
        return wikiId;
    }

    public void setWikiId(int wikiId) {
        this.wikiId = wikiId;
    }

    //抽象方法,获取wiki的简介
    public abstract String getSummary();
    
    public abstract void setSummary(String summary);

}

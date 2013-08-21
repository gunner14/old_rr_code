package com.renren.sns.guide.model;

/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-12 上午11:02:17
 * 类说明
 */
import java.util.List;

/*
 * 你可能认识的人页面，一个对象对应于每一栏
 * 
 */
public interface GuideIRelation {
    
    public int getTypeId();
    
    public String getType();
    
    public String getTypeName();
    
    public int getCount();
    
    public String getButtonValue();

    public String getTitle();

    public String getMoreLink();

    public List<GuideIRelationFriend> getList();
    
    public void setList(List<GuideIRelationFriend> list);


}

 
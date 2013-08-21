package com.renren.sns.guide.logic;

import java.util.List;

import com.renren.sns.guide.view.UserTinyView;


public interface HotStarRelation {
    
    public int getUnivId(); 
    
    public String getUnivName();   
    
    public String getType(); 

    public List<UserTinyView> getList();

}

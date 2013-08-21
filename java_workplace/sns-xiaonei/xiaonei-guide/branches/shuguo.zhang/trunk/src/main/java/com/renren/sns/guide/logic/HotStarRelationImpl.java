package com.renren.sns.guide.logic;

import java.util.ArrayList;
import java.util.List;

import com.renren.sns.guide.view.UserTinyView;


public class HotStarRelationImpl implements HotStarRelation{
    
    private List<UserTinyView> list = new ArrayList<UserTinyView>();
    
    private int univId;
    
    private String univName;
    
    private String type;

    public List<UserTinyView> getList() {
        return list;
    }
    
    public void setList(List<UserTinyView> list) {
        this.list = list;
    }
    
    public int getUnivId() {
        return univId;
    }

    public void setUnivId(int univId) {
        this.univId = univId;
    }

    public String getUnivName() {
        return univName;
    }

    public void setUnivName(String univName) {
        this.univName = univName;
    }
    
    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

}

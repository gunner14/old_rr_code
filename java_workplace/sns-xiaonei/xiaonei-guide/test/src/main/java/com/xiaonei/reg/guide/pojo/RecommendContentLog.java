package com.xiaonei.reg.guide.pojo;

import java.sql.Timestamp;


public class RecommendContentLog {
    
    private int id;
    private int user_id;
    private int type;
    private int recommend_content_id;
    private Timestamp op_time;
    
    public int getId() {
        return id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public int getUser_id() {
        return user_id;
    }
    
    public void setUser_id(int user_id) {
        this.user_id = user_id;
    }
    
    public int getType() {
        return type;
    }
    
    public void setType(int type) {
        this.type = type;
    }
    
    public int getRecommend_content_id() {
        return recommend_content_id;
    }
    
    public void setRecommend_content_id(int recommend_content_id) {
        this.recommend_content_id = recommend_content_id;
    }
    
    public Timestamp getOp_time() {
        return op_time;
    }
    
    public void setOp_time(Timestamp op_time) {
        this.op_time = op_time;
    }
    
    

}

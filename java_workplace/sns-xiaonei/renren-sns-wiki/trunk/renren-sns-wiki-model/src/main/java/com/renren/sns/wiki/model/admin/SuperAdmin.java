package com.renren.sns.wiki.model.admin;

import java.util.Date;

/**
 * 超管
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-4
 * 
 */
public class SuperAdmin {

    private int id;

    /**
     * 人人id
     */
    private int userId;

    /**
     * 名字,一般为人人网的名字
     */
    private String name;

    /**
     * 备注
     */
    private String remark;

    /**
     * 管理员类型
     */
    private int type;

    /**
     * 数据插入时间,默认值
     */
    private Date insertTime;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public Date getInsertTime() {
        return insertTime;
    }

    public void setInsertTime(Date insertTime) {
        this.insertTime = insertTime;
    }

}

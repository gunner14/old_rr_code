package com.xiaonei.reg.jump.data.model;

/**
 * MlinkRegToGameMapModel.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-14 创建文件
 */
public class MlinkRegToGameMapModel {

    private int id;

    private int ss_id;

    private int rt_id;

    private int x_id;

    private int y_id;

    private int game_stat_id;

    private String code_id;

    private String to_url;

    private String remark;

    private String insert_time;

    private String update_time;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getSs_id() {
        return ss_id;
    }

    public void setSs_id(int ss_id) {
        this.ss_id = ss_id;
    }

    public int getRt_id() {
        return rt_id;
    }

    public void setRt_id(int rt_id) {
        this.rt_id = rt_id;
    }

    public int getX_id() {
        return x_id;
    }

    public void setX_id(int x_id) {
        this.x_id = x_id;
    }

    public int getY_id() {
        return y_id;
    }

    public void setY_id(int y_id) {
        this.y_id = y_id;
    }

    public int getGame_stat_id() {
        return game_stat_id;
    }

    public void setGame_stat_id(int game_stat_id) {
        this.game_stat_id = game_stat_id;
    }

    public String getCode_id() {
        return code_id;
    }

    public void setCode_id(String code_id) {
        this.code_id = code_id;
    }

    public String getTo_url() {
        return to_url;
    }

    public void setTo_url(String to_url) {
        this.to_url = to_url;
    }

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public String getInsert_time() {
        return insert_time;
    }

    public void setInsert_time(String insert_time) {
        this.insert_time = insert_time;
    }

    public String getUpdate_time() {
        return update_time;
    }

    public void setUpdate_time(String update_time) {
        this.update_time = update_time;
    }

    /**
     * Constructs a <code>String</code> with all attributes
     * in name = value format.
     *
     * @return a <code>String</code> representation 
     * of this object.
     */
    public String toString()
    {
        final String TAB = " ";
        
        String retValue = "";
        
        retValue = "MlinkRegToGameMapModel ( "
            + "id = " + this.id + TAB
            + "ss_id = " + this.ss_id + TAB
            + "rt_id = " + this.rt_id + TAB
            + "x_id = " + this.x_id + TAB
            + "y_id = " + this.y_id + TAB
            + "game_stat_id = " + this.game_stat_id + TAB
            + "code_id = " + this.code_id + TAB
            + "to_url = " + this.to_url + TAB
            + "remark = " + this.remark + TAB
            + "insert_time = " + this.insert_time + TAB
            + "update_time = " + this.update_time + TAB
            + " )";
    
        return retValue;
    }

}

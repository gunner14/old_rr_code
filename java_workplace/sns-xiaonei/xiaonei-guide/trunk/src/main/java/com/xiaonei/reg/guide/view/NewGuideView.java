package com.xiaonei.reg.guide.view;

import com.xiaonei.search2.model.client.SearchView;

import java.io.Serializable;
import java.util.List;

public class NewGuideView implements Serializable {

    //因为这个对象可以被大学、高中等学校类型使用，所以单独抽取一个schoolid存储学校ID
    private int schoolid;

    public int getSchoolid() {
        return schoolid;
    }

    public void setSchoolid(int schoolid) {
        this.schoolid = schoolid;
    }

    /**
     *
     */
    private static final long serialVersionUID = 1L;

    private String title;

    private int univId;

    private int totalCount;

    private List<SearchView> result;

    public List<SearchView> getResult() {
        return result;
    }

    public void setResult(List<SearchView> result) {
        this.result = result;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public int getTotalCount() {
        return totalCount;
    }

    public void setTotalCount(int totalCount) {
        this.totalCount = totalCount;
    }

    public int getUnivId() {
        return univId;
    }

    public void setUnivId(int univId) {
        this.univId = univId;
    }

}

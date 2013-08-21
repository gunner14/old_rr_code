package com.renren.sns.guide.model;

import java.util.ArrayList;
import java.util.List;

import com.xiaonei.reg.guide.model.SnsFootprintView;

/**
 * @author wei.xiang(wei.xiang@opi-corp.com)
 * @since 2010-1-26
 */
public class SnsFootprintResult {

    private List<SnsFootprintView> footprintViewList = new ArrayList<SnsFootprintView>();

    private int userVisitCount = 0;

    public SnsFootprintResult() {
    }

    /**
     * @return the footprintViewList
     */
    public List<SnsFootprintView> getFootprintViewList() {
        return footprintViewList;
    }

    /**
     * @return the userVisitCount
     */
    public int getUserVisitCount() {
        return userVisitCount;
    }

    /**
     * @param userVisitCount the userVisitCount to set
     */
    public void setUserVisitCount(int userVisitCount) {
        this.userVisitCount = userVisitCount;
    }

    public void addSnsFootprintView(SnsFootprintView view) {
        getFootprintViewList().add(view);
    }
}

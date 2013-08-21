package com.renren.sns.minisite.feed.xoa.components.activeuser.model;

import java.util.Collections;
import java.util.List;


public class UserAct {

    private List<Integer> activeIds = Collections.emptyList();

    private List<Integer> unActiveIds = Collections.emptyList();

    public List<Integer> getActiveIds() {
        return this.activeIds;
    }

    public List<Integer> getUnActiveIds() {
        return this.unActiveIds;
    }

    public void setActiveIds(final List<Integer> activeIds) {
        this.activeIds = activeIds;
    }

    public void setUnActiveIds(final List<Integer> unActiveIds) {
        this.unActiveIds = unActiveIds;
    }

}

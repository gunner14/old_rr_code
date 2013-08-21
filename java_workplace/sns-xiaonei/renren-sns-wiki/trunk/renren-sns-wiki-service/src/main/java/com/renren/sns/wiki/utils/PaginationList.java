package com.renren.sns.wiki.utils;

import java.io.Serializable;
import java.util.List;

/**
 * @author yi.li@renren-inc.com since Aug 14, 2012
 * 
 */
public class PaginationList<T> implements Serializable {

    /**
     * 
     */
    private static final long serialVersionUID = 5763394058786656951L;

    private int totalCount;

    private int currentPage;

    private int pageSize;

    private List<T> pageList;

    public int getTotalCount() {
        return totalCount;
    }

    public void setTotalCount(int totalCount) {
        this.totalCount = totalCount;
    }

    public int getCurrentPage() {
        return currentPage;
    }

    public void setCurrentPage(int currentPage) {
        this.currentPage = currentPage;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public List<T> getPageList() {
        return pageList;
    }

    public void setPageList(List<T> pageList) {
        this.pageList = pageList;
    }
}

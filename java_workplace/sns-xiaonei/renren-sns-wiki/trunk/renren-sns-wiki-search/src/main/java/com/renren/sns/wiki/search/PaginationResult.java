package com.renren.sns.wiki.search;

import java.io.Serializable;
import java.util.List;

import org.apache.lucene.document.Document;

/**
 * 用来存储分页查询结果
 * 
 * @author yi.li@renren-inc.com since Aug 13, 2012
 * 
 */
public class PaginationResult implements Serializable {

    /**
     * 
     */
    private static final long serialVersionUID = 1744424735034479052L;

    private int currentPage;

    private int pageSize;

    private int totalHits;

    public int getTotalHits() {
        return totalHits;
    }

    public void setTotalHits(int totalHits) {
        this.totalHits = totalHits;
    }

    private List<Document> docs;

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

    public List<Document> getDocs() {
        return docs;
    }

    public void setDocs(List<Document> docs) {
        this.docs = docs;
    }

}

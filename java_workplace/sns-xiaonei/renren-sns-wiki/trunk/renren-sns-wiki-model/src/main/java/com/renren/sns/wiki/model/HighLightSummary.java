package com.renren.sns.wiki.model;

import java.io.Serializable;
import java.util.List;

/**
 * HighLight记录Hight light区的数据
 * 
 * @author yi.li@renren-inc.com since 2012-5-2
 * 
 */
public class HighLightSummary implements Serializable {

    private static final long serialVersionUID = -5364194225696223075L;

    /**
     * 缩略图列表
     */
    private List<String> thumbnails;

    /**
     * 该类型内容的数量，不等于thumbUrls的大小
     */
    private int count;

    public List<String> getThumbnails() {
        return thumbnails;
    }

    public void setThumbnails(List<String> thumbnails) {
        this.thumbnails = thumbnails;
    }

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }
}

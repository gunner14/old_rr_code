package com.renren.sns.wiki.model.feed;


/**
 * 用于新鲜事feed和json key的对应
 * @author weiwei.wang@renren-inc.com since 2012-4-24
 *
 */
public class JsonFeedKeyPair {

    private int id; //没有实际意义的id
    private int feedType;
    private String jsonKey;
    private String feedKey;
    
    public JsonFeedKeyPair() {
    }

    
    public JsonFeedKeyPair(String jsonKey, String feedKey) {
        this.jsonKey = jsonKey;
        this.feedKey = feedKey;
    }

    public int getId() {
        return id;
    }
    
    public void setId(int id) {
        this.id = id;
    }
    
    public int getFeedType() {
        return feedType;
    }
    
    public void setFeedType(int feedType) {
        this.feedType = feedType;
    }
    
    public String getJsonKey() {
        return jsonKey;
    }
    
    public void setJsonKey(String jsonKey) {
        this.jsonKey = jsonKey;
    }
    
    public String getFeedKey() {
        return feedKey;
    }
    
    public void setFeedKey(String feedKey) {
        this.feedKey = feedKey;
    }
    
}

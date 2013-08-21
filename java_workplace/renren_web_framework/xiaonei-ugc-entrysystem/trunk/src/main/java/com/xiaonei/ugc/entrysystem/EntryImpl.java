package com.xiaonei.ugc.entrysystem;

import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

/**
 * {@link Entry} 的简单实现，API的实现方可以使用这个类来转化原来每种UGC自己定义的模型类来提供 Entry 的访问方式。
 * 
 * @author zhiliang.wang [zhiliang.wang@opi-corp.com]
 * 
 */
public class EntryImpl implements Entry {

    private String queryId;

    private Integer userId;

    private String userName;

    private String userIp;

    private String title;

    private String body;

    private Date publishTime;

    private Integer deleteUserId;

    private Date deleteTime;

    private int subEntryCount;

    private Map<String, Object> specificProperties;

    @Override
    public String getQueryId() {
        return queryId;
    }

    @Override
    public void setQueryId(String queryId) {
        this.queryId = queryId;
    }

    @Override
    public Integer getUserId() {
        return userId;
    }

    @Override
    public void setUserId(Integer userId) {
        this.userId = userId;
    }

    @Override
    public String getUserName() {
        return userName;
    }

    @Override
    public void setUserName(String userName) {
        this.userName = userName;
    }

    @Override
    public String getUserIp() {
        return userIp;
    }

    @Override
    public void setUserIp(String userIp) {
        this.userIp = userIp;
    }

    @Override
    public String getTitle() {
        return title;
    }

    @Override
    public void setTitle(String title) {
        this.title = title;
    }

    @Override
    public String getBody() {
        return body;
    }

    @Override
    public void setBody(String body) {
        this.body = body;
    }

    @Override
    public Date getPublishTime() {
        return publishTime;
    }

    @Override
    public void setPublishTime(Date publishTime) {
        this.publishTime = publishTime;
    }

    @Override
    public Integer getDeleteUserId() {
        return deleteUserId;
    }

    @Override
    public void setDeleteUserId(Integer deleteUserId) {
        this.deleteUserId = deleteUserId;
    }

    @Override
    public Date getDeleteTime() {
        return deleteTime;
    }

    @Override
    public void setDeleteTime(Date deleteTime) {
        this.deleteTime = deleteTime;
    }

    @Override
    public int getSubEntryCount() {
        return subEntryCount;
    }

    @Override
    public void setSubEntryCount(int subEntryCount) {
        this.subEntryCount = subEntryCount;
    }

    @Override
    public synchronized Map<String, Object> getSpecificProperties() {
        if (specificProperties == null) {
            return Collections.emptyMap();
        }
        return Collections.unmodifiableMap(new HashMap<String, Object>(specificProperties));
    }

    @Override
    public synchronized Object getSpecificProperty(String name) {
        return specificProperties == null ? null : specificProperties.get(name);
    }

    @Override
    public synchronized void setSpecificProperty(String name, Object value) {
        if (this.specificProperties == null) {
            specificProperties = new HashMap<String, Object>();
        }
        specificProperties.put(name, value);
    }

}

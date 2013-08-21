package com.xiaonei.ugc.entrysystem;

import java.util.Date;
import java.util.Map;

/**
 * {@link Entry} 抽象不同类型的UGC，提供了UGC条目的统一访问方式。
 * <p>
 * 不同的UGC实现可以在自己的UGC模型类(如Blog类)实现这个接口，使他可以以 {@link Entry}
 * 的形式被访问；也可以把具体的UGC条目数据设置到 {@link EntryImpl} 来提供访问。
 * 
 * @see EntryImpl
 * 
 * @author zhiliang.wang [zhiliang.wang@opi-corp.com]
 * 
 */
public interface Entry {

    /**
     * 返回唯一识别该条目、能够通过他进行识别查找的标识串
     * <p>
     * 该串作为一个整体可提高给 {@link EntrySystem#getEntry(String)} 用于获取这个条目的详细信息
     * <p>
     * 这个queryId可能不是数据的一个列，根据我们的分表原则它可能是由2个元素组成的，即用户ID和条目ID
     * 
     * @return
     */
    public String getQueryId();

    /**
     * 设置该条目的唯一识别串，能够通过他进行识别、查找
     * 
     * @param queryId
     */
    public void setQueryId(String queryId);

    /**
     * 返回该条目的作者/所有者 ID
     * 
     * @return
     */
    public Integer getUserId();

    /**
     * 设置该条目的作者/所有者 ID
     * 
     * @param userId
     */
    public void setUserId(Integer userId);

    /**
     * 设置该条目的作者的名字。
     * <p>
     * 如果返回null，表示这种条目的没有提供此信息，调用着应该使用另外的API获取该作者的信息
     * 
     * @return
     */
    public String getUserName();

    /**
     * 设置该条目的作者的名字
     * 
     * @param userName
     */
    public void setUserName(String userName);

    /**
     * 返回作者在发布这个UGC条目时候的访问地址(IP)
     * 
     * @return
     */
    public String getUserIp();

    /**
     * 设置作者在发布这个UGC条目时候的访问地址(IP)
     * 
     * @param userIp
     */
    public void setUserIp(String userIp);

    /**
     * 返回这个条目的标题、主题(Subject)之类的信息
     * 
     * @return
     */
    public String getTitle();

    /**
     * 设置这个条目的标题、主题(Subject)之类的
     * 
     * @param title
     */
    public void setTitle(String title);

    /**
     * 返回这个条目的主体内容、正文信息之类的信息
     * 
     * @return
     */
    public String getBody();

    /**
     * 设置这个条目的主体内容、正文信息之类的
     * 
     * @param body
     */
    public void setBody(String body);

    /**
     * 返回这个条目的发布时间
     * 
     * @return
     */
    public Date getPublishTime();

    /**
     * 设置这个条目的发布时间
     * 
     * @param publishTime
     */
    public void setPublishTime(Date publishTime);

    /**
     * 如果这个条目被删除放到“回收站”时，返回删除该条目的人
     * 
     * @return
     */
    public Integer getDeleteUserId();

    /**
     * 设置这个条目的删除者，如果该条目被删除到“回收站”时
     * 
     * @param deleteUserId
     */
    public void setDeleteUserId(Integer deleteUserId);

    /**
     * 如果这个条目被删除放到“回收站”时，返回删除该条目的时间
     * 
     * @return
     */
    public Date getDeleteTime();

    /**
     * 设置这个条目的删除时间，如果该条目被删除到“回收站”时
     * 
     * @param deleteTime
     */
    public void setDeleteTime(Date deleteTime);

    /**
     * 返回附着于此条目的子类条目数(比如回复、评论数之类的)
     * 
     * @return
     */
    public int getSubEntryCount();

    /**
     * 设置附着于此条目的子类条目数(比如回复、评论数之类的)
     * 
     * @param subEntryCount
     */
    public void setSubEntryCount(int subEntryCount);

    /**
     * 返回标准规定的属性之外，该条目的一些特殊属性
     * 
     * @return
     */
    public Object getSpecificProperty(String name);

    /**
     * 设置这类别条目的一些特别属性
     * 
     * @param name
     * @param value
     */
    public void setSpecificProperty(String name, Object value);

    /**
     * 返回这种类别条木的所有特别属性
     * 
     * @return
     */
    public Map<String, Object> getSpecificProperties();

}

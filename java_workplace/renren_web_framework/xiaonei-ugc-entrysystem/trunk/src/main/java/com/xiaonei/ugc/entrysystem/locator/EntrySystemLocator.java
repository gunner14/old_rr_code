package com.xiaonei.ugc.entrysystem.locator;

import com.xiaonei.ugc.entrysystem.EntrySystem;

/**
 * {@link EntrySystem}的工厂类，使用方通过这个接口获取到不同类型的 {@link EntrySystem}实例。
 * <p>
 * 
 * @author zhiliang.wang [zhiliang.wang@opi-corp.com]
 * 
 */
public interface EntrySystemLocator {

    /**
     * 返回某标识符对应的 {@link EntrySystemRef}
     * <p>
     * 
     * @param refId
     * @return
     */
    public EntrySystemRef getEntrySystemRef(String refId);

    /**
     * 返回某标识符对应的 {@link EntrySystem}
     * 
     * @param refId
     * @return
     */
    public EntrySystem getEntrySystem(String refId);
}

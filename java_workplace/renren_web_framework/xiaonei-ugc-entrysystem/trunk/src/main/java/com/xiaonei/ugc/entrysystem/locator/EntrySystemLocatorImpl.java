package com.xiaonei.ugc.entrysystem.locator;

import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

import com.xiaonei.ugc.entrysystem.EntrySystem;

/**
 * 
 * @author zhiliang.wang [zhiliang.wang@opi-corp.com]
 * 
 */
public class EntrySystemLocatorImpl implements EntrySystemLocator, ApplicationContextAware {

    private ApplicationContext applicationContext;

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
    }

    @Override
    public EntrySystemRef getEntrySystemRef(String refId) {
        return (EntrySystemRef) applicationContext.getBean(refId, EntrySystemRef.class);
    }

    @Override
    public EntrySystem getEntrySystem(String refId) {
        EntrySystemRef ref = getEntrySystemRef(refId);
        return ref == null ? null : ref.getSystem();
    }

}

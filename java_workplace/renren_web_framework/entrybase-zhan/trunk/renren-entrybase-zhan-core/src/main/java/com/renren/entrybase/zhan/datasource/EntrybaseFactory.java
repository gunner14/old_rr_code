package com.renren.entrybase.zhan.datasource;

import org.springframework.beans.factory.FactoryBean;

import com.renren.entrybase.EntryBase;

public class EntrybaseFactory implements FactoryBean {

    private ZhanBase zhanBase = new ZhanBase();

    private EntryBase entryBase;

    public void setCatalog(String catalog) {
        this.zhanBase.setCatalog(catalog);
    }

    @Override
    public Object getObject() throws Exception {
        if (entryBase == null) {
            return entryBase = zhanBase.createEntryBase();
        }
        return entryBase;
    }

    @Override
    public Class<EntryBase> getObjectType() {
        return EntryBase.class;
    }

    @Override
    public boolean isSingleton() {
        return true;
    }

}


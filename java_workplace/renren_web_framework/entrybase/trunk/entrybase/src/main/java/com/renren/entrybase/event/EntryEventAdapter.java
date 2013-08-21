package com.renren.entrybase.event;

import com.renren.entrybase.EntryBase;
import com.renren.entrybase.model.Entry;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class EntryEventAdapter implements EntryEventListener {

    @Override
    public void onAdded(EntryBase entrybase, Entry entry) {
    }

    @Override
    public void onDelete(EntryBase entrybase, Entry entry) {
    }

    @Override
    public void onUpdate(EntryBase entrybase, Entry oldEntry, Entry newEntry) {
    }
}

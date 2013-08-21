package com.renren.entrybase.event;

import java.util.EventListener;

import com.renren.entrybase.EntryBase;
import com.renren.entrybase.model.Entry;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public interface EntryEventListener extends EventListener {

    /**
     * 
     * @param entrybase
     * @param entry
     */
    void onAdded(EntryBase entrybase, Entry entry);

    /**
     * 
     * @param entrybase
     * @param entry
     */
    void onDelete(EntryBase entrybase, Entry entry);

    /**
     * 
     * @param entrybase
     * @param oldEntry
     * @param newEntry
     */
    void onUpdate(EntryBase entrybase, Entry oldEntry, Entry newEntry);
}

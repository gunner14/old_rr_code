package com.renren.entries.event;

import com.renren.entries.EntryBase;
import com.renren.entries.model.Entry;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public interface EntryEventListener {

    /**
     * 
     * @param entryBase
     * @param entry
     */
    void onAdded(EntryBase entryBase, Entry entry);

    /**
     * 
     * @param entryBase
     * @param entry
     */
    void onDelete(EntryBase entryBase, Entry entry);

    /**
     * 
     * @param entryBase
     * @param oldEntry
     * @param newEntry
     */
    void onUpdate(EntryBase entryBase, Entry oldEntry, Entry newEntry);
}

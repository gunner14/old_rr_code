package com.renren.entries.event;

import com.renren.entries.EntryBase;
import com.renren.entries.model.Entry;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public abstract class EntryEventAdapter implements EntryEventListener {

    public abstract void onAdded(EntryBase entryBase, Entry entry);

    public abstract void onDelete(EntryBase entryBase, Entry entry);

    public abstract void onUpdate(EntryBase entryBase, Entry oldEntry, Entry newEntry);
}

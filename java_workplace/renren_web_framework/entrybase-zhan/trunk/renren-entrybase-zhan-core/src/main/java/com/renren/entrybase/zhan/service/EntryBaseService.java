package com.renren.entrybase.zhan.service;

import java.util.List;

import com.renren.entrybase.model.Entry;
import com.renren.entrybase.model.Field;

public interface EntryBaseService {

	public Entry getEntry(long entryId);
	
	public int getCount(String queryString);

	public long addEntry(long id, List<Field> fields);

	public long update(long id, List<Field> fields);

	public void delete(long id);

	public List<Entry> find(String queryString, int limit);
	
	public Entry[] gets(long[] entryIds);
	
	public Entry[] gets(long[] entryIds, String[] fields);
	
	public void moveEntry(long id);
}

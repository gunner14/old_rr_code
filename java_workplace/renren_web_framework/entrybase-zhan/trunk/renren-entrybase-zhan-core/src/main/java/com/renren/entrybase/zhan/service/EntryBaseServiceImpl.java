package com.renren.entrybase.zhan.service;

import java.util.List;

import javax.annotation.PostConstruct;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.entries.xoa.api.EntryService;
import com.renren.entrybase.EntryBase;
import com.renren.entrybase.model.Entry;
import com.renren.entrybase.model.Field;
import com.renren.entrybase.model.query.Query;
import com.renren.entrybase.zhan.datasource.EntrybaseFactory;

public class EntryBaseServiceImpl implements EntryBaseService {
	@Autowired
	private EntrybaseFactory entrybaseFactory;

	private EntryBase entryBase;
	
	@Autowired
	private EntryService oldEntryService;

	private Log logger = LogFactory.getLog(this.getClass());

	@PostConstruct
	public void initEntryBase() {
		try {
			entryBase = (EntryBase) entrybaseFactory.getObject();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public Entry getEntry(long entryId) {
		// TODO Auto-generated method stub
		return entryBase.get(entryId);
	}

	@Override
	public long addEntry(long id, List<Field> fields) {
		// TODO Auto-generated method stub
		return entryBase.addEntry(id, fields).getId();
	}

	@Override
	public long update(long id, List<Field> fields) {
		// TODO Auto-generated method stub
		return entryBase.updateEntry(id, false, fields).getId();
	}

	@Override
	public void delete(long id) {
		// TODO Auto-generated method stub
		entryBase.deleteEntry(id);
	}

	@Override
	public List<Entry> find(String queryString, int limit) {
		// TODO Auto-generated method stub
		Query query = Query.toQuery(queryString);
		List<Entry> list = entryBase.find(query, limit);
		return list;
	}

	@Override
	public void moveEntry(long id) {
		// TODO Auto-generated method stub
		Entry entry = entryBase.get(id);
		com.renren.entries.model.Entry oldEntry = new com.renren.entries.model.Entry();
		List<Field> fields = entry.getFields();
		for (Field field : fields) {
			oldEntry.addField(field.getName(), field.getValue());
		}
		// 这个是老的entryService
		oldEntryService.save(id, oldEntry.getFields()).submit().awaitUninterruptibly();
		// entryBase.moveEntry(id, referBase);
	}

	@Override
	public Entry[] gets(long[] entryIds) {
		// TODO Auto-generated method stub
		Entry[] entries = entryBase.gets(entryIds);
		return entries;
	}
	
	@Override
	public Entry[] gets(long[] entryIds, String[] fields) {
		// TODO Auto-generated method stub
		Entry[] entries = entryBase.gets(entryIds, fields);
		return entries;
	}

	@Override
	public int getCount(String queryString) {
		// TODO Auto-generated method stub
		Query query = Query.toQuery(queryString);
		return entryBase.getCount(query);
	}
}

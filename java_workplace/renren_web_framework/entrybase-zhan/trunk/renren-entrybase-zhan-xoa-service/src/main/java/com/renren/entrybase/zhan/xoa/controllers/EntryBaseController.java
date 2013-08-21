package com.renren.entrybase.zhan.xoa.controllers;

import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.entrybase.model.Entry;
import com.renren.entrybase.zhan.service.EntryBaseService;
import com.renren.xoa.commons.annotation.JsonParam;

@Path("")
public class EntryBaseController {
	@Autowired
	private EntryBaseService entryBaseService;

	@Post("/entry/{entryId}")
	public Long add(@Param("entryId") long id,
			@JsonParam @Param("entry") Entry entry) {
		return entryBaseService.addEntry(id, entry.getFields());
	}

	//
	// @Post("/entry/update")
	// public void update(@Param("entryId") long id, @JsonParam @Param("entry")
	// Entry entry) {
	// zhanService.update(id, entry.getFields());
	// }

	@Get("/entry/{entryId}")
	public Entry get(@Param("entryId") long id) {
		return entryBaseService.getEntry(id);
	}
	
	@Get("/entry/count")
	public int getCount(@Param("query") String query) {
		return entryBaseService.getCount(query);
	}

	@Delete("/entry/{entryId}")
	public void delete(@Param("entryId") long id) {
		entryBaseService.delete(id);
	}

	@Get("/entry/find")
	public List<Entry> find(@Param("query") String query, //
			@Param("limit") int limit) {
		return entryBaseService.find(query, limit);
	}

	@Get("/entry")
	public Entry[] gets(@Param("entryIds") long[] entryIds, @Param("fields") String[] fields) {
		if(fields == null || fields.length == 0){
			return entryBaseService.gets(entryIds);
		}
		return entryBaseService.gets(entryIds, fields);
	}

	@Post("/entry/move/{entryId}")
	public void moveEntry(@Param("entryId") long id) {
		entryBaseService.moveEntry(id);
	}
}

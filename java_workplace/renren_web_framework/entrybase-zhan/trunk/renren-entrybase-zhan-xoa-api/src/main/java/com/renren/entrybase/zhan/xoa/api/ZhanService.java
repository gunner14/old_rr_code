package com.renren.entrybase.zhan.xoa.api;

import com.renren.entrybase.model.Entry;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.annotation.XoaDelete;
import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaParam;
import com.renren.xoa.lite.annotation.XoaPost;
import com.renren.xoa.lite.annotation.XoaService;

@XoaService(serviceId = "entrybase.xoa.renren.com")
public interface ZhanService {
	@XoaGet("/entry/{entryId}")
	public ServiceFuture<Entry> get(@XoaParam("entryId") long entryId);

	@XoaPost("/entry/{entryId}")
	public ServiceFuture<Void> add(@XoaParam("entryId") long entryId,
			@XoaParam(value = "entry", type = "json") Entry entry);

//	@XoaPost("/entry/update")
//	public ServiceFuture<Long> update(@XoaParam("entryId") long id,
//			@XoaParam(value = "entry", type = "json") Entry entry);

	@XoaDelete("/entry/{entryId}")
	public ServiceFuture<Void> delete(@XoaParam("entryId") long id);

	@XoaGet("/{authorId}/entrys")
	public ServiceFuture<Entry[]> find(@XoaParam("authorId") long authorId, //
			@XoaParam("offset") int offset, //
			@XoaParam("limit") int limit, //
			@XoaParam("sort") int sort);
}

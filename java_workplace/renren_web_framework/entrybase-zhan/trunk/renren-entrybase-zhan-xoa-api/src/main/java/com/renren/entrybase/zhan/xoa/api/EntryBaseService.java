package com.renren.entrybase.zhan.xoa.api;

import java.util.List;

import com.renren.entrybase.model.Entry;
import com.renren.entrybase.model.Field;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.annotation.XoaDelete;
import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaParam;
import com.renren.xoa.lite.annotation.XoaPost;
import com.renren.xoa.lite.annotation.XoaPut;
import com.renren.xoa.lite.annotation.XoaService;

/**
 * @author root
 * 
 */
@XoaService(serviceId = "entrybase.xoa.renren.com")
public interface EntryBaseService {

	/**
	 * @param entryId
	 *            Entry的gid
	 * @param fields
	 *            Entry的字段
	 * @return {@link ServiceFuture}对象
	 */
	@XoaPost("/entry/{entryId}")
	public ServiceFuture<Void> add(@XoaParam("entryId") long entryId,
			@XoaParam(value = "entry", type = "json") Entry entry);

	/**
	 * @param entryId
	 * @param fields
	 * @return {@link ServiceFuture}对象
	 */
	@XoaPut("/entry/{entryId}")
	public ServiceFuture<Void> update(@XoaParam("entryId") long entryId,
			@XoaParam(value = "fields", type = "json") List<Field> fields,
			@XoaParam("saveIfNotExists") boolean saveIfNotExists);

	/**
	 * 删除指定Entry，即删除所有Field和index
	 * 
	 * @param entryId
	 * @return {@link ServiceFuture}对象
	 */
	@XoaDelete("/entry/{entryId}")
	public ServiceFuture<Void> delete(@XoaParam("entryId") long entryId);

	/**
	 * 获取指定Entry
	 * 
	 * @param entryId
	 * @return {@link ServiceFuture}对象，包含{@link Entry}对象
	 */
	@XoaGet("/entry/{entryId}")
	public ServiceFuture<Entry> get(@XoaParam("entryId") long entryId);
	
	/**
	 * 获取query指定的count
	 * @param query
	 * @return
	 */
	@XoaGet("/entry/count")
	public ServiceFuture<Integer> getCount(@XoaParam("query") String query);

	/**
	 * 批量获取指定Entry
	 * 
	 * @param entryIds
	 * @return {@link ServiceFuture}对象，包含{@link Entry[]}对象
	 */
	@XoaGet("/entry")
	public ServiceFuture<Entry[]> gets(@XoaParam("entryIds") long[] entryIds);
	
	/**
	 * 批量获取指定Entry，指定fields
	 * 
	 * @param entryIds
	 * @return {@link ServiceFuture}对象，包含{@link Entry[]}对象
	 */
	@XoaGet("/entry")
	public ServiceFuture<Entry[]> gets(@XoaParam("entryIds") long[] entryIds, @XoaParam("fields") String[] fields);

	// TODO implement find method
	@XoaGet("/entry/find")
	public ServiceFuture<Entry[]> find(@XoaParam("query") String query,
			@XoaParam("limit") int limit);
}

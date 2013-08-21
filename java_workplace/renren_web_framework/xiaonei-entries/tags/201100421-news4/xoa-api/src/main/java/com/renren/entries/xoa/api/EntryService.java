package com.renren.entries.xoa.api;

import java.util.List;

import com.renren.entries.model.Entry;
import com.renren.entries.model.EntryPage;
import com.renren.entries.model.Field;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.annotation.XoaDelete;
import com.renren.xoa.lite.annotation.XoaGet;
import com.renren.xoa.lite.annotation.XoaParam;
import com.renren.xoa.lite.annotation.XoaPost;
import com.renren.xoa.lite.annotation.XoaPut;
import com.renren.xoa.lite.annotation.XoaService;
import com.xiaonei.commons.gid.util.Type;

/**
 * Entry相关的服务接口
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-3-11 下午03:02:51
 */
@XoaService(serviceId = "entries.xoa.renren.com")
public interface EntryService {

	/**
	 * 保存一个Entry信息，如果entryId代表的Entry不存在，则插入之；
	 * 如果由entryId代表的Entry已经存在了，则增量更新其Field；
	 * 如果要删除一个Field，则需要将这个Field的value设置为null。
	 * 
	 * @param entryId
	 *            Entry的gid
	 * @param fields
	 *            Entry的字段
	 * @return {@link ServiceFuture}对象
	 */
	@XoaPost("/entry/{entryId}")
	public ServiceFuture<Void> save(@XoaParam("entryId") long entryId,
			@XoaParam(value = "fields", type = "json") List<Field> fields);

	/**
	 * 更新指定entry的Field。
	 * 
	 * 如果传入参数中的某Field之前不存在，则插入之； 如果传入参数中的某Field之前已经存在了，则更新之；
	 * 之前存在，但是不在传入参数中的Field保持不变。
	 * 
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
	 * 将一个数字类型的字段值增加1或n，并返回变更后的值
	 * 
	 * 可选参数： dist:增量值，正整数，默认为1
	 * 
	 * @param entryId
	 * @param fieldName
	 * @return
	 */
	@XoaPost("/entry/{entryId}/{fieldName}/increament")
	public ServiceFuture<Long> increaseField(@XoaParam("entryId") long entryId,
			@XoaParam("fieldName") String fieldName);

	/**
	 * 将一个数字类型的字段值减少1或n，并返回变更后的值
	 * 
	 * 可选参数： dist:减量值，正整数，默认为1
	 * 
	 * @param entryId
	 * @param fieldName
	 * @return
	 */
	@XoaPost("/entry/{entryId}/{fieldName}/decreament")
	public ServiceFuture<Long> decreaseField(@XoaParam("entryId") long entryId,
			@XoaParam("fieldName") String fieldName);

	/**
	 * 批量获取指定Entry
	 * 
	 * @param entryIds
	 * @return {@link ServiceFuture}对象，包含{@link Entry[]}对象
	 */
	@XoaGet("/entry")
	public ServiceFuture<Entry[]> gets(@XoaParam("entryId") long[] entryIds);

	/**
	 * 批量获取指定Entry
	 * 
	 * @param entryIds
	 * @return {@link ServiceFuture}对象，包含{@link Entry[]}对象
	 */
	@XoaGet("/entry")
	public ServiceFuture<Entry[]> gets(@XoaParam("entryId") long[] entryIds,
			@XoaParam("fields") String[] fields);

	/**
	 * 
	 * @param query
	 * @return
	 */
	@XoaGet("/entry/count")
	ServiceFuture<Integer> getCount(@XoaParam("query") String query);

	/**
	 * 
	 * @param query
	 * @param limit
	 * @return
	 */
	@XoaGet("/entry/find")
	public ServiceFuture<EntryPage> find(@XoaParam("query") String query,
			@XoaParam("limit") int limit);

	@XoaGet("/entry/find")
	public ServiceFuture<EntryPage> find(@XoaParam("query") Query query,
			@XoaParam("limit") int limit);

	/**
	 * 
	 * @param query
	 * @param cursor
	 * @param limit
	 * @return
	 */
	@XoaGet("/entry/find")
	public ServiceFuture<EntryPage> find(@XoaParam("query") String query,
			@XoaParam("cursor") long cursor, @XoaParam("limit") int limit);

	@XoaGet("/entry/find")
	public ServiceFuture<EntryPage> find(@XoaParam("query") Query query,
			@XoaParam("cursor") long cursor, @XoaParam("limit") int limit);

	/**
	 * 根据query进行查询，外层的and用于取交集，内层的or用于取并集
	 * 
	 * @param query
	 *            查询条件，例如: query =
	 *            "tag:(123 or 456) and create_time:232134-232144"
	 * @param cursor
	 *            如果按照时间倒序来排列检索结果，那么查询是cursor表示所要检索的最大的sequence id，
	 *            返回本次扫描过程中发现的最小的sequence_id
	 * @param limit
	 *            取多少数据
	 * @return {@link ServiceFuture}对象包含{@link FindResult}
	 *         对象，包含本次扫描过程中发现的最小的sequence_id，和entryId数组
	 */
	@XoaGet("/entry/find")
	public ServiceFuture<EntryPage> find(@XoaParam("query") String query,
			@XoaParam("forward") boolean forward,
			@XoaParam("cursor") long cursor, @XoaParam("limit") int limit);

	@XoaGet("/entry/find")
	public ServiceFuture<EntryPage> find(@XoaParam("query") Query query,
			@XoaParam("forward") boolean forward,
			@XoaParam("cursor") long cursor, @XoaParam("limit") int limit);

	/**
	 * 获取entry id序列中的下一个id转化而成的{@link entryId}
	 * 
	 * @return {@link ServiceFuture}对象，包含entry id序列中的下一个id
	 * @param gtype
	 *            指定获取哪个 @ GType} 的entry id，具体的常量定义请参考 @ GType} 类
	 */
	@XoaGet("/entry/id/next")
	public ServiceFuture<Long> nextEntryId(@XoaParam("type") Type type);

}

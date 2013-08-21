package com.renren.entries.core;


import com.renren.entries.model.Entry;
import com.renren.entries.model.EntryPage;
import com.renren.entries.model.Tag;

/**
 * 汇总服务，对外提供接口
 * 
 * @author yanghe.liang@opi-corp.com
 * @author qieqie.wang@opi-corp.com
 */
public interface EntryService {

    /**
     * 更新相应的Field，那些不在fields的其它Field不做任何变更
     * <p>
     * 如果newEntry中的field的值为null表示要删除该Field
     * 
     * @param newEntry
     * @param saveIfNotExists 
     *        当系统中不存在该entry时是否要将newEntry进行保存?true代表是，false代表不保存直接忽略此update
     */
    public void updateEntry(Entry newEntry, boolean saveIfNotExists);

    /**
     * 删除该条目及其索引
     */
    public void deleteEntry(long entryId);

    /**
     * 增量更新数字类型的字段，可以增加也可以减少，减少时distance参数传入负值就可以了
     * 
     * @param entryId
     * @param fieldName
     * @param distance
     * @return
     */
    public long increamentAndGetNumericField(long entryId, String fieldName, long distance);
    
    /**
     * 根据query进行查询，外层的and用于取交集，内层的or用于取并集
     * <p>
     * 例如: query = "tag:(123 or 456) and create_time:232134-232144"
     * 
     * @param forwardOrBackward true代表下一页，false代表上一页
     */
    public EntryPage find(String query, boolean forwardOrBackward, long cursor, int limit);

    /**
     * 获得单个实体
     */
    public Entry get(long entryId);

    /**
     * 批量获取实体
     */
    public Entry[] gets(long[] entries);

    /**
     * 批量获取实体
     */
    public Entry[] gets(long[] entries, String[] fields);

    /**
     * @return 获取entry id序列中的下一个id转化而成的{@link GID}
     * @param type 指定获取哪种type 的entry id
     */
    public long nextEntryId(long type);

    /**
     * 
     * @param namespce
     * @param value
     * @param create
     * @return
     */
    public Tag getTag(String namespace, String value, boolean create);

    /**
     * 
     * @param id
     * @return
     */
    public Tag getTag(int id);

    /**
     * 
     * @param id
     * @return
     */
    public Tag[] getTags(int[] ids);
    
    public void increaseTagCount(int tagId, int inc);
}

package com.xiaonei.ugc.entrysystem;

import java.util.Collections;
import java.util.Date;
import java.util.List;

/**
 * {@link EntrySystem} 是对UGC条目服务的对外抽象，每种类型的UGC都实现一个自己的独特的
 * {@link EntrySystem} 。
 * <p>
 * 本接口的实现应将之作为一个完整的业务进行规划，不仅仅只是数据的操作，比如有些操作可能需要进行站内信、新鲜事的跟随处理、以及自动进行内部数据转移(
 * 回收站)。
 * <p>
 * {@link Entry}代表这个UGC系统中的一个UGC条目。 {@link EntrySystem}在此可看成是 {@link Entry}
 * 的一个集合以及附加在其上的业务。 每一个 {@link Entry} 都有一个 queryId， {@link EntrySystem}
 * 应负责根据UGC条目的ID以及其他信息(如果作者ID)生成该类型UGC的 queryId<br>
 * 如果是使用按作者ID进行分表的，建议参用 “${userId}-${entryId}”组合作为条目的查询ID。
 * <p>
 * 实现时，建议 {@link EntrySystem} 的实现放到 com.xiaonei.ugc.entrysystem.impl.blog
 * 等类似的package下，但这不是必须的规范。
 * 
 * @author zhiliang.wang [zhiliang.wang@opi-corp.com]
 * 
 */
public interface EntrySystem {

    /**
     * 散表数量
     * 
     * @return 1 表示没有散表 其他代表散表数量
     */
    public Integer getPartitionCount();

    /**
     * 通过时间，分页查询本类型的全部UGC条目(不包含被删除放到回收站的)。
     * <p>
     * 因为分表关系无法实现的，请返回 {@link Collections#emptyList()}，请不要抛出
     * {@link UnsupportedOperationException}异常。
     * 
     * @param beginTime 开始时间，可以包含这个时间，非null
     * @param endTime 结束时间，返回的结果不包含这个时间，非null
     * @param firstResult 分页参数之一，第一条记录的在全集中的位置，从0开始计的非负整数
     * @param maxResult 分页参数之一，此次查询返回结果的最大条目。返回的结果数目可能小于这数。
     * @param entryStatus avaliable 有效 unavaliable 无效 all 所有
     * @return 非null的 {@link Entry} 集合，如果查不到符合条件的结果，应返回长度为0的集合
     */
    public List<Entry> findEntries(Date beginTime, Date endTime, EntryStatus entryStatus);

    /**
     * 通过时间，分页查询本类型的全部UGC条目(不包含被删除放到回收站的)。
     * <p>
     * 因为分表关系无法实现的，请返回 {@link Collections#emptyList()}，请不要抛出
     * {@link UnsupportedOperationException}异常。
     * 
     * @param beginTime 开始时间，可以包含这个时间，非null
     * @param endTime 结束时间，返回的结果不包含这个时间，非null
     * @param entryStatus avaliable 有效 unavaliable 无效 all 所有
     * @return 非null的 {@link Entry} 集合，如果查不到符合条件的结果，应返回长度为0的集合
     */
    public List<Entry> findEntries(Date beginTime, Date endTime, int partitionCount,
            EntryStatus entryStatus);

    /**
     * 通过时间，分页查询本类型的全部UGC条目(不包含被删除放到回收站的)。
     * <p>
     * 因为分表关系无法实现的，请返回 {@link Collections#emptyList()}，请不要抛出
     * {@link UnsupportedOperationException}异常。
     * 
     * @param beginTime 开始时间，可以包含这个时间，非null
     * @param endTime 结束时间，返回的结果不包含这个时间，非null
     * @param firstResult 分页参数之一，第一条记录的在全集中的位置，从0开始计的非负整数
     * @param maxResult 分页参数之一，此次查询返回结果的最大条目。返回的结果数目可能小于这数。
     * @return 非null的 {@link Entry} 集合，如果查不到符合条件的结果，应返回长度为0的集合
     */
    public List<Entry> findEntries(Date beginTime, Date endTime, int firstResult, int maxResult);

    /**
     * 返回这个用户产生的UGC条目数(不包含被删除放到回收站的)。
     * <p>
     * 因为分表关系无法实现的，请返回-1，请不要返回0，更不抛出 {@link UnsupportedOperationException}
     * 异常。
     * 
     * @param userId UGC的作者、拥有者ID
     * @return
     */
    public int countEntries(int userId);

    /**
     * 返回这个用户某个时间段内的UGC条目数(不包含被删除放到回收站的)。
     * <p>
     * 
     * @param userId UGC的作者、拥有者ID
     * @param beginTime 开始时间，可以包含这个时间，非null
     * @param endTime 结束时间，返回的结果不包含这个时间，非null
     * @return
     */
    public int countEntries(int userId, Date beginTime, Date endTime);

    /**
     * 分页返回某用户的UGC实体(不包含被删除放到回收站的)。
     * <p>
     * 
     * @param userId UGC的作者、拥有者ID
     * @param firstResult 分页参数之一，第一条记录的在全集中的位置，从0开始计的非负整数
     * @param maxResult 分页参数之一，此次查询返回结果的最大条目。返回的结果数目可能小于这数。
     * @return 非null的 {@link Entry} 集合，如果查不到符合条件的结果，应返回长度为0的集合
     */
    public List<Entry> findEntries(int userId, int firstResult, int maxResult);

    /**
     * 分页返回某用户一段时间内的UGC实体(不包含被删除放到回收站的)。
     * <p>
     * 
     * @param userId UGC的作者、拥有者ID
     * @param beginTime 开始时间，可以包含这个时间，非null
     * @param endTime 结束时间，返回的结果不包含这个时间，非null
     * @param firstResult 分页参数之一，第一条记录的在全集中的位置，从0开始计的非负整数
     * @param maxResult 分页参数之一，此次查询返回结果的最大条目。返回的结果数目可能小于这数。
     * @return 非null的 {@link Entry} 集合，如果查不到符合条件的结果，应返回长度为0的集合
     */
    public List<Entry> findEntries(int userId, Date beginTime, Date endTime, int firstResult,
            int maxResult);

    /**
     * 返回指定查询ID的UGC条目内容。
     * <p>
     * 
     * @param queryId UGC条目查询ID，根据分表策略这个查询ID应该由2个元素组成：用户ID和条目ID
     * @return
     * @see Entry#getQueryId()
     */
    public Entry getEntry(String queryId);

    /**
     * 返回当前总共需要被审核的UGC条目数。
     * <p>
     * 
     * @return
     */
    public int countReviewingEntries();

    /**
     * 分页返回当前需要被审核的UGC条目。
     * <p>
     * 
     * @param firstResult 分页参数之一，第一条记录的在全集中的位置，从0开始计的非负整数
     * @param maxResult 分页参数之一，此次查询返回结果的最大条目。返回的结果数目可能小于这数。
     * @return 非null的 {@link Entry} 集合，如果查不到符合条件的结果，应返回长度为0的集合
     */
    public List<Entry> findReviewingEntries(int firstResult, int maxResult);

    /**
     * 确认并发布一个待审核的UGC条目。
     * <p>
     * 
     * @param queryId 该条目的查询ID
     * @param publishUserId 发布者
     * @param publishNote 发布说明(如果没有填写null或空串)
     * @return true发布成功
     */
    public boolean publishEntry(String queryId, int publishUserId, String publishNote);

    /**
     * 删除指定查询ID的UGC条目。
     * <p>
     * 如果该条目具有可恢复能力，实现方应该在这个方法里实现放到“回收站”逻辑
     * 
     * @param queryId
     * @return true 删除成功
     */
    public boolean deleteEntry(String queryId);

    /**
     * 批量删除指定用户的所有UGC条目。
     * <p>
     * 如果这些条目具有可恢复能力，实现方应该在这个方法里实现放到“回收站”逻辑
     * 
     * @param userId UGC的作者、拥有者ID
     * @return 0或正数代表删除了的UGC条目个数；-1表示未知数目。
     */
    public int deleteEntries(int userId);

    /**
     * 批量删除指定用户的某时间段内的UGC条目。
     * <p>
     * 如果这些条目具有可恢复能力，实现方应该在这个方法里实现放到“回收站”逻辑
     * 
     * @param userId UGC的作者、拥有者ID
     * @param beginTime 开始时间，可以包含这个时间，非null
     * @param endTime 结束时间，返回的结果不包含这个时间，非null
     * @return 0或正数代表删除了的UGC条目个数；-1表示未知数目。
     */
    public int deleteEntries(int userId, Date beginTime, Date endTime);

    /**
     * 批量删除指定查询ID的所有UGC条目。
     * <p>
     * 如果这些条目具有可恢复能力，实现方应该在这个方法里实现放到“回收站”逻辑
     * 
     * @param queryIdList UGC条目queryId数组
     * @return 0或正数代表删除了的UGC条目个数；-1表示未知数目。
     */
    public int deleteEntries(String[] queryIdList);

    /**
     * 返回某用户被删除的放到“回收站”的UGC条目数，这些条目对公众不可见，但或许通过“restore/恢复”操作又对外可见。
     * <p>
     * 
     * @param userId UGC条目的作者、拥有者
     * @return
     */
    public int countRecyclingEntries(int userId);

    /**
     * 从回收站恢复/还原指定一个查询ID对应的UGC实体，使对公众可见(具体视隐私策略规定)
     * <p>
     * 
     * @param queryId UGC条目查询ID
     * @return true 还原成功
     */
    public boolean restoreEntry(String queryId);

    /**
     * 从回收站，恢复/还原某指定UGC，使对公众可见(具体视隐私策略规定)。
     * <p>
     * 
     * @param queryIdList UGC条目queryId数组
     * @return 0或正数代表还原成功的UGC条目个数；-1表示未知数目。
     */
    public int restoreEntries(String[] queryIdList);

    /**
     * 返回某用户被删除的放到“回收站”的UGC条目，这些条目对公众不可见，但或许通过“restore/恢复”操作又对外可见。
     * <p>
     * 
     * @param firstResult 分页参数之一，第一条记录的在全集中的位置，从0开始计的非负整数
     * @param maxResult 分页参数之一，此次查询返回结果的最大条目。返回的结果数目可能小于这数。
     * @return
     */
    public List<Entry> findRecyclingEntries(int userId, int firstResult, int maxResult);

    /**
     * 从回收站，全部恢复/还原某人的所有UGC，使对公众可见(隐私策略如果允许的话)。
     * <p>
     * 
     * @param userId UGC作者、拥有者ID
     * @return 0或正数代表还原成功的UGC条目个数；-1表示未知数目。
     */
    public int restoreEntries(int userId);

    /**
     * 从回收站，全部恢复/还原某人的某时间内的UGC，使对公众可见(具体视隐私策略规定)。
     * <p>
     * 
     * @param userId UGC作者、拥有者ID
     * @param beginTime 开始时间，可以包含这个时间，非null
     * @param endTime 结束时间，返回的结果不包含这个时间，非null
     * @return 0或正数代表还原成功的UGC条目个数；-1表示未知数目。
     */
    public int restoreEntries(int userId, Date beginTime, Date endTime);
}

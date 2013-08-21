package com.renren.entries.xoa.controllers;

import net.paoding.rose.web.annotation.DefValue;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.rest.Delete;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.annotation.rest.Put;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.entries.core.EntryService;
import com.renren.entries.model.Entry;
import com.renren.entries.model.EntryPage;
import com.renren.entries.model.Field;
import com.renren.xoa.commons.annotation.JsonParam;
import com.xiaonei.commons.gid.util.G;
import com.xiaonei.commons.gid.util.Type;

/**
 * 对外提供的xoa接口
 * 
 * @author yanghe.liang@opi-corp.com
 * @author weibo.li@opi-corp.com
 */
public class EntryController {

    @Autowired
    private EntryService entryService;

    /**
     * 如果原先不存在则新增之，原先存在则完全更新之（等价于先删除后新增）
     * 
     * @param entryId 统一id
     * @param fields key-value对应的数组
     * @throws EntryException
     */
    @Post("{entryId:\\d+}")
    public void save(@Param("entryId") long entryId, @JsonParam @Param("fields") Field[] fields) {
        Entry entry = new Entry(entryId, fields);
        entryService.updateEntry(entry, true);
    }

    /**
     * 更新相应的Field，那些不在fields的其它Field不做任何变更
     * 
     * @param entryId 统一id
     * @param fields key-value对应的数组
     * @throws EntryException
     */
    @Put("{entryId:\\d+}")
    public void update(@Param("entryId") long entryId, @JsonParam @Param("fields") Field[] fields,
            @Param("saveIfNotExists") boolean saveIfNotExists) {
        Entry entry = new Entry(entryId, fields);
        entryService.updateEntry(entry, saveIfNotExists);
    }

    /**
     * 删除该条目及其索引
     * 
     * @param entryId 统一id
     * @throws EntryException
     */
    @Delete("{entryId:\\d+}")
    public void delete(@Param("entryId") long entryId) {
        if (entryId == 0) {
            throw new IllegalArgumentException("entryId不能为0");
        }
        entryService.deleteEntry(entryId);
    }

    /**
     * 查询单个实体
     * 
     * @param entryId 统一id
     * @return
     * @throws EntryException
     */
    @Get("{entryId:\\d+}")
    public Entry get(@Param("entryId") long entryId) {
        if (entryId == 0) {
            throw new IllegalArgumentException("entryId不能为0");
        }
        Entry entry = entryService.get(entryId);
        return entry;
    }

    /**
     * 批量查询实体
     * 
     * @param entryIds 统一id数组
     * @return
     * @throws EntryException
     */
    @Get("")
    public Entry[] gets(@Param("entryId") long[] entryIds, @Param("fields") String[] fields) {
        if (entryIds == null) {
            throw new IllegalArgumentException("entryId数组不能为空");
        }
        if (fields == null || fields.length == 0) {
            return entryService.gets(entryIds);
        }
        return entryService.gets(entryIds, fields);
    }

    /**
     * 根据query进行查询，外层的and用于取交集，内层的or用于取并集
     * 
     * @param query 查询条件，例如: query =
     *        "tag:(123 or 456) and create_time:232134-232144"
     * @param curosr 如果按照时间倒序来排列检索结果，那么查询是cursor表示所要检索的最大的sequence id，
     *        返回本次扫描过程中发现的最小的sequence id
     * @param limit 取多少数据
     * @return
     * @throws EntryException
     */
    @Get("find")
    public EntryPage find(@Param("query") String query,//
            @Param("forward") @DefValue("true") boolean forward,//
            @Param("cursor") @DefValue("-1") long cursor,//
            @Param("limit") @DefValue("25") int limit) {
        if (query == null || "".equals(query) || "".equals(query.trim())) {
            throw new IllegalArgumentException("query不能为空");
        }
        if (cursor < 0) {
            cursor = forward ? Long.MAX_VALUE : 0;
        }
        EntryPage page = entryService.find(query, forward, cursor, limit);
        return page;
    }

    /**
     * 
     * 根据指定的type获取entry id序列中的下一个id转化而成的{@link G}
     * 
     * @return
     * @param gtype {@link GType}值
     */
    @Get("id/next")
    public long nextEntryId(@Param("type") Type type) {
        return entryService.nextEntryId(type.value);
    }

}

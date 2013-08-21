package com.renren.entries.xoa.controllers;

import java.util.ArrayList;

import net.paoding.rose.web.annotation.DefValue;
import net.paoding.rose.web.annotation.IfParamExists;
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
import com.renren.entries.model.Hit;
import com.renren.entrybase.zhan.xoa.api.EntryBaseService;
import com.renren.xoa.commons.annotation.JsonParam;
import com.renren.xoa.lite.ServiceFutureHelper;
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

    @Autowired
    private EntryBaseService entryBaseService;

    // @IfParamExists("lifecycle=contribution")
    // @Post("{entryId:\\d+}")
    // public void doubleAdd(@Param("entryId") long entryId, @JsonParam
    // @Param("fields") Field[] fields) {
    // // save(entryId, fields);
    // // request.
    // // com.renren.entrybase.model.Entry newEntry = new
    // com.renren.entrybase.model.Entry();
    // // for(Field field : fields){
    // // newEntry.addField(field.getName(), field.getValue());
    // // }
    // // entryBaseService.add(entryId,
    // newEntry.getFields()).submit().awaitUninterruptibly();
    // }

    /**
     * 如果原先不存在则新增之，原先存在则完全更新之（等价于先删除后新增）
     * 
     * @param entryId 统一id
     * @param fields key-value对应的数组
     * @throws EntryException
     */
    @Post("{entryId:\\d+}")
    public void save(@Param("entryId") long entryId, @Param("lifecycle") String lifecycle,
            @JsonParam @Param("fields") Field[] fields) {
        if (lifecycle == null || lifecycle.equals("") || lifecycle.equals("publication")) {
            Entry entry = new Entry(entryId, fields);
            entryService.updateEntry(entry, true);
        } else if (lifecycle.equals("contribution")) {
            com.renren.entrybase.model.Entry newEntry = this.entryOldToNew(new Entry(entryId,
                    fields), entryId);
            ServiceFutureHelper.executeWithThroughputControl(entryBaseService
                    .add(entryId, newEntry), "add", 100, 2000);
        }
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
            throw new IllegalArgumentException("entryId can not be 0");
        }
        entryService.deleteEntry(entryId);
    }

    @Delete("{entryId:\\d+}")
    @IfParamExists("lifecycle=contribution")
    public void deleteContribution(@Param("entryId") long entryId) {
        if (entryId == 0) {
            throw new IllegalArgumentException("entryId can not be 0");
        }
        ServiceFutureHelper.executeWithThroughputControl(entryBaseService.delete(entryId), "delete", 50, 2000);
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
            throw new IllegalArgumentException("entryId can not be 0");
        }
        Entry entry = entryService.get(entryId);
        return entry;
    }

    @Get("{entryId:\\d+}")
    @IfParamExists("lifecycle=contribution")
    public Entry getContribution(@Param("entryId") long entryId) {
        com.renren.entrybase.model.Entry newEntry = ServiceFutureHelper
                .executeWithThroughputControl(entryBaseService.get(entryId), "get", 50, 2000);
        Entry oldEntry = this.entryNewToOld(newEntry, entryId);
        return oldEntry;
    }

    @Post("/{entryId:\\d+}/{fieldName}/increament")
    public long increaseField(@Param("entryId") long entryId, @Param("fieldName") String fieldName,
            @Param("dist") int dist) {
        if (dist < 1) {
            dist = 1;
        }
        return entryService.increamentAndGetNumericField(entryId, fieldName, dist);
    }

    @Post("/{entryId:\\d+}/{fieldName}/decreament")
    public long decreaseField(@Param("entryId") long entryId, @Param("fieldName") String fieldName,
            @Param("dist") int dist) {
        if (dist < 1) {
            dist = 1;
        }
        return entryService.increamentAndGetNumericField(entryId, fieldName, -1 * dist);
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
            throw new IllegalArgumentException("entryId[] can not be null");
        }
        if (fields == null || fields.length == 0) {
            return entryService.gets(entryIds);
        }
        return entryService.gets(entryIds, fields);
    }

    @Get("")
    @IfParamExists("lifecycle=contribution")
    public Entry[] getsContribution(@Param("entryId") long[] entryIds,
            @Param("fields") String[] fields) {
        com.renren.entrybase.model.Entry[] newEntries = ServiceFutureHelper
                .executeWithThroughputControl(entryBaseService.gets(entryIds), "get", 50, 2000);
        Entry[] oldEntries = new Entry[newEntries.length];
        int i = 0;
        for (com.renren.entrybase.model.Entry newEntry : newEntries) {
            if (newEntry != null) {
                oldEntries[i] = this.entryNewToOld(newEntry, newEntry.getId());
            }
            i++;
        }
        return oldEntries;
    }

    @Get("count")
    public String count(@Param("query") String query) {
//        if (query == null || "".equals(query) || "".equals(query.trim())) {
//            throw new IllegalArgumentException("query不能为空");
//        }
//        return entryService.count(query);
        return "@to be implement";
    }
    
    @Get("count")
    @IfParamExists("lifecycle=contribution")
    public int countContribution(@Param("query") String query) {
        if (query == null || "".equals(query) || "".equals(query.trim())) {
            throw new IllegalArgumentException("query cannot be null");
        }
        return ServiceFutureHelper.executeWithThroughputControl(entryBaseService.getCount(query),
                "getCount", 50, 1000);
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
            throw new IllegalArgumentException("query can not be null");
        }
        if (cursor < 0) {
            cursor = forward ? Long.MAX_VALUE : 0;
        }
        EntryPage page = entryService.find(query, forward, cursor, limit);
        return page;
    }

    @Get("find")
    @IfParamExists("lifecycle=contribution")
    public EntryPage findContribution(@Param("query") String query,//
            @Param("forward") @DefValue("true") boolean forward,//
            @Param("cursor") @DefValue("-1") long cursor,//
            @Param("limit") @DefValue("25") int limit) {
        if (query == null || "".equals(query) || "".equals(query.trim())) {
            throw new IllegalArgumentException("query can not be null");
        }
        if (cursor < 0) {
            cursor = forward ? Long.MAX_VALUE : 0;
        }
        EntryPage page = new EntryPage();
        com.renren.entrybase.model.Entry[] newPage = ServiceFutureHelper
                .executeWithThroughputControl(entryBaseService.find(query, limit), "find", 100,
                        2000);
        ArrayList<Hit> hits = new ArrayList<Hit>(newPage.length);
        for (int i = 0; i < newPage.length; i++) {
            com.renren.entrybase.model.Entry newEntry = newPage[i];
            Hit hit = new Hit();
            hit.setCursor(newEntry.getSequence());
            hit.setEntryId(newEntry.getId());
            hits.add(hit);
        }
        page.setHits(hits);
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

    private Entry entryNewToOld(com.renren.entrybase.model.Entry newEntry, long entryId) {
        if (newEntry == null) {
            return null;
        }
        Entry oldEntry = new Entry();
        oldEntry.setId(entryId);
        for (com.renren.entrybase.model.Field field : newEntry.getFields()) {
            oldEntry.addField(new Field(field.getName(), field.getValue()));
        }
        return oldEntry;
    }

    private com.renren.entrybase.model.Entry entryOldToNew(Entry oldEntry, long entryId) {
        if (oldEntry == null) {
            return null;
        }
        com.renren.entrybase.model.Entry newEntry = new com.renren.entrybase.model.Entry();
        newEntry.setId(entryId);
        for (Field field : oldEntry.getFields()) {
            // newEntry.addField(field.getName(), field.getValue());
            newEntry.addField(new com.renren.entrybase.model.Field(field.getName(), field
                    .getValue()));
        }
        return newEntry;
    }

}

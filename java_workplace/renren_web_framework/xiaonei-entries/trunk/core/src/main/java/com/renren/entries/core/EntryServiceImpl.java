package com.renren.entries.core;

import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.entries.core.dao.BodyDAO;
import com.renren.entries.core.dao.CachedInfoDAO;
import com.renren.entries.core.dao.IndexDAO;
import com.renren.entries.core.dao.SeqDAO;
import com.renren.entries.core.query.Query;
import com.renren.entries.core.query.QueryService;
import com.renren.entries.core.query.parser.ParseException;
import com.renren.entries.core.query.parser.QueryParser;
import com.renren.entries.core.tag.TagService;
import com.renren.entries.model.Entry;
import com.renren.entries.model.EntryPage;
import com.renren.entries.model.Field;
import com.renren.entries.model.Tag;
import com.xiaonei.commons.gid.util.G;
import com.xiaonei.commons.gid.util.SubType;
import com.xiaonei.commons.gid.util.Type;

//DONE: 各种field为null的判断；服务器决定index的逻辑；log4j日志；
//DONE: field的排序，比如master在前，tag在后
//DONE: 上一页的支持
//DONE: Entry.toInfoString要不要对fields进行排序？
//TODO: 索引表(调用外部命令)自动建立?--不要！
//TODO: 日志(万一数据库操作错了我如何处理)?
//TODO: 哪些项目能掉用，哪些项目不能调用能控制否？--这里的调用指增加、删除
//TODO: 如果fields长度超过某个数值如何处理？
//TODO: query字段的value如果是一个区间值的还未搞

//TODO: 非索引字段的查找支持
//TODO: cache的加入-info已经完成，body呢
//DONE: entry类型查询的支持
//TODO: count功能的实现
//TODO: 好友的feed怎么查？
//TODO: 数据库查询时间监控、传输监控等等！
//TODO: isNull等查询的支持
//TODO: photo、album要全部接入到信息进来，靠谱否？要确认：传玉把album修改为parent了否；同时photo弃resin改tomcat!
//TODO: info长度的检查
//TODO: 要能够让信息聚合多实例部属，有大的系统，也可各种独立系统！
//TODO: 要使用信息聚合独立于人人的任何系统(我们可以提供core包，以及简化的spdy服务和客户端)，要开源出来!
//TODO: query的日志要带上发生时间和耗费吗？
//TODO: sub_type是否应该索引，否则查询太慢！非得要从cache取出entry，则应该改为使用getMults批量取
//TODO: 有些字段系统可支持索引，但如果客户端没有标注需要索引默认就是不索引，这和author不一样
//TODO: 要限制xoa-api可使用的field字段名字！即部提供String的接口，只提供Query.add的接口
//TODO: 更新和插入时候要对Field的数据验证
/**
 * 汇总服务，对外提供接口
 * 
 * @author yanghe.liang@opi-corp.com
 * @author qieqie.wang@opi-corp.com
 */
public class EntryServiceImpl implements EntryService {

    private static Log logger = LogFactory.getLog(EntryServiceImpl.class);

    private IndexDAO indexDAO;

    private BodyDAO bodyDAO;

    private CachedInfoDAO infoDAO;

    private SeqDAO seqDAO;

    private QueryService queryService;

    private TagService tagService;

    private FieldConstraints fieldConstraints;

    private transient boolean infoEnabled;

    public EntryServiceImpl() {
        infoEnabled = logger.isInfoEnabled();
    }

    @Autowired
    public void setFieldConstraints(FieldConstraints fieldConstraints) {
        this.fieldConstraints = fieldConstraints;
    }

    @Autowired
    public void setIndexDAO(IndexDAO indexDAO) {
        this.indexDAO = indexDAO;
    }

    @Autowired
    public void setBodyDAO(BodyDAO bodyDAO) {
        this.bodyDAO = bodyDAO;
    }

    @Autowired
    public void setInfoDAO(CachedInfoDAO infoDAO) {
        this.infoDAO = infoDAO;
    }

    @Autowired
    public void setSeqDAO(SeqDAO seqDAO) {
        this.seqDAO = seqDAO;
    }

    @Autowired
    public void setQueryService(QueryService queryService) {
        this.queryService = queryService;
    }

    @Autowired
    public void setTagService(TagService tagService) {
        this.tagService = tagService;
    }

    @Override
    public void updateEntry(Entry newEntry, boolean saveIfNotExists) {
        // 删除对外外应用不可更新的field(比如以点号开始的、名称为null的等等字段)
        checkUserFields(newEntry);

        // 存储中已有的entry对象
        Entry oldEntry = infoDAO.get(newEntry.getId());
        if (oldEntry == null) {
            if (!saveIfNotExists) {
                return;
            }
        } else {
            // 把将要更新的条目补充完整
            fullfillNewEntry(newEntry, oldEntry);
        }

        // 更新info和body数据
        saveEntryContent(oldEntry, newEntry);

        // 更新index数据
        updateEntryIndex(oldEntry, newEntry);
    }

    @Override
    public void deleteEntry(long entryId) {
        Entry entry = infoDAO.get(entryId);
        if (entry == null) {
            return;
        }
        // 先删index与body
        for (Field field : entry.getFields()) {
            if (field == null) {
                continue;
            }
            if (field.getIndex(false) && fieldConstraints.supportsIndex(field.getName())) {
                indexDAO.delete(field.getName(), field.getValue(), entryId);
            }
            if (field.isNamed(".body.length") && field.hasValue()) {
                bodyDAO.delete(entryId);
            }
        }
        // 后删info
        infoDAO.delete(entryId);
    }

    @Override
    public EntryPage find(String query, boolean forwardOrBackward, long cursor, int limit) {
        long start = System.currentTimeMillis();
        Query queryObject;
        try {
            QueryParser queryParser = new QueryParser(new StringReader(query));
            queryObject = queryParser.parse();
        } catch (ParseException e) {
            throw new IllegalArgumentException(e);
        }
        EntryPage page = queryService.doQuery(queryObject, forwardOrBackward, cursor, limit);
        long end = System.currentTimeMillis();
        //
        if (infoEnabled) {
            String cursorString;
            if (cursor != Long.MAX_VALUE) {
                cursorString = String.valueOf(cursor);
            } else {
                cursorString = "Long.MAX_VALUE";
            }
            logger.info("[" + getCostString(start, end) + "ms] find " + page.getHits().size()
                    + " entries for: " + queryObject + " ("
                    + (forwardOrBackward ? "forward" : "backward") + ", " + cursorString + ", "
                    + limit + ")");
        }
        return page;
    }

    private String getCostString(long start, long end) {
        long cost = (end - start);
        String costString;
        if (cost < 10) {
            costString = "00" + cost;
        } else if (cost < 100) {
            costString = "0" + cost;
        } else {
            costString = String.valueOf(cost);
        }
        return costString;
    }

    @Override
    public Entry get(long entryId) {
        return get(entryId, true);
    }

    protected Entry get(long entryId, boolean loadbody) {
        Entry entry = infoDAO.get(entryId);
        if (entry == null) {
            return null;
        }
        if (loadbody) {
            Field fieldBody = entry.getField(".body.length");
            if (fieldBody != null && fieldBody.hasValue()) {
                fieldBody.setName("body");
                fieldBody.setValue(bodyDAO.get(entryId));
            }
        }
        return entry;
    }

    @Override
    public Entry[] gets(long[] entries) {
        Entry[] array = this.infoDAO.gets(entries);
        for (int i = 0; i < array.length; i++) {
            Entry entry = array[i];
            if (entry != null) {
                Field fieldBody = entry.getField(".body.length");
                if (fieldBody != null && fieldBody.hasValue()) {
                    fieldBody.setName("body");
                    fieldBody.setValue(bodyDAO.get(entry.getId()));
                }
            }
        }
        return array;
    }

    @Override
    public Entry[] gets(long[] entries, String[] fields) {
        boolean loadbody = ArrayUtils.indexOf(fields, "body") != -1;
        Entry[] array = new Entry[entries.length];
        for (int i = 0; i < array.length; i++) {
            array[i] = get(entries[i], loadbody);
        }
        for (Entry entry : array) {
            for (Iterator<Field> iter = entry.getFields().iterator(); iter.hasNext();) {
                Field f = iter.next();
                if (!f.getName().startsWith(".") && ArrayUtils.indexOf(fields, f.getName()) < 0) {
                    iter.remove();
                }
            }
        }
        return array;
    }

    @Override
    public long nextEntryId(long typeValue) {
        Type type = Type.toType(typeValue);
        if (type == Type.ENTRY_INDEX) {
            long seqId = seqDAO.nextIndexId();
            return G.id(seqId, type);
        } else {
            long seqId = seqDAO.nextEntryId() | (1L << 43);
            return G.id(seqId, type);
        }
    }

    @Override
    public Tag getTag(int id) {
        return tagService.getTag(id);
    }

    @Override
    public Tag getTag(String namespace, String value, boolean create) {
        if (value == null) {
            throw new NullPointerException("value");
        }
        return tagService.getTag(namespace, value, create);
    }

    @Override
    public Tag[] getTags(int[] ids) {
        return tagService.getTags(ids);
    }

    //---------------------------------------------------------------

    /**
     * 删除对外外应用不可更新的field(比如以点号开始的、名称为null的等等字段)、按照服务器的规定设置字段是否索引标志
     * 
     * @param entry
     */
    private void checkUserFields(Entry entry) {
        for (Iterator<Field> iterator = entry.getFields().iterator(); iterator.hasNext();) {
            Field f = iterator.next();
            if (f == null) {
                iterator.remove();
                continue;
            }
            if (f.getName() == null) {
                throw new NullPointerException("field name is required; entry:" + entry);
            }
            if (f.getName().startsWith(".")) {
                iterator.remove();
                continue;
            }
            if (fieldConstraints.getIndex(f)) {
                f.setIndex(Field.INDEX_TRUE);
            } else {
                f.setIndex(Field.INDEX_FALSE);
            }
        }
    }

    /**
     * 把将要更新的条目补充完整
     * 
     * @param newEntry
     * @param oldEntry
     */
    private void fullfillNewEntry(Entry newEntry, Entry oldEntry) {
        Set<String> newFieldNames = new HashSet<String>(); // 统计要被替换掉的key
        for (Field newField : newEntry.getFields()) {
            newFieldNames.add(newField.getName());
        }

        // 没有在此次更新之列的老字段，要加入到fields一会去重新
        for (Field oldField : oldEntry.getFields()) {
            String oldName = oldField.getName();
            if (!oldName.startsWith(".") && !newFieldNames.contains(oldName)) {
                // 之前有索引，但现在不再支持索引了，修正！
                if (oldField.getIndex(false) && !fieldConstraints.supportsIndex(oldField.getName())) {
                    oldField = oldField.clone();
                    oldField.setIndex(Field.INDEX_FALSE);
                }
                newEntry.addField(oldField);
            }
        }
    }

    /**
     * 存储一个条目信息，即info及其body
     * <p>
     * 
     * @param oldEntry
     * @param newEntry
     * @param newBody 本次保存是否要保存或变更的body，如果newBody为null表示不用更新body
     */
    private void saveEntryContent(Entry oldEntry, Entry newEntry) {
        long start = System.currentTimeMillis();
        String dotType = G.extractType(newEntry.getId()).toString();
        Field subTypeField = newEntry.getField("subtype");
        if (subTypeField != null) {
            dotType = dotType + "." + SubType.toType(subTypeField.getLongValue());
        }
        newEntry.addField(".type", dotType);
        Field newBody = newEntry.getField("body");
        // newBody非null，那就更新body表
        if (newBody != null) {
            if (!newBody.hasValue()
                    && (oldEntry != null && oldEntry.getField(".body.length") != null)) {
                bodyDAO.delete(newEntry.getId());
            } else {
                bodyDAO.save(newEntry.getId(), newBody.getValue());
            }

            // 必须让newEntry中没有name为body的字段！
            if (newBody != null && newBody.hasValue()) {
                Field bodylen = new Field(".body.length", newBody.getValue().length());
                newEntry.replaceField("body", bodylen);
            }
        } else {
            // body保持不变(所以要从oldEntry中将.body.length拷贝进来)
            Field oldBodylen = oldEntry == null ? null : oldEntry.getField(".body.length");
            if (oldBodylen != null) {
                newEntry.addField(oldBodylen);
            }
        }
        String infoString = Entry.toInfoString(newEntry.getFields());
        if (infoString.length() > 8000) {
            logger.warn("infoString length has reached " + infoString.length()
                    + ", please check the length of column entry_info.value");
            // TODO: 这里既然要抛出异常了，那就应该也拒绝上面的body.delete或者save
            if (infoString.length() > 10000) {
                throw new IllegalArgumentException("entry_info is to long: "
                        + infoString.substring(0, 255) + "...");
            }
        }
        infoDAO.save(newEntry.getId(), infoString);
        if (infoEnabled) {
            long end = System.currentTimeMillis();
            int limit = infoString.length() > 100 ? 100 : infoString.length();
            logger.info("[" + getCostString(start, end) + "ms] saved: " + newEntry.getId()
                    + ", type=" + G.extractType(newEntry.getId()) + ", length="
                    + infoString.length() + ":" + infoString.substring(0, limit) + " ... ");
        }
    }

    private void updateEntryIndex(Entry oldEntry, Entry newEntry) {
        long entryId = newEntry.getId();

        // 要新增的索引字段，从newEntry中选择获取
        List<Field> fieldsToBeAdded;
        if (oldEntry == null) {
            fieldsToBeAdded = newEntry.getFields();
        } else {
            // 通过for循环newEntry提取，一开始长度为0，
            fieldsToBeAdded = new ArrayList<Field>(newEntry.getFields().size());

            // 要删除的索引字段，从oldEntry中选择获取，一开始长度为0
            ArrayList<Field> fieldsToBeDeleted = new ArrayList<Field>(oldEntry.getFields().size());

            // 
            for (Iterator<Field> iter = newEntry.getFields().iterator(); iter.hasNext();) {
                Field newField = iter.next();
                String newFieldName = newField.getName();
                if (newFieldName.equals("body") || newFieldName.startsWith(".")) {
                    continue;
                }
                // 新字段索引不？newFiledIndexFlag为true就是要
                boolean newFiledIndexFlag = newField.getIndex(false);// newField的index标识已经在checkUserFields方法中修正了，这里只需要getIndex(false)即可！

                // 但这个(name, value)对是否已经是索引的了？
                Field oldField = oldEntry.getField(newFieldName, newField.getValue());

                // 之前没有索引的且现在要加入索引的，则加入toBeAdded
                if (oldField == null || !oldField.getIndex(false)) {
                    if (newFiledIndexFlag) {
                        fieldsToBeAdded.add(newField);
                    }
                }
                // 之前有索引的，现在不索引的，则加入toBeDeleted
                else if (!newFiledIndexFlag) {
                    // 之前有索引，但索引不再支持了(比如整个索引表truncated了)，此时也就不用再delete了
                    if (fieldConstraints.supportsIndex(oldField.getName())) {
                        fieldsToBeDeleted.add(oldField);
                    }
                }
            }

            // 删除多余的旧索引
            for (Field oldField : fieldsToBeDeleted) {
                indexDAO.delete(oldField.getName(), oldField.getValue(), entryId);
            }

        }

        // 添加新索引
        for (Field newField : fieldsToBeAdded) {
            if (newField.getIndex(false)) {
                long id = nextEntryId(Type.ENTRY_INDEX.value);
                indexDAO.add(newField.getName(), id, newField.getValue(), entryId);
            }
        }
    }
}

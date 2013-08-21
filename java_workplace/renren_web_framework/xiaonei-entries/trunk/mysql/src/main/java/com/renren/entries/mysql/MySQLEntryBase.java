package com.renren.entries.mysql;

import java.sql.Date;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import javax.annotation.PostConstruct;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.entries.EntryBase;
import com.renren.entries.event.EntryEventListener;
import com.renren.entries.model.Entry;
import com.renren.entries.model.Field;
import com.renren.entries.model.query.QField;
import com.renren.entries.model.query.QValue;
import com.renren.entries.model.query.Query;
import com.renren.entries.mysql.dao.EntryDAO;
import com.renren.entries.mysql.dao.IndexDAO;
import com.renren.entries.mysql.dao.SeqDAO;
import com.renren.entries.mysql.dao.ShardDAO;
import com.renren.entries.mysql.model.FieldConstraints;
import com.renren.entries.mysql.model.Range;

//DONE: 各种field为null的判断；服务器决定index的逻辑；log4j日志；
//DONE: field的排序，比如master在前，tag在后
//DONE: 上一页的支持
//DONE: Entry.toInfoString要不要对fields进行排序？
//DONE: 非索引字段的查找支持
//TODO: 索引表(调用外部命令)自动建立?--不要！
//TODO: 日志(万一数据库操作错了我如何处理)?
//TODO: 哪些项目能掉用，哪些项目不能调用能控制否？--这里的调用指增加、删除
//TODO: 如果fields长度超过某个数值如何处理？
//TODO: query字段的value如果是一个区间值的还未支持

//TODO: cache的加入
//DONE: entry类型查询的支持（通过idRange支持）
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
//TODO: shardField从properties配置

/**
 * 必设: {@link #indexDAO}、{@link #entryDAO}、{@link #shardDAO}、
 * {@link #seqDAO}、{@link #fieldConstraints}
 * <p>
 * 可选: {@link #infoLengthLimit}、 {@link #infoLengthWarnning}
 * <p>
 * PostConstruct: {@link #postConstruct()}
 * 
 * @author yanghe.liang@opi-corp.com
 * @author qieqie.wang@opi-corp.com
 */
public class MySQLEntryBase implements EntryBase {

    /**
     * 记录info字段警告信息之用
     * 
     * @see #updateEntryInfo(long, Entry, Entry)
     */
    private static Log loggerOfInfo = LogFactory.getLog(MySQLEntryBase.class + ".info");

    /**
     * 变更条目日志
     */
    private static Log loggerOfUpdate = LogFactory.getLog(MySQLEntryBase.class + ".update");

    /**
     * 查询条目日志
     */
    private static Log loggerOfQuery = LogFactory.getLog(MySQLEntryBase.class + ".query");

    /**
     * 当info串长度超过这个限制时，抛出异常
     */
    private int infoLengthLimit = 9000;

    /**
     * 当info串长度超过这个限制时，在日志中做警告
     */
    private int infoLengthWarnning = 8000;

    /** 注册的事件侦听器 */
    private ArrayList<EntryEventListener> listeners = new ArrayList<EntryEventListener>();

    /**
     * 字段约束信息
     */
    private FieldConstraints fieldConstraints;

    /**
     * 主散字段名称，默认为null表示按照entry的id散
     * <p>
     * 如果非null，在新增entry时侯必须提供该字段的值进来
     */
    private String shardField;

    /**
     * 索引表访问接口
     */
    private IndexDAO indexDAO;

    /**
     * 实体表访问接口
     */
    private EntryDAO entryDAO;

    /**
     * shard表访问接口，所谓shard表按照entry的id来散，用于记录某个entry的散表值<br>
     * 因此当只提供一个id时，也能够查询出entry出来
     * <p>
     * 当entry是使用id散表时，shard表不记录，不用操作shardDAO
     */
    private ShardDAO shardDAO;

    /**
     * 分布式下保证entry.sequence的一个措施
     */
    private SeqDAO seqDAO;

    /**
     * query查询逻辑
     */
    private QueryService queryService;

    //-----------------------

    public MySQLEntryBase() {
    }

    @Autowired
    public void setShardDAO(ShardDAO shardDAO) {
        this.shardDAO = shardDAO;
    }

    public ShardDAO getShardDAO() {
        return shardDAO;
    }

    @Autowired
    public void setFieldConstraints(FieldConstraints fieldConstraints) {
        this.fieldConstraints = fieldConstraints;
    }

    public FieldConstraints getFieldConstraints() {
        return fieldConstraints;
    }

    @Autowired
    public void setIndexDAO(IndexDAO indexDAO) {
        this.indexDAO = indexDAO;
    }

    public IndexDAO getIndexDAO() {
        return indexDAO;
    }

    @Autowired
    public void setEntryDAO(EntryDAO entryDAO) {
        this.entryDAO = entryDAO;
    }

    public EntryDAO getEntryDAO() {
        return entryDAO;
    }

    public void setSeqDAO(SeqDAO seqDAO) {
        this.seqDAO = seqDAO;
    }

    public Object getSeqDAO() {
        return seqDAO;
    }

    @PostConstruct
    public void postConstruct() {
        assert this.fieldConstraints != null;
        assert this.shardDAO != null;
        assert this.entryDAO != null;
        assert this.indexDAO != null;
        assert this.seqDAO != null;
        this.shardField = this.fieldConstraints.getShardField();
        this.queryService = new QueryService();
        this.queryService.setEntryBase(this);
    }

    //------------------

    public void addEventListener(EntryEventListener l) {
        if (l == null) {
            throw new NullPointerException("EntryEventlistener");
        }
        this.listeners.add(l);
    }

    @Override
    public Entry addEntry(long id, Field... fields) {
        ArrayList<Field> array = new ArrayList<Field>();
        for (int i = 0; i < fields.length; i++) {
            array.add(fields[i]);
        }
        return addEntry(id, array);
    }

    @Override
    public Entry addEntry(long id, List<Field> fields) {
        if (id <= 0) {
            throw new IllegalArgumentException("illegal entry id " + id);
        }

        Entry entry = new Entry(id, fields);

        // 删除外部调用着不可更新的field(比如以点号开始的、名称为null的等等字段)
        checkUserFields(entry);

        // 散表值
        Long shardValue;
        if (this.shardField == null) {
            shardValue = id;
        } else {
            shardValue = entry.getFieldValue(shardField, Long.class);
            if (shardValue == null) {
                throw new IllegalArgumentException(//
                        "'" + shardField + "' is required for entry " + id);
            }
        }

        // 更新entry数据
        updateEntryInfo(shardValue, false, entry);

        // 更新shard表
        if (shardField != null) {
            shardDAO.addShardValue(entry.getSequence(), entry.getId(), shardValue);
        }

        // 更新index数据
        updateEntryIndex(null, entry);

        if (entry.getSequence() <= 0) {
            throw new Error("the sequence is required when operation has been done.");
        }

        // fire event
        for (EntryEventListener l : this.listeners) {
            l.onAdded(this, entry);
        }
        return entry;
    }

    @Override
    public Entry updateEntry(long id, boolean replace, Field... fields) {
        ArrayList<Field> array = new ArrayList<Field>();
        for (int i = 0; i < fields.length; i++) {
            array.add(fields[i]);
        }
        return updateEntry(id, replace, array);
    }

    @Override
    public Entry updateEntry(long id, boolean replace, List<Field> fields) {
        Entry newEntry = new Entry(id, fields);

        // 删除外部调用着不可更新的field(比如以点号开始的、名称为null的等等字段)
        checkUserFields(newEntry);

        // 取oldEntry
        Long shardValue;
        Entry oldEntry = null;
        if (shardField == null) {
            shardValue = newEntry.getId();
            oldEntry = entryDAO.get(shardValue, newEntry.getId());
        } else {
            // TODO: shardField值的更新还不支持，目前也没这个需求，暂时不列入实现计划
            shardValue = newEntry.getFieldValue(shardField, Long.class);
            if (shardValue == null) {
                shardValue = shardDAO.getShardValue(newEntry.getId());
            }
            if (shardValue == null) {
                throw new IllegalArgumentException(//
                        "shard field '" + shardField + "' is required for entry " + id);
            }
            oldEntry = entryDAO.get(shardValue, newEntry.getId());
        }

        // 如果存储中没有原记录，返回null抗议
        if (oldEntry == null) {
            loggerOfUpdate.info("failed to update the entry " + newEntry.getId() + ": not exists.");
            return null;
        }

        // 对后续更新或插入操作的一些判断和准备
        if (!replace) {
            // 把将要更新的条目补充完整
            fullfillNewEntry(newEntry, oldEntry);
        }

        // 更新entry数据
        updateEntryInfo(shardValue, oldEntry != null, newEntry);

        // 不更新shard表：因为分散的字段目前仍不支持其更改的
        // do nothing about shardDAO

        // 更新index数据
        updateEntryIndex(oldEntry, newEntry);

        // fire event
        for (EntryEventListener l : this.listeners) {
            l.onUpdate(this, oldEntry, newEntry);
        }
        return newEntry;
    }

    @Override
    public void deleteEntry(long entryId) {
        Long shardValue = getShardValue(entryId);
        if (shardValue == null) {
            loggerOfUpdate.info(//
                    "failed to delete the entry " + entryId + ": no shard value for it");
            return;
        }
        Entry entry = entryDAO.get(shardValue, entryId);
        if (entry == null) {
            loggerOfUpdate.info("failed to delete the entry " + entryId + ": not exists.");
            return;
        }

        // 先删index，后删entry; index_shard不删
        boolean debugEnable = loggerOfUpdate.isDebugEnabled();
        for (Field field : entry.getFields()) {
            if (field == null || field.getName().equals(shardField)/*shardField字段所在的表是entry表，要放到最后*/) {
                continue;
            }
            if (field.getIndex(false) && fieldConstraints.supportsIndex(field.getName())) {
                indexDAO.delete(field.getName(), field.getValue(), entryId);
                if (debugEnable) {
                    loggerOfUpdate.debug("index deleted, " + field.getName() + ": "
                            + field.getValue() + "->" + entryId);

                }
            }
        }

        entryDAO.delete(shardValue, entryId);

        if (loggerOfUpdate.isInfoEnabled()) {
            loggerOfUpdate.info("entry deleted, " + entryId + " by shard value " + shardValue);
        }

        // fire event
        for (EntryEventListener l : this.listeners) {
            l.onDelete(this, entry);
        }
    }

    @Override
    public int getCount(Query query) {
        query = query.clone();

        Range<Long> idRange = extractFieldId(query);
        Range<Long> sequenceRange = extractFieldCreateTime(query);
        //
        if (query.getFields().size() != 1) {
            throw new IllegalArgumentException("Illegal query '" + query + "': too more fields");
        }
        //
        QField field = query.getFields().get(0);
        if (!this.fieldConstraints.supportsIndex(field.getName())) {
            throw new IllegalArgumentException("'" + field.getName() + "' is not a index field.");
        }
        //
        int sum = 0;
        for (QValue value : field.getValues()) {
            Object v = value.convertValue1();
            sum += indexDAO.count(field.getName(), v, idRange, sequenceRange.getStart(),
                    sequenceRange.getEnd());
        }
        return sum;
    }

    @Override
    public List<Entry> find(Query query, int offset, int size) {
        query = query.clone();

        Range<Long> idRange = extractFieldId(query);
        Range<Long> sequenceRange = extractFieldCreateTime(query);

        if (query.getFields().size() != 1) {
            throw new IllegalArgumentException("Illegal query '" + query + "': too more fields");
        }
        //
        QField field = query.getFields().get(0);
        if (!this.fieldConstraints.supportsIndex(field.getName())) {
            throw new IllegalArgumentException("'" + field.getName() + "' is not a index field.");
        }
        //
        if (field.getValues().size() != 0) {
            throw new IllegalArgumentException("no 'or' in '" + field.getName() + "' field");
        }
        Object v = field.getValues().get(0).convertValue1();
        return indexDAO.find(field.getName(), v, idRange, sequenceRange.getStart(),
                sequenceRange.getEnd(), offset, size);
    }

    @Override
    public List<Entry> find(Query query, int limit) {
        query = query.clone();

        Range<Long> idRange = extractFieldId(query);
        Range<Long> sequenceRange = extractFieldCreateTime(query);
        sequenceRange = extractFieldOlderAndNewer(query, sequenceRange);

        long start = System.currentTimeMillis();
        List<Entry> page = queryService.doQuery(query, idRange, sequenceRange, limit);

        long queryDone = System.currentTimeMillis();

        if (page.size() > 0 && (shardField == null || query.getField(shardField) == null)) {
            for (Entry entry : page) {
                if (entry.getFields().size() == 0) {
                    Entry e = get(entry.getId());
                    entry.setFields(e.getFields());
                    entry.setSequence(e.getSequence());
                }
            }
        }

        long end = System.currentTimeMillis();

        if (loggerOfQuery.isInfoEnabled()) {
            loggerOfQuery.info("[" + getCostString(start, end) + "ms]["
                    + getCostString(queryDone, end) + "ms] find " + page.size() + " entries for: "
                    + query);
        }
        return page;
    }

    /**
     * 返回给定ID对应的entry，如果没有则返回null
     * 
     * @param entryId
     * @param loadbody
     * @return
     */
    public Entry get(long entryId) {
        Long shard = getShardValue(entryId);
        return shard == null ? null : entryDAO.get(shard, entryId);
    }

    // TODO: 现在还是逐个调用get，而不是批量走数据库。
    @Override
    public Entry[] gets(long[] entries) {
        if (entries == null || entries.length == 0) {
            return new Entry[0];
        }
        Entry[] array = new Entry[entries.length];
        for (int i = 0; i < array.length; i++) {
            array[i] = get(entries[i]);
        }
        return array;
    }

    @Override
    public Entry[] gets(long[] entries, String[] fields) {
        Entry[] array = gets(entries);
        if (fields != null) {
            for (Entry entry : array) {
                for (Iterator<Field> iter = entry.getFields().iterator(); iter.hasNext();) {
                    Field f = iter.next();
                    if (!f.getName().startsWith(".") && ArrayUtils.indexOf(fields, f.getName()) < 0) {
                        iter.remove();
                    }
                }
            }
        }
        return array;
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
        if (entry.getFields().size() == 0) {
            throw new IllegalArgumentException("empty fields of entry " + entry.getId());
        }
    }

    /**
     * 把将要更新的条目补充完整
     * 
     * @param newEntry
     * @param oldEntry
     */
    private void fullfillNewEntry(Entry newEntry, Entry oldEntry) {
        newEntry.setSequence(oldEntry.getSequence());
        //
        Set<String> newFieldNames = new HashSet<String>(); // 统计要被替换掉的key
        for (Field newField : newEntry.getFields()) {
            newFieldNames.add(newField.getName());
        }

        // 没有在此次更新之列的老字段，要加入到fields一会去重新
        for (Field oldField : oldEntry.getFields()) {
            String oldName = oldField.getName();
            if ("create_time".equals(oldName)) {
                newEntry.setField("create_time", oldField.getValue());
            } else {
                if (!oldName.startsWith(".") && !newFieldNames.contains(oldName)) {
                    // 之前有索引，但现在不再支持索引了，修正！
                    if (oldField.getIndex(false)
                            && !fieldConstraints.supportsIndex(oldField.getName())) {
                        oldField = oldField.clone();
                        oldField.setIndex(Field.INDEX_FALSE);
                    }
                    newEntry.addField(oldField);
                }
            }
        }
    }

    /**
     * 新增或更新本体数据
     * <p>
     * 如果 update 值为 true 表示系统已经存在该id对应的条目，此时应该进行更新，否则进行新增
     */
    private void updateEntryInfo(long shardValue, boolean update, Entry entry) {
        long start = System.currentTimeMillis();

        // 取出创建时间
        Long createTime = entry.getFieldValue("create_time", Long.class);
        if (createTime == null) {
            createTime = start;
            entry.addField(new Field("create_time", createTime));
        }

        // 将所有field拼成一个info串（body除外）
        String info = Entry.toInfoString(entry.getFields());
        if (info.length() >= infoLengthWarnning) {
            if (info.length() >= infoLengthLimit) {
                throw new IllegalArgumentException("the info string is to long: "
                        + info.substring(0, 255) + "...");
            } else {
                loggerOfInfo.warn("info's length has reached " + info.length()
                        + ", please check the length of column `info` (of table `entry`)");
            }
        }

        // 取出body
        String body = entry.getFieldValue("body");

        // 更新
        if (update) {
            entryDAO.updateEntry(shardValue, entry.getId(), info, body);

            if (loggerOfUpdate.isInfoEnabled()) {
                loggerOfUpdate.info("[" + getCostString(start, System.currentTimeMillis())
                        + "ms] updated:" + " id=" + entry.getId() + ", info.len=" + info.length()
                        + " (" + info + ")");
            }
        }
        // 新增
        else {
            // 创建与之对应的一个序列，然后保存
            long sequence = nextEntrySequence(createTime);
            entry.setSequence(sequence);
            entryDAO.addEntry(shardValue, sequence, entry.getId(), new Date(createTime), info, body);

            if (loggerOfUpdate.isInfoEnabled()) {
                loggerOfUpdate.info("[" + getCostString(start, System.currentTimeMillis())
                        + "ms] saved: id=" + entry.getId() + ", info.len=" + info.length() + " ("
                        + info + ")");
            }
        }
    }

    /**
     * 更所各种索引，删除或新增
     */
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
                if (!oldField.getName().equals(shardField)) {
                    indexDAO.delete(oldField.getName(), oldField.getValue(), entryId);
                }
            }

        }

        // 添加新索引
        for (Field newField : fieldsToBeAdded) {
            if (newField.getIndex(false)) {
                if (!newField.getName().equals(shardField)) {
                    indexDAO.add(newField.getName(), newField.getValue(), entryId,
                            newEntry.getSequence());
                }
            }
        }
    }

    private long nextEntrySequence(long createTime) {
        int seqId = seqDAO.nextSequnenceSuffix();
        long seq = createTime * 1000000 + seqId % 1000000;
        if (seq < createTime) {
            throw new Error(//
                    "error entry sequence '" + seq + "' for entry with create_time " + createTime);
        }
        return seq;
    }

    private Long getShardValue(long entryId) {
        if (this.shardField == null) {
            return entryId;
        } else {
            return shardDAO.getShardValue(entryId);
        }
    }

    /**
     * 返回一个时间耗费3位数(不包含时间单位)
     * 
     * @param start
     * @param end
     * @return
     */
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

    private Range<Long> extractFieldCreateTime(Query query) {
        Range<Long> entrySequenceRange = null;
        QField fieldCreateTime = query.removeField("create_time");
        if (fieldCreateTime != null) {
            if (fieldCreateTime.getValues().size() > 1) {
                throw new IllegalArgumentException("no 'or' in create_time field");
            }
            if (fieldCreateTime.getValues().size() == 1) {
                QValue v = fieldCreateTime.getValues().get(0);
                entrySequenceRange = new Range<Long>();
                entrySequenceRange.setStart(((Long) v.convertValue1()).longValue() * 1000000);
                entrySequenceRange.setEnd(((Long) v.convertValue2()).longValue() * 1000000);

            }
        }
        return entrySequenceRange;
    }

    private Range<Long> extractFieldId(Query query) {
        Range<Long> entryIdRange = null;
        QField fieldId = query.removeField("id");
        if (fieldId != null) {
            if (fieldId.getValues().size() > 1) {
                throw new IllegalArgumentException("no 'or' in id field");
            }
            if (fieldId.getValues().size() == 1) {
                QValue v = fieldId.getValues().get(0);
                Long lowerId = ((Long) v.convertValue1());
                Long upperId = ((Long) v.convertValue2());
                entryIdRange = new Range<Long>(lowerId, upperId);
            }
        }
        return entryIdRange;
    }

    private Range<Long> extractFieldOlderAndNewer(Query query, Range<Long> entrySequenceRange) {
        // oldler:(entryId)
        QField fieldOlder = query.removeField("older");
        if (fieldOlder != null) {
            if (fieldOlder.getValues().size() != 0) {
                throw new IllegalArgumentException("illegal older field value size: "
                        + fieldOlder.getValues().size());
            }
            QValue v = fieldOlder.getValues().get(0);
            Long entryId = (Long) v.convertValue1();
            Long seq = shardDAO.getSequence(entryId);
            if (seq != null) {
                if (entrySequenceRange == null) {
                    entrySequenceRange = new Range<Long>();
                }
                if (entrySequenceRange.getEnd() == null || entrySequenceRange.getEnd() > seq) {
                    entrySequenceRange.setEnd(seq);
                }
            }
        }
        // newer:(entryId)
        QField fieldNewer = query.removeField("newer");
        if (fieldNewer != null) {
            if (fieldNewer.getValues().size() != 0) {
                throw new IllegalArgumentException("illegal newer field value size: "
                        + fieldNewer.getValues().size());
            }
            QValue v = fieldNewer.getValues().get(0);
            Long entryId = (Long) v.convertValue1();
            Long seq = shardDAO.getSequence(entryId);
            if (seq != null) {
                if (entrySequenceRange == null) {
                    entrySequenceRange = new Range<Long>();
                }
                if (entrySequenceRange.getStart() == null || entrySequenceRange.getStart() < seq) {
                    entrySequenceRange.setStart(seq);
                }
            }

        }
        return entrySequenceRange;
    }

}

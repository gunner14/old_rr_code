package com.renren.entrybase.mysql;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.SortedSet;
import java.util.TreeSet;

import com.renren.entrybase.model.Entry;
import com.renren.entrybase.model.query.QField;
import com.renren.entrybase.model.query.QValue;
import com.renren.entrybase.model.query.Query;
import com.renren.entrybase.mysql.dao.IndexDAO;
import com.renren.entrybase.mysql.model.EntryPage;
import com.renren.entrybase.mysql.model.FieldComparator;
import com.renren.entrybase.mysql.model.FieldConstraints;
import com.renren.entrybase.mysql.model.Range;

/**
 * 
 * @author qieqie.wang@gmail.com
 * @author yanghe.liang@renren-inc.com
 * 
 */
class QueryService {

    private IndexDAO indexDAO;

    private FieldConstraints fieldConstraints;

    private String shardField;

    private Comparator<QField> fieldComparator;

    private MySQLEntryBase entryBase;

    /**
     * 
     * @param base
     */
    public void setEntryBase(MySQLEntryBase base) {
        this.entryBase = base;
        this.indexDAO = base.getIndexDAO();
        this.fieldConstraints = base.getFieldConstraints();
        this.fieldComparator = new FieldComparator(fieldConstraints.getIndexFields());
        this.shardField = this.fieldConstraints.getShardField();
    }

    //--------------

    /**
     * 
     * 
     * @param query 查询条件，例如"author:1234 and master:(4321 or 7890);"
     * @param offset 开始的条数，从0开始，null代表0
     * @param size 最多返回个数
     * @return
     */
    public List<Entry> doQuery(Query query, Range<Long> entryIdRange, Range<Long> sequenceRange,
            int size) {

        // 如果没有任何条件，那就返回empty
        if (query.getFields().size() == 0 || size <= 0) {
            return new ArrayList<Entry>();
        }

        // sortedFields 存储将query中的所有fields进行排序，即将有索引的字段提前
        List<QField> sortedFields = query.getFields();
        Collections.sort(sortedFields, fieldComparator);

        // 至少要提供一个有索引的字段进行查询
        final QField first = sortedFields.get(0);
        if (!fieldConstraints.supportsIndex(first.getName())) {
            throw new IllegalArgumentException("query must contains an index field: " + query);
        }

        // 执行真正的、可返回超过size个数的查询
        EntryPage page = doRollQuery(query, entryIdRange, sequenceRange, size);

        if (size >= page.size()) {
            return page.getHits();
        } else {
            // 截取一部分hits出来，符合size的限制
            return page.getHits().subList(0, size);
        }
    }

    /**
     * 通过逐个field的一轮轮查询，返回可能超过size个数的查询结果
     * 
     * @param fields
     * @param cursor
     * @param forward
     * @param userLimit
     * @return
     */
    // 对那些非索引字段的查询，应该在shardField查询之后立即进行过滤！
    private EntryPage doRollQuery(Query query, Range<Long> entryIdRange, Range<Long> sequenceRange,
            int size) {

        // the page to return 
        EntryPage page = null;
        ArrayList<QField> fields = new ArrayList<QField>(query.getFields());

        // 非索引字段开始的位置
        int unindexField = fields.size();
        for (int i = 0; i < fields.size(); i++) {
            if (!this.fieldConstraints.supportsIndex(fields.get(i).getName())) {
                unindexField = i;
                break;
            }
        }

        // 一次while对第一个field进行查询的结果数
        int roll = getRoll(size, fields);

        while (true) {
            // 逐个做 field query
            EntryPage roundPage = null; // 代表某一轮（while）的查询结果
            int firstFieldPageSize = 0; // 第一个field的查询结果数
            int fieldLimit = fields.size(); // fieldLimit标识下面for循环的截止位置(不包含该位置)

            for (int i = 0; i < fieldLimit; i++) {
                QField field = fields.get(i);
                if (i == 0) {
                    roundPage = doFieldQuery(field, entryIdRange, sequenceRange, null, roll);
                    firstFieldPageSize = roundPage.getHits().size();
                    // 下一轮(while)的sequenceRange提前计算好
                    if (firstFieldPageSize > 0) {
                        if (sequenceRange == null) {
                            sequenceRange = new Range<Long>();
                            sequenceRange.setEnd(Long.MAX_VALUE);
                        }
                        if (sequenceRange.getEnd() != null) {
                            sequenceRange.setEnd(roundPage.getLastEntry().getSequence());
                        } else {
                            sequenceRange.setStart(roundPage.getFirstEntry().getSequence());
                        }
                    }
                    // 从主散取出来的entry已经有info了，因此提前做byEntryInfo的过滤
                    if (unindexField != fields.size() && field.getName().equals(shardField)) {
                        for (int j = unindexField; j < fields.size(); j++) {
                            filterByEntryInfo(roundPage,
                                    fields.subList(unindexField, fields.size()));
                        }
                        // 变更fieldLimit
                        fieldLimit = unindexField;
                    }
                }
                // 通过索引查找
                else if (i < unindexField) {
                    // 第一个field需要用到entryIdRange，第2field开始entryIdRange传null
                    EntryPage p = doFieldQuery(//
                            field, null, null, roundPage.getEntries(), roll);
                    setHits(roundPage, p.getHits());
                }
                // 一旦发现了一个非索引的field，剩下的都通过entry_info查找
                else {
                    filterByEntryInfo(roundPage, fields.subList(i, fields.size()));
                    break; // break the remaining fields
                }
                if (roundPage.size() == 0) {
                    break;
                }
            }
            if (page == null) {
                page = roundPage;
            } else {
                page.appendHits(roundPage.getHits());
            }
            if (page.size() >= size) {
                break;
            }
            if (firstFieldPageSize < roll) {
                // 不用找了，因为本轮的第一个field查找中已经找不满roll个符合条件的了
                break;
            }
        }
        return page;
    }

    /**
     * 
     * @param field
     * @param entryIdRange
     * @param cursor
     * @param limitOption
     * @param size
     * @return
     */
    private EntryPage doFieldQuery(QField field, Range<Long> entryIdRange,
            Range<Long> sequenceRange, long[] entries, int size) {
        // 
        if (field.getValues().size() == 0) {
            return new EntryPage(new ArrayList<Entry>());
        }

        // author:100
        if (field.getValues().size() == 1) {
            QValue value = field.getValues().get(0);
            List<Entry> hits = doFieldValueQuery(//
                    value, entryIdRange, sequenceRange, entries, size);
            return new EntryPage(hits);
        }

        // author: (100 or 200 or 300)
        else {
            SortedSet<Entry> hits = new TreeSet<Entry>();
            List<QValue> values = field.getValues();
            for (QValue value : values) {
                List<Entry> singleHits = doFieldValueQuery(//
                        value, entryIdRange, sequenceRange, entries, size);
                hits.addAll(singleHits);
            }
            return new EntryPage(new ArrayList<Entry>(hits));
        }
    }

    private void filterByEntryInfo(EntryPage roundPage, List<QField> queryFields) {
        Iterator<Entry> iter = roundPage.getHits().iterator();
        for (int i = 0; iter.hasNext(); i++) {
            Entry entry = iter.next();
            if (entry == null) {
                iter.remove();
                continue;
            } else {
                if (entry.getFields().size() == 0) {
                    Entry _entry = entryBase.get(entry.getId());
                    if (_entry == null) {
                        iter.remove();
                        continue;
                    }
                    entry.setFields(_entry.getFields());
                    entry.setSequence(_entry.getSequence());
                }
                for (QField queryField : queryFields) {//true代表
                    // true代表通过了queryFiled检查
                    if (!matched(entry, queryField)) {
                        iter.remove();
                        break; // break the fields filter
                    }
                }
            }
        }
    }

    private boolean matched(Entry entry, QField queryField) {
        for (QValue v : queryField.getValues()) {
            // 单值比较
            if (v.getValue2() == null) {
                if (entry.hasFieldValue(queryField.getName(), v.getValue1())) {
                    return true;
                }
            }
            // 区间比较
            else {
                List<String> entryFieldValues = entry.getFieldValues(queryField.getName());
                if (v.isInteger()) {
                    long v1 = (Long) v.convertValue1();
                    long v2 = (Long) v.convertValue2();
                    for (String entryFieldValue : entryFieldValues) {
                        try {
                            long entryFieldLongValue = Long.valueOf(entryFieldValue);
                            if (entryFieldLongValue >= v1 && entryFieldLongValue < v2) {
                                return true;
                            }
                        } catch (NumberFormatException e) {}
                    }
                } else if (v.isDouble()) {
                    double v1 = (Double) v.convertValue1();
                    double v2 = (Double) v.convertValue2();
                    for (String entryFieldValue : entryFieldValues) {
                        try {
                            double entryFieldLongValue = Double.valueOf(entryFieldValue);
                            if (entryFieldLongValue >= v1 && entryFieldLongValue < v2) {
                                return true;
                            }
                        } catch (NumberFormatException e) {}
                    }
                } else {
                    throw new IllegalArgumentException("not supported range filter for field " + v);
                }
            }
        }
        //
        return false;
    }

    private List<Entry> doFieldValueQuery(//
            QValue value, //
            Range<Long> entryIdRange, //
            Range<Long> sequenceRange,//
            long[] entries, //
            int size) //
    {
        // TODO: value是两个值的未支持!
        if (value.getValue2() != null) {
            throw new IllegalArgumentException(value + " is not supported");
        }
        Object indexValue = value.convertValue1();

        List<Entry> indexes;
        if (entries == null) {
            indexes = indexDAO.find(value.getField(), indexValue, entryIdRange,
                    sequenceRange.getStart(), sequenceRange.getEnd(), null, size);
        } else {
            indexes = indexDAO.findByIn(value.getField(), indexValue, entries, size);
        }
        return indexes;
    }

    /**
     * 返回从一次fieldQuery的最大数目
     * 
     * @param limit
     * @param sortedFields
     * @return
     */
    private int getRoll(int limit, List<QField> sortedFields) {
        if (sortedFields.size() == 1) {
            return limit;
        }
        int roll = limit * sortedFields.size() * 10;
        if (roll > 1000) {
            roll = 1000;
        }
        return roll;
    }

    /**
     * 将page设置为给定的结果，但是如果原来page中的结果的entry已经字段信息的而且给定的hits对应的entry没有，
     * 则将之设置到hits中
     * 
     * @param page
     * @param hits
     */
    private void setHits(EntryPage page, List<Entry> hits) {
        if (page.getHits().size() == 0) {
            page.setHits(hits);
        } else {
            for (int i = 0; i < hits.size(); i++) {
                Entry ie = hits.get(i);
                if (ie.getFields().size() > 0) {
                    continue;
                }
                List<Entry> jhits = page.getHits();
                for (int j = 0; j < jhits.size(); j++) {
                    Entry je = jhits.get(j);
                    if (je.equals(ie)) {
                        if (je.getFields().size() > 0) {
                            ie.setFields(je.getFields());
                            ie.setSequence(je.getSequence());
                        }
                        break;
                    }
                }
            }
            page.setHits(hits);
        }
    }

}

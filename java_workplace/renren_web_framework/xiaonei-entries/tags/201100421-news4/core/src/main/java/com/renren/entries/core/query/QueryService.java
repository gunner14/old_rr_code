package com.renren.entries.core.query;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.entries.core.FieldConstraints;
import com.renren.entries.core.dao.Between;
import com.renren.entries.core.dao.CachedInfoDAO;
import com.renren.entries.core.dao.IndexDAO;
import com.renren.entries.model.Entry;
import com.renren.entries.model.EntryPage;
import com.renren.entries.model.Hit;
import com.xiaonei.commons.gid.util.G;
import com.xiaonei.commons.gid.util.SubType;
import com.xiaonei.commons.gid.util.Type;

/**
 * 
 * @author qieqie.wang@gmail.com
 * @author yanghe.liang@renren-inc.com
 * 
 */
public class QueryService {

    private IndexDAO indexDAO;

    private CachedInfoDAO infoDAO;

    private FieldConstraints fieldConstraints;

    private Comparator<Field> fieldComparator;

    /**
     * 设置 {@link IndexDAO} 接口
     * 
     * @param indexDAO
     */
    @Autowired
    public void setIndexDAO(IndexDAO indexDAO) {
        this.indexDAO = indexDAO;
    }

    @Autowired
    public void setInfoDAO(CachedInfoDAO infoDAO) {
        this.infoDAO = infoDAO;
    }

    @Autowired
    public void setFieldConstraints(FieldConstraints fieldConstraints) {
        this.fieldConstraints = fieldConstraints;
        this.fieldComparator = new FieldComparator(fieldConstraints.getIndexFields());
    }

    /**
     * 从upperCursor开始，查找并返回符合query要求的实体
     * 
     * @param query 查询条件，例如"author:1234 and master:(4321 or 7890);"
     * @param forwardOrBackward 向前或向后查找
     * @param cursor
     *        向前查找时，所返回的实体的游标属性必须小于该值(游标属性是系统内部规定的一个属性)；查询第一页时，请设置Long.
     *        MAX_VALUE作为其值
     * @param limit 最多返回个数
     * @return
     */
    public EntryPage doQuery(Query query, boolean forward, long cursor, int limit) {

        // 如果没有任何条件，那就返回empty
        if (query.getFields().size() == 0 || limit == 0) {
            List<Hit> hits = Collections.emptyList();
            return new EntryPage(cursor, forward, hits);
        }

        // sortedFields 存储将query中的所有fields进行排序后的结果，即将有索引的字段提前
        List<Field> sortedFields = query.getFields();
        Collections.sort(sortedFields, fieldComparator);

        // 至少要提供一个有索引的字段进行查询
        final Field first = sortedFields.get(0);
        if (!fieldConstraints.supportsIndex(first.getName())) {
            throw new IllegalArgumentException("query must contains an index field: " + query);
        }

        // 执行真正的、可返回超过limit个数的查询
        EntryPage page = doRollQuery(query, forward, cursor, limit);

        // 截取一部分hits出来，符合limit的限制
        return page.truncates(limit);
    }

    /**
     * 通过逐个field的一轮轮查询，返回可能超过limit个数的查询结果
     * 
     * @param fields
     * @param cursor
     * @param forward
     * @param userLimit
     * @return
     */
    private EntryPage doRollQuery(//
            Query query, boolean forward, long cursor, int userLimit) {
        List<Field> fields = query.getFields();
        int roll = getRoll(userLimit, fields);
        EntryPage page = null; // 最终要返回的page
        //
        Between entryIdRange = null;
        if (query.getTypeField() != null) {
            Type type;
            String typeString = query.getTypeField().getValues().get(0).getValue1();
            if (Character.isLetter(typeString.charAt(0))) {
                type = Type.toType(typeString);
            } else {
                type = Type.toType(Long.parseLong(typeString));
            }
            entryIdRange = new Between(G.id(type, 1), G.id(type, G.upperXValueOf(type)) + 1);
        }
        //
        while (true) {
            long roundCursor; // 
            // 更换本轮的cursor
            if (page == null) {
                roundCursor = cursor;
            } else {
                roundCursor = forward ? page.getLowerCursor() : page.getUpperCursor();
            }
            // 逐个做field query
            EntryPage roundPage = null; // 代表新一轮的查询结果
            int firstFieldPageSize = Integer.MAX_VALUE; // 第一个field的查询结果数
            for (int i = 0; i < fields.size(); i++) {
                Field field = fields.get(i);
                if (i == 0) {
                    EntryPage cursorScope = new EntryPage(roundCursor, forward, null);
                    roundPage = doFieldQuery(field, entryIdRange, cursorScope, roll);
                    firstFieldPageSize = roundPage.getHits().size();
                }
                // 通过索引查找
                else if (fieldConstraints.supportsIndex(field.getName())) {
                    EntryPage p = doFieldQuery(field, null, roundPage, roll);// 第一个field需要用到entryRange，用完即弃，第2个参数传null
                    roundPage.setHits(p.getHits());
                }
                // 一旦发现了一个非索引的field，剩下的都通过entry_info查找
                else {
                    filterByEntryInfo(roundPage, fields.subList(i, fields.size()));
                    break; // break the remaining fields
                }
                if (roundPage.getHits().size() == 0) {
                    break;
                }
            }
            if (page == null) {
                page = roundPage;
            } else {
                page.appendHits(roundPage.getHits());
            }
            if (page.getHits().size() >= userLimit) {
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
     * @param limit
     * @return
     */
    private EntryPage doFieldQuery(Field field, Between entryIdRange, EntryPage cursorScope,
            int limit) {
        if (field.getValues().size() == 0) {
            List<Hit> hits = Collections.emptyList();
            return new EntryPage(cursorScope.getCursor(), cursorScope.isForward(), hits);
        }
        // author:100
        if (field.getValues().size() == 1) {
            List<Hit> indexes = doFieldValueQuery(field.getValues().get(0), entryIdRange,
                    cursorScope, limit);
            return new EntryPage(cursorScope.getCursor(), cursorScope.isForward(), indexes);
        }
        // author: (100 or 200 or 300)
        else {
            // TODO: 这边的排序或许有优化空间，毕竟每一次doFieldValueQuery出来的indexes已经是排好序的
            Map<Long/*entry*/, Hit/*lowerIndex*/> lowerCursors = new HashMap<Long, Hit>();

            List<Value> values = field.getValues();
            for (Value value : values) {
                List<Hit> indexes = doFieldValueQuery(value, entryIdRange, cursorScope, limit);
                for (Hit index : indexes) {
                    long entry = index.getEntryId();
                    Hit lower = lowerCursors.get(entry);
                    if (lower == null || index.getCursor() < lower.getCursor()) {
                        lowerCursors.put(entry, index);
                    }
                }
            }
            List<Hit> indexes = new ArrayList<Hit>(lowerCursors.values());
            //  按indexId排序，即按时间排序
            Collections.sort(indexes);
            return new EntryPage(cursorScope.getCursor(), cursorScope.isForward(), indexes);
        }
    }

    private void filterByEntryInfo(EntryPage roundPage, List<Field> queryFields) {
        Entry[] entries = infoDAO.gets(roundPage.getEntries());
        Iterator<Hit> iter = roundPage.getHits().iterator();
        for (int i = 0; iter.hasNext(); i++) {
            iter.next();
            Entry entry = entries[i];
            if (entry == null) {
                iter.remove();
                continue;
            } else {
                for (Field queryField : queryFields) {//true代表
                    // true代表通过了queryFiled检查
                    if (!matched(entry, queryField)) {
                        iter.remove();
                        break; // break the fields filter
                    }
                }
            }
        }
    }

    private boolean matched(Entry entry, Field queryField) {
        for (Value v : queryField.getValues()) {
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

    private List<Hit> doFieldValueQuery(Value value, Between entryRange, EntryPage scope, int limit) {
        // TODO: value是两个值的还未搞！
        if (value.getValue2() != null) {
            throw new IllegalArgumentException(value + " is not supported");
        }
        Object indexValue = value.convertValue1();
        if (value.getField().equals("subtype")) {
            if (indexValue instanceof String) {
                if (StringUtils.isNumeric((String) indexValue)) {
                    indexValue = Long.parseLong((String) indexValue);
                } else {
                    indexValue = SubType.toType((String) indexValue).getValue();
                }
            }
        }
        long upper = scope.getUpperCursor();
        long lower = scope.getLowerCursor();
        long[] entries = scope.getEntries();
        List<Hit> indexes;
        if (entries == null) {
            if (lower <= 0) {
                // lower无，upper有
                if (upper > 0 && upper <= Long.MAX_VALUE) {
                    indexes = indexDAO.nextPage(//
                            value.getField(), indexValue, entryRange, upper, limit);
                }
                // lower无，upper无
                else {
                    throw new IllegalArgumentException("upperCursor or lowerCursor is needed.");
                }
            } else {
                // lower有，upper不是MAX_VALUE
                if (upper > 0 && upper < Long.MAX_VALUE /*upper没有等号*/) {
                    indexes = indexDAO.rangePage(//
                            value.getField(), indexValue, entryRange, upper, lower, limit);
                }
                // lower有，upper为MAX_VALUE
                else {
                    indexes = indexDAO.prePage(//
                            value.getField(), indexValue, entryRange, lower, limit);
                }
            }
        } else {
            if (entryRange != null) {
                throw new IllegalArgumentException("entry range here is never supported");
            }
            if (lower <= 0) {
                // lower无，upper有
                if (upper > 0 && upper <= Long.MAX_VALUE) {
                    indexes = indexDAO.nextPage(//
                            value.getField(), indexValue, upper, entries, limit);
                }
                // lower无，upper无
                else {
                    throw new IllegalArgumentException("upperCursor or lowerCursor is needed.");
                }
            } else {
                // lower有，upper有
                if (upper > 0 && upper < Long.MAX_VALUE/*upper没有等号*/) {
                    indexes = indexDAO.rangePage(//
                            value.getField(), indexValue, upper, lower, entries, limit);
                }
                // lower有，upper无
                else {
                    indexes = indexDAO.prePage(//
                            value.getField(), indexValue, lower, entries, limit);
                }
            }
        }
        return indexes;
    }

    public static void main(String[] args) {
        System.out.println(G.extractType(3602888497989419696L).value);//blog
        System.out.println(G.extractType(3530830903951491759L).value);//status
        System.out.println(G.extractType(3891118874141131445L).value);//SHARE
        System.out.println(Type.toType(832));
    }

    /**
     * 返回从一次fieldQuery的最大数目
     * 
     * @param limit
     * @param sortedFields
     * @return
     */
    private int getRoll(int limit, List<Field> sortedFields) {
        if (sortedFields.size() == 1) {
            return limit;
        }
        int roll = limit * sortedFields.size() * 10;
        if (roll > 1000) {
            roll = 1000;
        }
        return roll;
    }

}

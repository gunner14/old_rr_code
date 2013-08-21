package com.renren.entrybase.model.query;

import java.io.StringReader;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.RandomAccess;

import com.renren.entrybase.model.query.parser.QueryParser;

/**
 * {@link Query}包含了若干字段及其值，作为条件用于从本系统中查询出相应的数据
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class Query implements Cloneable {

    /**
     * 创建一个 {@link Query} 对象
     * 
     * @param queryString
     * @return
     */
    public static Query toQuery(String queryString) {
        return new QueryParser(new StringReader(queryString)).parse();
    }

    //

    private List<QField> fields = new LinkedList<QField>();

    public Query() {
    }

    public Query(String queryString) {
        Query q = new QueryParser(new StringReader(queryString)).parse();
        this.fields = q.fields;
    }

    public void addField(QField field) {
        fields.add(field);
    }

    /**
     * fields
     * 
     * @return shold be a {@link RandomAccess} list
     */
    public List<QField> getFields() {
        return fields;
    }

    public Query add(String fieldName, Collection<?> values) {
        Object[] array = values.toArray();
        return add(fieldName, array);
    }

    //TODO: 如果传入的是“1－2”，1和2表示的数值而非字符串怎么处理？按照line46的做法就会悲剧，就被当成String了！
    public Query add(String fieldName, Object... values) {
        for (Object value : values) {
            QField field = new QField(fieldName);
            if (value instanceof String) {
                int index = ((String) value).indexOf('-');
                if (index >= 0) {
                    String value1 = value.toString().substring(0, index);
                    String value2 = value.toString().substring(index + 1);
                    field.addValue(new QValue(fieldName, value1, value2));
                } else {
                    field.addValue(new QValue(fieldName, value));
                }
            } else {
                field.addValue(new QValue(fieldName, value));
            }
            this.fields.add(field);
        }
        return this;
    }

    /**
     * 返回给定名称的第一个字段，如果无则返回null
     * 
     * @param name
     * @return
     */
    public QField getField(String name) {
        if (name == null) {
            throw new IllegalArgumentException("name");
        }
        for (QField f : getFields()) {
            if (f == null) {
                continue;
            }
            if (name.equals(f.getName())) {
                return f;
            }
        }
        return null;
    }

    /**
     * 删除兵返回给定名称的第一个字段，如果无则返回null
     * 
     * @param name
     * @return
     */
    public QField removeField(String name) {
        if (name == null) {
            throw new IllegalArgumentException("name");
        }
        for (Iterator<QField> iterator = fields.iterator(); iterator.hasNext();) {
            QField f = iterator.next();
            if (name.equals(f.getName())) {
                iterator.remove();
                return f;
            }
        }
        return null;
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (QField field : fields) {
            appendField(sb, field);
        }
        sb.append(";");
        return sb.toString();
    }

    private void appendField(StringBuilder sb, QField andNode) {
        if (sb.length() > 0) {
            sb.append(" and ");
        }
        sb.append(andNode.getName());
        sb.append(":(");
        int orcount = 0;
        for (QValue v : andNode.getValues()) {
            if (orcount++ > 0) {
                sb.append(" or ");
            }
            if (v.isString()) {
                sb.append("\"").append(v.getValue1()).append("\"");// v.getValue1可能含有引号，不做特别处理!
            } else {
                sb.append(v.getValue1());
                if (v.getValue2() != null) {
                    sb.append("-");
                    sb.append(v.getValue2());
                }
            }
        }
        sb.append(")");
    }

    @Override
    public Query clone() {
        Query query = new Query();
        List<QField> fields = new LinkedList<QField>();
        fields.addAll(this.fields);
        query.fields = fields;
        return query;
    }

}

package com.renren.entries.mysql.dao.supports;

import java.sql.ResultSet;
import java.sql.SQLException;

import org.springframework.jdbc.core.RowMapper;

import com.renren.entries.model.Entry;
import com.renren.entries.model.Field;

/**
 * {@link EntryRowMapper} 实现了Spring的 {@link RowMapper}
 * 接口，负责将entry表的一行数据映射为一个 {@link Entry}对象。
 * <p>
 * 
 * 查询SQL必须这样写（即要保证列的顺序）: SELECT entry_seq, entry_id, shard_value,
 * create_time, info, body FROM ...
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class EntryRowMapper implements RowMapper {

    @Override
    public Entry mapRow(ResultSet rs, int n) throws SQLException {
        //entry_seq, entry_id, shard_value, create_time, info, body
        Entry entry = new Entry();
        entry.setSequence(rs.getLong(1));
        entry.setId(rs.getLong(2));
        entry.setFields(Entry.toFields(rs.getString(5)));
        String body = rs.getString(6);
        if (body != null) {
            entry.addField(new Field("body", body));
        }
        return entry;
    }

}

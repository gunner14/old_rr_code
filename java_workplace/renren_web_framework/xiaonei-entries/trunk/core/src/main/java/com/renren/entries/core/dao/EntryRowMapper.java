package com.renren.entries.core.dao;

import java.sql.ResultSet;
import java.sql.SQLException;

import org.springframework.jdbc.core.RowMapper;

import com.renren.entries.model.Entry;

/**
 * {@link EntryRowMapper} 实现了Spring的 {@link RowMapper}
 * 接口，负责将entry_info表的一行数据映射为一个 {@link Entry}对象。
 * <p>
 * 
 * 查询SQL必须这样写（即要保证列的顺序）: SELECT entry_id, value FROM entry_info WHERE ...
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class EntryRowMapper implements RowMapper {

    @Override
    public Entry mapRow(ResultSet rs, int n) throws SQLException {
        Entry entry = new Entry();
        entry.setId(rs.getLong(1));
        entry.setUpdated(rs.getTimestamp(2).getTime());
        entry.setFields(Entry.toFields(rs.getString(3)));
        return entry;
    }

}

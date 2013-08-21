package com.renren.entrybase.mysql.dao.supports;

import java.sql.ResultSet;
import java.sql.SQLException;

import org.springframework.jdbc.core.RowMapper;

import com.renren.entrybase.model.Entry;

public class HitRowMapper implements RowMapper {

    private boolean checked;

    private RowMapper entryRowMapper;

    @Override
    public Entry mapRow(ResultSet rs, int i) throws SQLException {
        if (!checked) {
            String tableName = rs.getMetaData().getTableName(1);
            if (tableName.startsWith("entry")) {
                entryRowMapper = new EntryRowMapper();
            }
            checked = true;
        }
        if (entryRowMapper != null) {
            return (Entry) entryRowMapper.mapRow(rs, i);
        } else {
            // entry_id, entry_seq"
            Entry entry = new Entry();
            entry.setId(rs.getLong(1));
            entry.setSequence(rs.getLong(2));
            return entry;
        }
    }
}

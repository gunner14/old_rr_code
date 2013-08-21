package com.renren.entries.core.dao;

import java.sql.ResultSet;
import java.sql.SQLException;

import org.springframework.jdbc.core.RowMapper;

import com.renren.entries.model.Hit;

public class HitRowMapper implements RowMapper {

    @Override
    public Hit mapRow(ResultSet rs, int n) throws SQLException {
        Hit item = new Hit();
        item.setCursor(rs.getLong(1));
        item.setEntryId(rs.getLong(2));
        return item;
    }

}

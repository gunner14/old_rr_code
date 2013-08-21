package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.reg.register.model.Upload;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class UploadDAO {
    private static UploadDAO instance = new UploadDAO();

    public static UploadDAO getInstance() {
        return instance;
    }

    private UploadDAO() {
    }

    public Upload get(final String id) throws SQLException {
        final String sql = "select * from upload where id = ?";
        OpUniq op = new OpUniq(sql, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, id);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUpload(rs);
            }
        };
        return (Upload) DataAccessMgr.getInstance().queryUnique(op);
    }

    private Upload parseUpload(ResultSet rs) throws SQLException {
        Upload u = new Upload();
        u.setId(rs.getInt("id"));
        u.setSize1(rs.getString("size1"));
        u.setSize2(rs.getString("size2"));
        u.setSize3(rs.getString("size3"));
        u.setSize4(rs.getString("size4"));
        return u;
    }
}
package com.xiaonei.reg.guide.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.guide.model.ElementarySchool;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

final public class ElementarySchoolDAO extends XnDAO {

    private static ElementarySchoolDAO instance = new ElementarySchoolDAO();

    public static ElementarySchoolDAO getInstance() {
        return instance;
    }

    private ElementarySchoolDAO() {
    }

    private final static String SQL_GET_BY_NAME = "select id, name from elementary_school where name = ?";

    public ElementarySchool getByName(final String name) throws SQLException {
        OpUniq op = new OpUniq(SQL_GET_BY_NAME, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, name);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                ElementarySchool elementarySchool = new ElementarySchool();
                elementarySchool.setId(rs.getInt("id"));
                elementarySchool.setName(rs.getString("name"));
                return elementarySchool;
            }
        };
        return (ElementarySchool) DataAccessMgr.getInstance().queryUnique(op);
    }

}

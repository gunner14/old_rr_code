package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.model.College;
import com.xiaonei.platform.core.model.JuniorSchool;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class JuniorSchoolDAO extends XnDAO {

    private static JuniorSchoolDAO instance = new JuniorSchoolDAO();

    public static JuniorSchoolDAO getInstance() {
        return instance;
    }

    private JuniorSchoolDAO() {
    }

    private final static String sqlgetByName = "select id, name from junior_school where name = ?";

    public JuniorSchool getByName(final String name) throws SQLException {
        OpUniq op = new OpUniq(sqlgetByName, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, name);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                JuniorSchool juniorSchool = new JuniorSchool();
                juniorSchool.setId(rs.getInt("id"));
                juniorSchool.setName(rs.getString("name"));
                return juniorSchool;
            }
        };
        return (JuniorSchool) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String qlGetJuniorSchoolListByCounty = "select id,name,code_pro,code_city from junior_school where code_county = ?";

    public List<College> getJuniorSchoolListByCounty(final String county) throws SQLException {
        OpList op = new OpList(qlGetJuniorSchoolListByCounty, DataAccessMgr.BIZ_COMMON) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, county);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                JuniorSchool juniorSchool = new JuniorSchool();
                juniorSchool.setId(rs.getInt("id"));
                juniorSchool.setName(rs.getString("name"));
                juniorSchool.setCode_pro(rs.getString("code_pro"));
                juniorSchool.setCode_city(rs.getString("code_city"));
                return juniorSchool;
            }
        };
        return extracted(op);
    }

    @SuppressWarnings("unchecked")
    private ArrayList<College> extracted(OpList op) throws SQLException {
        return (ArrayList<College>) DataAccessMgr.getInstance().queryList(op);
    }
}

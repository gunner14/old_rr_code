package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.model.College;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

final public class CollegeDAO extends XnDAO {

    private static CollegeDAO instance = new CollegeDAO();

    public static CollegeDAO getInstance() {
        return instance;
    }

    private CollegeDAO() {
    }

    private final static String SQL_GET_BY_NAME = "select id, name from college where name = ?";

    public College getByName(final String name) throws SQLException {
        OpUniq op = new OpUniq(SQL_GET_BY_NAME, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, name);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                College college = new College();
                college.setId(rs.getInt("id"));
                college.setName(rs.getString("name"));
                return college;
            }
        };
        return (College) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String SQL_GET_BY_ID = "select id, code, name from college where id = ?";

    /**
     * 根据ID查询中专技校信息
     * <ul>
     * <li></li>
     * </ul>
     *
     * @param id
     * @return
     * @throws SQLException
     */
    public College getByID(final int id) throws SQLException {
        OpUniq op = new OpUniq(SQL_GET_BY_ID, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                College college = new College();
                college.setId(rs.getInt("id"));
                college.setName(rs.getString("name"));
                college.setCode(rs.getString("code"));
                return college;
            }
        };
        return (College) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String SQL_GET_CLOOLGET_SCHOOLE_LIST_BY_COUNTY = "select id,name,code_pro,code_city,tribe from college where code_county = ?";

    public List<College> getCollegeSchoolListByCounty(final String county)
            throws SQLException {
        OpList op = new OpList(SQL_GET_CLOOLGET_SCHOOLE_LIST_BY_COUNTY,
                DataAccessMgr.BIZ_COMMON) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, county);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                College college = new College();
                college.setId(rs.getInt("id"));
                college.setName(rs.getString("name"));
                college.setCode_pro(rs.getString("code_pro"));
                college.setCode_city(rs.getString("code_city"));
                college.setTribeId(rs.getInt("tribe"));
                return college;
            }
        };

        return extracted(op);
    }

    @SuppressWarnings("unchecked")
    private ArrayList<College> extracted(OpList op) throws SQLException {
        return (ArrayList<College>) DataAccessMgr.getInstance().queryList(op);
	}

}

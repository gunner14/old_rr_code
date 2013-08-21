/**
 *
 */
package com.xiaonei.reg.guide.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * HighSchoolDAO.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-5 下午09:10:59
 */
public class HighSchoolDAO extends XnDAO {

    static class SingletonHolder {
        static HighSchoolDAO instance = new HighSchoolDAO();
    }

    public static HighSchoolDAO getInstance() {
        return SingletonHolder.instance;
    }

    private HighSchoolDAO() {

    }

    private final static String sql = "select code from high_school where id = ?";

    public String getHighSchoolCode(final int id) throws SQLException {
        OpUniq op = new OpUniq(sql, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            public String parse(ResultSet rs) throws SQLException {
                return rs.getString("code");
            }
        };

        return (String) DataAccessMgr.getInstance().queryUnique(op);
    }

}

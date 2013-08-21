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
 * ChinaAreaDAO.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-5 下午06:14:48
 */
public class ChinaAreaDAO extends XnDAO {

    static class SingletonHolder {
        static ChinaAreaDAO instance = new ChinaAreaDAO();
    }

    public static ChinaAreaDAO getInstance() {
        return SingletonHolder.instance;
    }

    private ChinaAreaDAO() {

    }

    private static final String sqlGetProvince = "select name from china_area where code=concat(substring(?,1,2),'0000000000')";

    /**
     * 得到对应省
     *
     * @param code china_area code
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-5 下午06:57:49
     */
    public String getProvinceName(final String code) throws SQLException {
        OpUniq op = new OpUniq(sqlGetProvince, DataAccessMgr.BIZ_COMMON) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, code);
            }

            @Override
            public String parse(ResultSet rs) throws SQLException {
                return rs.getString("name");
            }
        };
        return (String) DataAccessMgr.getInstance().queryUnique(op);
    }

    private static final String sqlGetCity = "select name from china_area where code=concat(substring(?,1,4),'00000000')";

    /**
     * 得到对应市
     *
     * @param code china_area code
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-5 下午07:02:32
     */
    public String getCityName(final String code) throws SQLException {
        OpUniq op = new OpUniq(sqlGetCity, DataAccessMgr.BIZ_COMMON) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, code);
            }

            @Override
            public String parse(ResultSet rs) throws SQLException {
                return rs.getString("name");
            }
        };
        return (String) DataAccessMgr.getInstance().queryUnique(op);
    }

    private static final String sqlGetDistrict = "select name from china_area where code=concat(substring(?,1,6),'000000')";

    /**
     * 得到对应区
     *
     * @param code
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-6 下午12:31:53
     */
    public String getDistrictName(final String code) throws SQLException {
        OpUniq op = new OpUniq(sqlGetDistrict, DataAccessMgr.BIZ_COMMON) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, code);
            }

            @Override
            public String parse(ResultSet rs) throws SQLException {
                return rs.getString("name");
            }
        };
        return (String) DataAccessMgr.getInstance().queryUnique(op);
	}
	
	

}

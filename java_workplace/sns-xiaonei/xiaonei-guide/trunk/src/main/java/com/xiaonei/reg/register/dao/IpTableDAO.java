/**
 *
 */
package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * IpTableDAO.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-9 下午08:31:44
 */
public class IpTableDAO extends XnDAO {
    
    private static IpTableDAO instance = new IpTableDAO();

    public static IpTableDAO getInstance() {
        return instance;
    }
    
    /**
     * 根据ip判断该ip隶属的大学
     */
    private static final String UniversityDAO_view = "select univ.id, univ.name, univ.count_m, univ.count_f, univ.url, univ.count_star, univ.count_a, univ.tribe, univ.ann_count, univ.logo_url, univ.country_id, univ.province_id ";
    private static final String sqlGetUnivByIp = UniversityDAO_view
            + " from  university univ , (select univ from ip_table where ? between start and end ) as itb where  itb.univ = univ.id ";

    public List<University> getUnivByIp(final String ip) throws SQLException {
        OpList op = new OpList(sqlGetUnivByIp, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setLong(1, getLong(ip));
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }

        };
        return extracted(op);
    }

    @SuppressWarnings("unchecked")
    private ArrayList<University> extracted(OpList op) throws SQLException {
        return (ArrayList<University>) DataAccessMgr.getInstance().queryList(op);
    }

    private University parseUniv(ResultSet rs) throws SQLException {
        University univ = new University();
        univ.setId(rs.getInt("id"));
        univ.setName(rs.getString("name"));
        univ.setNanCount(rs.getInt("count_m"));
        univ.setNvCount(rs.getInt("count_f"));
        univ.setUrl(rs.getString("url"));
        univ.setStarCount(rs.getInt("count_star"));
        univ.setStuCount(rs.getInt("count_a"));
        univ.setTribeId(rs.getInt("tribe"));
        univ.setAnnCount(rs.getInt("ann_count"));
        univ.setLogoUrl(rs.getString("logo_url"));
        univ.setCountryId(rs.getInt("country_id"));
        univ.setProvinceId(rs.getInt("province_id"));
        return univ;
    }

    private long getLong(String ip) {
        String[] sub = ip.trim().split("\\.");
        return (long) Integer.parseInt(sub[0]) * 16777216 + (long) Integer.parseInt(sub[1]) * 65536 + Integer.parseInt(sub[2]) * 256
                + Integer.parseInt(sub[3]);
    }

    /**
     * 国外大学，根据countryid 取ip
     */
    private static final String sqlGetIpByCountry = "SELECT COUNT(*) FROM ip_table_country WHERE country_id = ? AND ? BETWEEN start AND end";

    public int getCountryByIp(final int country, final String ip) throws SQLException {
        OpUniq op = new OpUniq(sqlGetIpByCountry, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, country);
                ps.setLong(2, getLong(ip));
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }
}

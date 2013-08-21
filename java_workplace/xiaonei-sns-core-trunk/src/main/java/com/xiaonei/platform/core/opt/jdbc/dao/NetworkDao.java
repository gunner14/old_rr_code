package com.xiaonei.platform.core.opt.jdbc.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.HashMap;
import java.util.Map;

import com.xiaonei.platform.core.model.College;
import com.xiaonei.platform.core.model.HighSchool;
import com.xiaonei.platform.core.model.JuniorSchool;
import com.xiaonei.platform.core.model.Region;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.network.NetworkManager;

public class NetworkDao {

    public static NetworkDao instance = new NetworkDao();

    public static NetworkDao getInstance() {
        return instance;
    }

    private static final String sqlUnivAll = " select * from university";

    public Map<Integer, University> getAllUniversity() {
        Connection con = null;
        PreparedStatement st = null;
        ResultSet rs = null;
        Map<Integer, University> mp = new HashMap<Integer, University>();
        try {
            con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_COMMON, true);

            st = con.prepareStatement(sqlUnivAll);
            rs = st.executeQuery();

            while (rs.next()) {
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
                univ.setType(NetworkManager.TYPE_UNIV);
                mp.put(univ.getId(), univ);

            }
            return mp;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            DataAccessMgr.getInstance().closeResultSet(rs);
            DataAccessMgr.getInstance().closeStatement(st);
            DataAccessMgr.getInstance().closeConnection(con);
        }
        return null;
    }

    private static final String sqlHighAll = " select id,name,code_pro,code_city,tribe from high_school ;";

    public Map<Integer, HighSchool> getAllHighSchool() {
        Connection con = null;
        PreparedStatement st = null;
        ResultSet rs = null;
        Map<Integer, HighSchool> mp = new HashMap<Integer, HighSchool>();
        try {
            con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_COMMON, true);
            st = con.prepareStatement(sqlHighAll);
            rs = st.executeQuery();
            while (rs.next()) {
                HighSchool highSchool = new HighSchool();
                highSchool.setId(rs.getInt("id"));
                highSchool.setName(rs.getString("name"));
                highSchool.setCode_pro(rs.getString("code_pro"));
                highSchool.setCode_city(rs.getString("code_city"));
                highSchool.setType(NetworkManager.TYPE_HIGH);
                String tribe = rs.getString("tribe");
                if (tribe != null) highSchool.setTribeId(Integer.parseInt(tribe));
                mp.put(highSchool.getId(), highSchool);
            }
            return mp;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            DataAccessMgr.getInstance().closeResultSet(rs);
            DataAccessMgr.getInstance().closeStatement(st);
            DataAccessMgr.getInstance().closeConnection(con);
        }
        return null;
    }

    private static final String sqlWorkplaceAll = " select * from workplace";

    public Map<Integer, Workplace> getAllWorkplace() {
        Connection con = null;
        PreparedStatement st = null;
        ResultSet rs = null;
        Map<Integer, Workplace> mp = new HashMap<Integer, Workplace>();
        try {
            con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_COMMON, true);
            st = con.prepareStatement(sqlWorkplaceAll);
            rs = st.executeQuery();
            while (rs.next()) {
                Workplace w = new Workplace();
                w.setId(rs.getInt("id"));
                w.setName(rs.getString("name"));
                w.setEmail(rs.getString("email"));
                w.setCountAll(rs.getInt("count_a"));
                w.setBuilding(rs.getString("building"));
                w.setType(NetworkManager.TYPE_WORK);
                String tribe = rs.getString("tribe");
                if (tribe != null) w.setTribeId(Integer.parseInt(tribe));
                mp.put(w.getId(), w);
            }
            return mp;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            DataAccessMgr.getInstance().closeResultSet(rs);
            DataAccessMgr.getInstance().closeStatement(st);
            DataAccessMgr.getInstance().closeConnection(con);
        }
        return null;
    }

    private static final String sqlZoneAll = " select * from region";

    public Map<Integer, Region> getAllRegion() {
        Connection con = null;
        PreparedStatement st = null;
        ResultSet res = null;
        Map<Integer, Region> mp = new HashMap<Integer, Region>();
        try {
            con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_COMMON, true);
            st = con.prepareStatement(sqlZoneAll);
            res = st.executeQuery();
            while (res.next()) {
                Region work = new Region();
                work.setId(res.getInt("id"));
                work.setName(res.getString("name"));
                work.setProvinceId(res.getInt("province_id"));
                work.setCountA(res.getInt("count_a"));
                work.setType(NetworkManager.TYPE_RIGION);
                //work.setTribeId(res.getInt("univ.tribe")) ;
                mp.put(work.getId(), work);
            }
            return mp;
        } catch (Exception e) {
            e.printStackTrace();

        } finally {
            DataAccessMgr.getInstance().closeResultSet(res);
            DataAccessMgr.getInstance().closeStatement(st);
            DataAccessMgr.getInstance().closeConnection(con);
        }
        return null;
    }

    private static final String sqlJuniorAll = " select * from junior_school;";

    public Map<Integer, JuniorSchool> getAllJuniorSchool() {
        Connection con = null;
        PreparedStatement st = null;
        ResultSet rs = null;
        Map<Integer, JuniorSchool> mp = new HashMap<Integer, JuniorSchool>();
        try {
            con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_COMMON, true);
            st = con.prepareStatement(sqlJuniorAll);
            rs = st.executeQuery();
            while (rs.next()) {
                JuniorSchool juniorSchool = new JuniorSchool();
                juniorSchool.setId(rs.getInt("id"));
                juniorSchool.setName(rs.getString("name"));
                mp.put(juniorSchool.getId(), juniorSchool);
            }
            return mp;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            DataAccessMgr.getInstance().closeResultSet(rs);
            DataAccessMgr.getInstance().closeStatement(st);
            DataAccessMgr.getInstance().closeConnection(con);
        }
        return null;
    }

    private static final String sqlCollegeAll = " select * from college;";

    public Map<Integer, College> getAllCollege() {
        Connection con = null;
        PreparedStatement st = null;
        ResultSet rs = null;
        Map<Integer, College> mp = new HashMap<Integer, College>();
        try {
            con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_COMMON, true);
            st = con.prepareStatement(sqlCollegeAll);
            rs = st.executeQuery();
            while (rs.next()) {
                College college = new College();
                college.setId(rs.getInt("id"));
                college.setName(rs.getString("name"));
                mp.put(college.getId(), college);
            }
            return mp;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            DataAccessMgr.getInstance().closeResultSet(rs);
            DataAccessMgr.getInstance().closeStatement(st);
            DataAccessMgr.getInstance().closeConnection(con);
        }
        return null;
    }
}

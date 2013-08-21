package com.xiaonei.platform.core.opt.jdbc.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;

public class UniversityDAO {

    private static UniversityDAO instance = new UniversityDAO();

    public static UniversityDAO getInstance() {
        if (instance == null) {
            synchronized (UniversityDAO.class) {
                if (instance == null) {
                    instance = new UniversityDAO();
                }
            }
        }
        return instance;
    }

    private UniversityDAO() {
        super();
    }

    protected static final String view = "select univ.id, univ.name, univ.count_m, univ.count_f, univ.url, univ.count_star, univ.count_a, univ.tribe, univ.ann_count, univ.logo_url, univ.country_id, univ.province_id ";

    protected static final String view2 = "select univ.id, univ.name, univ.country_id , univ.province_id ";

    private static final String sqlGet = view + " from university univ where univ.id = ?";

    public University get(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGet, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }
        };
        University uni = (University) DataAccessMgr.getInstance().queryUnique(op);
        if (uni == null) {
            uni = new University();
        }
        return uni;
    }

    private static final String sqlGetByTribe = view + " from university univ where univ.tribe = ?";

    public University getUnivByTribe(final int tribeId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetByTribe, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, tribeId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }
        };
        University uni = (University) DataAccessMgr.getInstance().queryUnique(op);
        if (uni == null) {
            uni = new University();
        }
        return uni;
    }

    private static final String sqlGetUnivTribe = view
            + " from university univ where univ.tribe = ?";

    public University isUnivTribe(final int tribeId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetUnivTribe, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, tribeId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }
        };
        University uni = (University) DataAccessMgr.getInstance().queryUnique(op);
        return uni;
    }

    private static final String sqlGetUnivByProv = view2
            + " from university univ where univ.country_id = ? and univ.province_id = ? order by id";

    @SuppressWarnings("unchecked")
    public List<University> getUnivByProv(final int countryId, final int provId)
            throws SQLException {
        OpList op = new OpList(sqlGetUnivByProv, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, countryId);
                ps.setInt(2, provId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUnivList(rs);
            }
        };
        return DataAccessMgr.getInstance().queryList(op);
    }

    private static final String sqlGetUnivByCountry = view2
            + " from university univ where univ.country_id = ? order by id";

    /**
     * 获取一个国家的所有大学
     * 
     * @author zhangjie
     * @param countryId
     * @return
     * @throws SQLException
     */
    @SuppressWarnings("unchecked")
    public List<University> getUnivByCountry(final int countryId) throws SQLException {
        OpList op = new OpList(sqlGetUnivByCountry, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, countryId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUnivList(rs);
            }
        };
        return DataAccessMgr.getInstance().queryList(op);
    }

    private static final String sqlGetOnePopUniv = "select univ.id, univ.name from university univ where univ.id > 1000 and univ.count_star > 200 and (id DIV 1000)=(? DIV 1000) order by rand() limit 1";

    /**
     * 得到和登录者属于同一个省的随机大学 add by lifs
     * 
     * @param userUnivId
     * @return
     * @throws SQLException
     */
    public University getOnePopUniv(final int userUnivId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetOnePopUniv, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userUnivId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                University univ = new University();
                univ.setId(rs.getInt("id"));
                univ.setName(rs.getString("name"));
                return univ;
            }
        };
        University uni = (University) DataAccessMgr.getInstance().queryUnique(op);
        if (uni == null) {
            uni = new University();
        }
        return uni;
    }

    private static final String sqlGetSearch = view + " from university2 univ where univ.id = ?";

    public University getSearch(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetSearch, DataAccessMgr.BIZ_SEARCH) { //BIZ_SEARCH

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }
        };
        University uni = (University) DataAccessMgr.getInstance().queryUnique(op);
        if (uni == null) {
            uni = new University();
        }
        return uni;
    }

    private static final String sqlGetAll = view + " from university univ";

    @SuppressWarnings("unchecked")
    public List<University> getAllUniversity() throws SQLException {
        OpList op = new OpList(sqlGetAll, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                //ps.setInt(1, 1000);
                //ps.setInt(2, 60000);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }
        };
        return DataAccessMgr.getInstance().queryList(op);
    }

    private static final String sqlGetAllShortName = "select univ.id, univ.name from univ_short univ where univ.id > ? and univ.id < ? order by id";

    @SuppressWarnings("unchecked")
    public List<University> getAllUniversityShortName() throws SQLException {
        OpList op = new OpList(sqlGetAllShortName, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, 1000);
                ps.setInt(2, 60000);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUnivShortName(rs);
            }
        };
        return DataAccessMgr.getInstance().queryList(op);
    }

    private static final String sqlGetShortName = "select univ.id,univ.name from univ_short univ where univ.id = ?";

    public University getUniversityShortName(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetShortName, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUnivShortName(rs);
            }
        };
        return (University) DataAccessMgr.getInstance().queryUnique(op);
    }

    private static final String sqlGetPopList = view
            + " from university univ where univ.id > ? and univ.count_star > ?";

    @SuppressWarnings("unchecked")
    public List<University> getPopList(final int count) throws SQLException {
        OpList op = new OpList(sqlGetPopList, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, 1000);
                ps.setInt(2, count);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }

        };
        return DataAccessMgr.getInstance().queryList(op);
    }

    @SuppressWarnings("unchecked")
    public List<University> getPopListForSearch(final int count) throws SQLException {
        OpList op = new OpList(view
                + " from university2 univ where univ.id > ? and univ.count_star > ?",
                DataAccessMgr.BIZ_COMMON) { //BIZ_SEARCH

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, 1000);
                ps.setInt(2, count);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }

        };
        return DataAccessMgr.getInstance().queryList(op);
    }

    private static final String sqlGetByUrl = view + "from university univ where univ.url = ?";

    public University getByUrl(final String url) throws SQLException {
        OpUniq op = new OpUniq(sqlGetByUrl, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, url);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }

        };
        return (University) DataAccessMgr.getInstance().queryUnique(op);
        //		Query qr = ss.createQuery("select univ from University univ where univ.url = ?").setString(0, url);
        //		return (University) qr.uniqueResult();
    }

    private static final String sqlGetByRegion = view
            + "from university univ where univ.id > ? and univ.id < ? order by id";

    @SuppressWarnings("unchecked")
    public List<University> getByRegion(final int reBegin, final int reEnd) throws SQLException {
        OpList op = new OpList(sqlGetByRegion, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, reBegin);
                ps.setInt(2, reEnd);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUniv(rs);
            }

        };
        return DataAccessMgr.getInstance().queryList(op);
    }

    private final static String sqlGetEmaiList = "select email from univ_email where university = ? order by id";

    @SuppressWarnings("unchecked")
    public List<String> getEmailList(final int univ) throws SQLException {
        OpList op = new OpList(sqlGetEmaiList, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return rs.getString("email");
            }

        };
        return DataAccessMgr.getInstance().queryList(op);
    }

    private final static String sqlupdateUnivName = "update university set name = ? where id = ?";

    public int updateUnivName(final String name, final int univId) throws SQLException {
        OpUpdate op = new OpUpdate(sqlupdateUnivName, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, name);
                ps.setInt(2, univId);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlupdateUnivShortName = "update univ_short set name = ? where id = ?";

    public int updateUnivShortName(final String name, final int univId) throws SQLException {
        OpUpdate op = new OpUpdate(sqlupdateUnivShortName, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, name);
                ps.setInt(2, univId);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlupdateUnivCateName = "update univ_cate set name = ? where id = ?";

    public int updateUnivCateName(final String name, final int univId) throws SQLException {
        OpUpdate op = new OpUpdate(sqlupdateUnivCateName, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, name);
                ps.setInt(2, univId);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlupdateUnivLogoUrl = "update university set logo_url = ? where id = ?";

    public int updateUnivLogoUrl(final String logoUrl, final int univId) throws SQLException {
        OpUpdate op = new OpUpdate(sqlupdateUnivLogoUrl, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, logoUrl);
                ps.setInt(2, univId);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlIncNanCount = "update university set count_m = count_m + 1 where id = ?";

    public int incNanCount(final int univ) throws SQLException {
        OpUpdate op = new OpUpdate(sqlIncNanCount, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlDecNanCount = "update university set count_m = count_m - 1 where id = ?";

    public int decNanCount(final int univ) throws SQLException {
        OpUpdate op = new OpUpdate(sqlDecNanCount, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlIncNvCount = "update university set count_f = count_f + 1 where id = ?";

    public int incNvCount(final int univ) throws SQLException {
        OpUpdate op = new OpUpdate(sqlIncNvCount, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlDecNvCount = "update university set count_f = count_f - 1 where id = ?";

    public int decNvCount(final int univ) throws SQLException {
        OpUpdate op = new OpUpdate(sqlDecNvCount, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlIncStuCount = "update university set count_a = count_a + 1 where id = ?";

    public int incStuCount(final int univ) throws SQLException {
        OpUpdate op = new OpUpdate(sqlIncStuCount, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlDecStuCount = "update university set count_a = count_a - 1 where id = ?";

    public int decStuCount(final int univ) throws SQLException {
        OpUpdate op = new OpUpdate(sqlDecStuCount, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlInsertEmail = "insert into univ_email (university, email) values (?, ?)";

    public int saveEmail(final int univ, final String email) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertEmail, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
                ps.setString(2, email);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlIncStarCount = "update university set count_star = count_star + 1 where id = ?";

    public int incStarCount(final int univ) throws SQLException {
        OpUpdate op = new OpUpdate(sqlIncStarCount, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlDecStarCount = "update university set count_star = count_star - 1 where id = ?";

    public int decStarCount(final int univ) throws SQLException {
        OpUpdate op = new OpUpdate(sqlDecStarCount, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlRemoveEmailByUniv = "delete from univ_email where university = ?";

    public int removeEmailByUniv(final int univ) throws SQLException {
        OpUpdate op = new OpUpdate(sqlRemoveEmailByUniv, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    protected static University parseUniv(ResultSet rs) throws SQLException {
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

    protected static University parseUnivList(ResultSet rs) throws SQLException {
        University univ = new University();
        univ.setId(rs.getInt("id"));
        univ.setName(rs.getString("name"));
        univ.setCountryId(rs.getInt("country_id"));
        univ.setProvinceId(rs.getInt("province_id"));
        return univ;
    }

    /**
     * add by lifs 2007-01-11
     * 
     * @param rs
     * @return
     * @throws SQLException
     */
    protected static University parseUnivShortName(ResultSet rs) throws SQLException {
        University univ = new University();
        univ.setId(rs.getInt("id"));
        univ.setName(rs.getString("name"));
        return univ;
    }

    private static final String sqlInsertUnivNameLog = "insert into update_univname_log (univ_id,old_name,new_name,admin,time) values (?,?,?,?,now())";

    public int saveUnivNameLog(final int univ_id, final String oldName, final String newName,
            final int admin) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertUnivNameLog, DataAccessMgr.BIZ_ADMIN_CONTENT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, univ_id);
                ps.setString(2, oldName);
                ps.setString(3, newName);
                ps.setInt(4, admin);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
}

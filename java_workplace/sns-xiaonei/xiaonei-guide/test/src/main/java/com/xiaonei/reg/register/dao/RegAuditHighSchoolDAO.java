/**
 *
 */
package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.register.model.RegAppAudit;
import com.xiaonei.reg.register.model.RegAppAuditHighSchool;
import com.xiaonei.reg.register.model.RegAuditHighSchool;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Date;

/**
 * RegAuditHighSchoolDAO.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-10-8 下午12:25:25
 */
public class RegAuditHighSchoolDAO extends XnDAO {

    private static RegAuditHighSchoolDAO instance = new RegAuditHighSchoolDAO();

    public static RegAuditHighSchoolDAO getInstance() {
        return instance;
    }

    private RegAuditHighSchoolDAO() {
        super();
    }

    //	private final static String sqlGetAuditStatus_tmp = "SELECT status FROM user_audit_high_school_tmp WHERE id = ?";
    private final static String sqlGetAuditStatus = "SELECT status FROM user_audit_high_school" + RegUtil.audit_table + " WHERE id = ?";

    /**
     * 获得注册用户的审核状态
     *
     * @param id
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-10-8 下午04:06:24
     */
    public int getAuditStatus(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetAuditStatus, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

        };
        int status = DataAccessMgr.getInstance().queryInt(op);
        return status;
    }

    //	private static final String sqlGetAuditUser_tmp = "SELECT id, email, password, name, schoolid, schoolname, student_number, enrollyear, high_school_class, largeurl, mainurl, headurl, tinyurl, begin_time, ip,gender, school_type,inviter_id, inviter_group_name FROM user_audit_high_school_tmp WHERE id = ? AND status = ? ";
    private static final String sqlGetAuditUser = "SELECT id, email, password, name, schoolid, schoolname, student_number, enrollyear, high_school_class, largeurl, mainurl, headurl, tinyurl, begin_time, ip,gender, school_type,inviter_id, inviter_group_name FROM user_audit_high_school" + RegUtil.audit_table + " WHERE id = ? AND status = ? ";

    /**
     * 获得审核通过的用户
     *
     * @param id
     * @return
     * @throws SQLException
     * @author (wang-tai)tai.wang@opi-corp.com 2008-10-8 下午04:11:11
     */
    public RegAuditHighSchool getAuditUser(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetAuditUser, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
                ps.setInt(2, RegAuditHighSchool.AUDIT_STATUS_NEW);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseRegAudit(rs);
            }

        };
        return (RegAuditHighSchool) DataAccessMgr.getInstance().queryUnique(op);
    }

    private static RegAuditHighSchool parseRegAudit(ResultSet rs) throws SQLException {

        RegAuditHighSchool rah = new RegAuditHighSchool();

        rah.setId(rs.getInt("id"));
        rah.setEmail(rs.getString("email"));
        rah.setPassword(rs.getString("password"));
        rah.setName(rs.getString("name"));
        rah.setHighSchoolCode(rs.getInt("schoolid"));
        rah.setHighSchoolName(rs.getString("schoolname"));
        rah.setStudentNumber(rs.getString("student_number"));
        rah.setEnrollYear(rs.getInt("enrollyear"));
        rah.setHighSchoolClass(rs.getString("high_school_class"));
        rah.setLargeurl(rs.getString("largeurl"));
        rah.setMainurl(rs.getString("mainurl"));
        rah.setHeadurl(rs.getString("headurl"));
        rah.setTinyurl(rs.getString("tinyurl"));
        rah.setBeginTime(rs.getTimestamp("begin_time"));
        rah.setIp(rs.getString("ip"));
        rah.setGender(rs.getString("gender"));
        rah.setSchoolType(rs.getInt("school_type"));
        rah.setInviterId(rs.getInt("inviter_id"));
        rah.setGroupName(rs.getString("inviter_group_name"));
        return rah;
    }

    //	private final static String sqlGetRegAppAuditHS_tmp = "select from_id, from_name, app_id, invite_type, app_token from reg_app_audit_hs_tmp where id = ?";
    private final static String sqlGetRegAppAuditHS = "select from_id, from_name, app_id, invite_type, app_token from reg_app_audit_hs" + RegUtil.audit_table + " where id = ?";

    public RegAppAuditHighSchool getRegAppAuditHS(final int id) throws SQLException {

        OpUniq op = new OpUniq(sqlGetRegAppAuditHS, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            @Override
            public RegAppAuditHighSchool parse(ResultSet rs) throws SQLException {
                return parseRegAppAudit(rs);
            }
        };
        return (RegAppAuditHighSchool) DataAccessMgr.getInstance().queryUnique(op);
    }

    private RegAppAuditHighSchool parseRegAppAudit(ResultSet rs) throws SQLException {
        RegAppAuditHighSchool raa = new RegAppAuditHighSchool();
        raa.setFrom_id(rs.getInt(1));
        raa.setFrom_name(rs.getString(2));
        raa.setApp_id(rs.getInt(3));
        raa.setInvite_type(rs.getString(4));
        raa.setApp_token(rs.getString(5));
        return raa;
    }
    /*table:reg_app_audit_hs
      *
      * */
//	private final static String sqlInsertRegAppAuditHS_tmp = 
    //		"insert into reg_app_audit_hs_tmp (id, from_id, from_name, app_id, invite_type, app_token, bagin_time, status) values (?,?,?,?,?,?,?,?)";
    private final static String sqlInsertRegAppAuditHS =
            "insert into reg_app_audit_hs" + RegUtil.audit_table + " (id, from_id, from_name, app_id, invite_type, app_token, bagin_time, status) values (?,?,?,?,?,?,?,?)";

    public void insertRegAppAuditHS(final RegAppAuditHighSchool raa) throws SQLException {

        OpUpdate op = new OpUpdate(sqlInsertRegAppAuditHS, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, raa.getId());
                ps.setInt(2, raa.getFrom_id());
                ps.setString(3, raa.getFrom_name());
                ps.setInt(4, raa.getApp_id());
                ps.setString(5, raa.getInvite_type());
                ps.setString(6, raa.getApp_token());
                ps.setTimestamp(7, new Timestamp(new Date().getTime()));
                ps.setInt(8, RegAppAudit.APP_AUDIT_NEW);
            }
        };

        DataAccessMgr.getInstance().insert(op);
    }

    //	private final static String sqlInsert_tmp = "INSERT INTO user_audit_high_school_tmp (email, password, name, schoolid, schoolname, student_number, enrollyear, high_school_class, largeurl, mainurl, headurl, tinyurl, ip, begin_time, status, audit_user, gender, school_type, inviter_id, inviter_group_name) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    private final static String sqlInsert = "INSERT INTO user_audit_high_school" + RegUtil.audit_table + " (email, password, name, schoolid, schoolname, student_number, enrollyear, high_school_class, largeurl, mainurl, headurl, tinyurl, ip, begin_time, status, audit_user, gender, school_type, inviter_id, inviter_group_name) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    public int save(final RegAuditHighSchool rah) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, rah.getEmail());
                ps.setString(2, rah.getPassword());
                ps.setString(3, rah.getName());
                ps.setInt(4, rah.getHighSchoolCode());
                ps.setString(5, rah.getHighSchoolName());
                ps.setString(6, rah.getStudentNumber());
                ps.setInt(7, rah.getEnrollYear());
                ps.setString(8, rah.getHighSchoolClass());
                ps.setString(9, rah.getLargeurl());
                ps.setString(10, rah.getMainurl());
                ps.setString(11, rah.getHeadurl());
                ps.setString(12, rah.getTinyurl());
                ps.setString(13, rah.getIp());
                ps.setTimestamp(14, new Timestamp(new Date().getTime()));
                ps.setInt(15, RegAuditHighSchool.AUDIT_STATUS_NEW);
                ps.setInt(16, 0);
                ps.setString(17, rah.getGender());
                ps.setInt(18, rah.getSchoolType());
                ps.setInt(19, rah.getInviterId());
                ps.setString(20, rah.getGroupName());

            }
        };
        return DataAccessMgr.getInstance().insertReturnId(op);
    }

    //	private final static String sqlGetAudit_tmp = "SELECT * FROM user_audit_high_school_tmp WHERE id = ?";
    private final static String sqlGetAudit = "SELECT * FROM user_audit_high_school" + RegUtil.audit_table + " WHERE id = ?";

    public RegAuditHighSchool getAudit(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetAudit, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseRegAudit(rs);
            }

        };
        return (RegAuditHighSchool) DataAccessMgr.getInstance().queryUnique(op);
    }

    //	private final static String sqlUpdateReAudit_tmp = "UPDATE user_audit_high_school_tmp SET name = ?,enrollyear = ?,high_school_class = ?,largeurl = ?,mainurl = ?,headurl = ?,tinyurl = ?,begin_time = ?,status = ?,end_time = ? WHERE id = ?";
    private final static String sqlUpdateReAudit = "UPDATE user_audit_high_school" + RegUtil.audit_table + " SET name = ?,enrollyear = ?,high_school_class = ?,largeurl = ?,mainurl = ?,headurl = ?,tinyurl = ?,begin_time = ?,status = ?,end_time = ? WHERE id = ?";

    public void updateAudit(final RegAuditHighSchool rhs) throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateReAudit, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, rhs.getName());
                ps.setInt(2, rhs.getEnrollYear());
                ps.setString(3, rhs.getHighSchoolClass());
                ps.setString(4, rhs.getLargeurl());
                ps.setString(5, rhs.getMainurl());
                ps.setString(6, rhs.getHeadurl());
                ps.setString(7, rhs.getTinyurl());
                ps.setTimestamp(8, new Timestamp(new Date().getTime()));
                ps.setInt(9, RegAuditHighSchool.AUDIT_STATUS_NEW);
                ps.setTimestamp(10, new Timestamp(new Date().getTime()));
                ps.setInt(11, rhs.getId());
            }
        };
        DataAccessMgr.getInstance().update(op);
	}
}

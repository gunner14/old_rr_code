/**
 *
 */
package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Date;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.register.model.RegAppAudit;
import com.xiaonei.reg.register.model.RegAudit;

/**
 * RegAuditDAO.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-28 下午03:30:41
 */
public class RegAuditDAO extends XnDAO {

    public static RegAuditDAO getInstance() {
        return instance;
    }

    private RegAuditDAO() {
        super();
    }

    private static RegAuditDAO instance = new RegAuditDAO();

    // private final static String sqlInsertSchoolType_tmp =
    // "insert into middle_school_user_log_tmp (id, type) values (?, ?)";
    private final static String sqlInsertSchoolType = "insert into middle_school_user_log"
            + RegUtil.audit_table + " (id, type) values (?, ?)";

    public void saveSchoolType(final int id, final int type)
            throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertSchoolType,
                DataAccessMgr.BIZ_LOGGING) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
                ps.setInt(2, type);
            }
        };
        DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlGetJuniorSchoolIdByHighSchoolId = "select high_school_id from middle_school where junior_school_id = ?";

    public int getJuniorSchoolIdByHighSchoolId(final int id)
            throws SQLException {
        OpUniq op = new OpUniq(sqlGetJuniorSchoolIdByHighSchoolId,
                DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    // private final static String sqlGetSchoolType_tmp =
    // "select type from middle_school_user_log_tmp where id = ?";
    private final static String sqlGetSchoolType = "select type from middle_school_user_log"
            + RegUtil.audit_table + " where id = ?";

    public int getSchoolType(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetSchoolType, DataAccessMgr.BIZ_LOGGING) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    // private final static String sqlGetAuditStatus_tmp =
    // "select status from user_audit_tmp where id = ?";
    private final static String sqlGetAuditStatus = "select status from user_audit"
            + RegUtil.audit_table + " where id = ?";

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

    // private static final String sqlGetAuditList_tmp =
    // "select id, email, password, name, univid, univname, department, dormitory, student_number, largeurl, mainurl, headurl, tinyurl, begin_time, channel, ip ,gender,type_of_course, inviter_id, inviter_group_name from user_audit_tmp where id = ? and status = ? ";
    // private static final String sqlGetAuditList =
    // "select id, email, password, name, univid, univname, department, dormitory, student_number, largeurl, mainurl, headurl, tinyurl, begin_time, channel, ip ,gender,type_of_course, inviter_id, inviter_group_name from user_audit"
    // + RegUtil.audit_table + " where id = ? and status = ? ";
    private static final String sqlGetAuditList = "select id, email, password, name, univid, univname, department, dormitory, student_number, largeurl, mainurl, headurl, tinyurl, begin_time, channel, ip ,gender,type_of_course, inviter_id, inviter_group_name from user_audit"
            + RegUtil.audit_table + " where id = ? ";

    public RegAudit getAuditUser(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetAuditList, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                RegLogger.getLogger("regLog", "AUDIT").info("audit in " + id);
                ps.setInt(1, id);
            }

            @Override
            public RegAudit parse(ResultSet rs) throws SQLException {
                return parseRegAudit(rs);
            }

        };
        return (RegAudit) DataAccessMgr.getInstance().queryUnique(op);
    }

    private static RegAudit parseRegAudit(ResultSet rs) throws SQLException {
        RegAudit ra = new RegAudit();
        RegLogger.getLogger("regLog", "AUDIT").info(
                "audit out " + rs.getInt("id") + " " + rs.getString("email"));
        ra.setId(rs.getInt("id"));
        ra.setEmail(rs.getString("email"));
        ra.setPassword(rs.getString("password"));
        ra.setName(rs.getString("name"));
        ra.setUniversityId(rs.getInt("univid"));
        ra.setUniversityName(rs.getString("univname"));
        ra.setDepartment(rs.getString("department"));
        ra.setDormitory(rs.getString("dormitory"));
        ra.setStudentNumber(rs.getString("student_number"));
        ra.setLargeurl(rs.getString("largeurl"));
        ra.setMainurl(rs.getString("mainurl"));
        ra.setHeadurl(rs.getString("headurl"));
        ra.setTinyurl(rs.getString("tinyurl"));
        ra.setBeginTime(rs.getTimestamp("begin_time"));
        ra.setChannel(rs.getString("channel"));
        ra.setIp(rs.getString("ip"));
        ra.setGender(rs.getString("gender"));
        ra.setDegree(rs.getString("type_of_course"));
        ra.setInviterId(rs.getInt("inviter_id"));
        ra.setGroupName(rs.getString("inviter_group_name"));
        return ra;
    }

    // private final static String sqlGetRegAppAudit_tmp =
    // "select from_id, from_name, app_id, invite_type, app_token from reg_app_audit_tmp where id = ?";
    private final static String sqlGetRegAppAudit = "select from_id, from_name, app_id, invite_type, app_token from reg_app_audit"
            + RegUtil.audit_table + " where id = ?";

    public RegAppAudit getRegAppAudit(final int id) throws SQLException {

        OpUniq op = new OpUniq(sqlGetRegAppAudit, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            @Override
            public RegAppAudit parse(ResultSet rs) throws SQLException {
                return parseRegAppAudit(rs);
            }
        };
        return (RegAppAudit) DataAccessMgr.getInstance().queryUnique(op);
    }

    private RegAppAudit parseRegAppAudit(ResultSet rs) throws SQLException {
        RegAppAudit raa = new RegAppAudit();
        raa.setFrom_id(rs.getInt(1));
        raa.setFrom_name(rs.getString(2));
        raa.setApp_id(rs.getInt(3));
        raa.setInvite_type(rs.getString(4));
        raa.setApp_token(rs.getString(5));
        return raa;
    }

    // private final static String sqlInsert_tmp =
    // "insert into user_audit_tmp (email, password, name, univid, univname, department, dormitory, student_number, largeurl, mainurl, headurl, tinyurl, ip, begin_time, status, audit_user, channel, gender, type_of_course, inviter_id, inviter_group_name) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    private final static String sqlInsert = "insert into user_audit"
            + RegUtil.audit_table
            + " (email, password, name, univid, univname, department, dormitory, student_number, largeurl, mainurl, headurl, tinyurl, ip, begin_time, status, audit_user, channel, gender, type_of_course, inviter_id, inviter_group_name) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    public int save(final RegAudit ra) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, ra.getEmail());
                ps.setString(2, ra.getPassword());
                ps.setString(3, ra.getName());
                ps.setInt(4, ra.getUniversityId());
                ps.setString(5, ra.getUniversityName());
                ps.setString(6, ra.getDepartment());
                ps.setString(7, ra.getDormitory());
                ps.setString(8, ra.getStudentNumber());
                ps.setString(9, ra.getLargeurl());
                ps.setString(10, ra.getMainurl());
                ps.setString(11, ra.getHeadurl());
                ps.setString(12, ra.getTinyurl());
                ps.setString(13, ra.getIp());
                ps.setTimestamp(14, new Timestamp(new Date().getTime()));
                ps.setInt(15, RegAudit.AUDIT_STATUS_NEW);
                ps.setInt(16, 0);
                ps.setString(17, ra.getChannel());
                ps.setString(18, ra.getGender());
                ps.setString(19, ra.getDegree());
                ps.setInt(20, ra.getInviterId());
                ps.setString(21, ra.getGroupName());
            }
        };
        return DataAccessMgr.getInstance().insertReturnId(op);
    }

    /*
      * table:reg_app_audit
      */
    // private final static String sqlInsertRegAppAudit_tmp =
    // "insert into reg_app_audit_tmp (id, from_id, from_name, app_id, invite_type, app_token, bagin_time, status) values (?,?,?,?,?,?,?,?)";
    private final static String sqlInsertRegAppAudit = "insert into reg_app_audit"
            + RegUtil.audit_table
            + " (id, from_id, from_name, app_id, invite_type, app_token, bagin_time, status) values (?,?,?,?,?,?,?,?)";

    public void insertRegAppAudit(final RegAppAudit raa) throws SQLException {

        OpUpdate op = new OpUpdate(sqlInsertRegAppAudit,
                DataAccessMgr.BIZ_ADMIN_USER) {
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

    // private final static String sqlGetAudit_tmp =
    // "select * from user_audit_tmp where id = ?";
    private final static String sqlGetAudit = "select * from user_audit"
            + RegUtil.audit_table + " where id = ?";

    public RegAudit getAudit(final int id) throws SQLException {
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
        return (RegAudit) DataAccessMgr.getInstance().queryUnique(op);
    }

    // private final static String sqlUpdateReAudit_tmp =
    // "update user_audit_tmp set name = ?, department = ?, dormitory = ?, headurl = ?, mainurl = ?, largeurl = ?, tinyurl = ?, begin_time = ?, status = ?, end_time = ? where id = ?";
    private final static String sqlUpdateReAudit = "update user_audit"
            + RegUtil.audit_table
            + " set name = ?, department = ?, dormitory = ?, headurl = ?, mainurl = ?, largeurl = ?, tinyurl = ?, begin_time = ?, status = ?, end_time = ? where id = ?";

    public void updateAudit(final RegAudit ra) throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateReAudit,
                DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, ra.getName());
                ps.setString(2, ra.getDepartment());
                ps.setString(3, ra.getDormitory());
                ps.setString(4, ra.getHeadurl());
                ps.setString(5, ra.getMainurl());
                ps.setString(6, ra.getLargeurl());
                ps.setString(7, ra.getTinyurl());
                ps.setTimestamp(8, new Timestamp(new Date().getTime()));
                ps.setInt(9, RegAudit.AUDIT_STATUS_NEW);
                ps.setTimestamp(10, new Timestamp(new Date().getTime()));
                ps.setInt(11, ra.getId());
            }
        };
        DataAccessMgr.getInstance().update(op);
    }
}

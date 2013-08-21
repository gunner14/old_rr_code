package com.xiaonei.reg.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.guide.model.PendingUser;
import com.xiaonei.reg.guide.model.PendingUserLog;
import com.xiaonei.reg.guide.view.PendingUserLogView;
import com.xiaonei.reg.guide.view.ReAuditLogView;

public class PendingUserDAO extends XnDAO {

    private static PendingUserDAO instance = new PendingUserDAO();

    public static PendingUserDAO getInstance() {
        return instance;
    }

    private PendingUserDAO() {
        super();
    }

    private final static String sqlIsExists = "select id from pending_user where usr = ?";

    public boolean isExists(final int user) throws SQLException {
        OpUniq op = new OpUniq(sqlIsExists, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, user);
            }

        };
        return DataAccessMgr.getInstance().queryExist(op);
    }

    private final static String sqlGet = "select id, usr, time from pending_user where id = ?";

    public PendingUser get(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGet, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            public Object parse(ResultSet rs) throws SQLException {
                PendingUser pu = new PendingUser(rs.getInt("usr"));
                pu.setId(rs.getInt("id"));
                pu.setTime(rs.getTimestamp("time"));
                return pu;
            }

        };
        return (PendingUser) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlGetLog = "select admin,admin_name,count(1) total,sum(selected=1)s "
            + " from pending_user_log "
            + " where op_time between ? and ?  "
            + " group by admin ";

    @SuppressWarnings("unchecked")
    public List<PendingUserLogView> getAllLogList(final String btime, final String etime) throws SQLException {
        OpList op = new OpList(sqlGetLog, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, btime);
                ps.setString(2, etime);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                PendingUserLogView view = new PendingUserLogView();
                view.setAdmin(rs.getInt("admin"));
                view.setAdminName(rs.getString("admin_name"));
                view.setCount(rs.getInt("total"));
                view.setAddCount(rs.getInt("s"));
                return view;
            }
        };
        return (ArrayList<PendingUserLogView>) DataAccessMgr.getInstance().queryList(op);
    }

    private final static String sqlGetReauditLog = "select admin,admin_name,sum(page_count)pc from reaudit_log where op_time between ? and ? group by admin";

    @SuppressWarnings("unchecked")
    public List<ReAuditLogView> getReauditLog(final String btime, final String etime) throws SQLException {
        OpList op = new OpList(sqlGetReauditLog, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, btime);
                ps.setString(2, etime);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                ReAuditLogView view = new ReAuditLogView();
                view.setAdmin(rs.getInt("admin"));
                view.setAdminName(rs.getString("admin_name"));
                view.setCount(rs.getInt("pc"));
                return view;
            }
        };
        return (ArrayList<ReAuditLogView>) DataAccessMgr.getInstance().queryList(op);
    }

    private final static String sqlGetReauditLogById = "select auditor,auditor_name,sum(audit_count)ac from reaudit_log where admin=? and op_time between ? and ? group by auditor";

    @SuppressWarnings("unchecked")
    public List<ReAuditLogView> getReauditLogById(final int id, final String btime, final String etime) throws SQLException {
        OpList op = new OpList(sqlGetReauditLogById, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
                ps.setString(2, btime);
                ps.setString(3, etime);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                ReAuditLogView view = new ReAuditLogView();
                view.setAdmin(rs.getInt("auditor"));
                view.setAdminName(rs.getString("auditor_name"));
                view.setCount(rs.getInt("ac"));
                return view;
            }
        };
        return (ArrayList<ReAuditLogView>) DataAccessMgr.getInstance().queryList(op);
    }



    private final static String sqlGetPendingLogToday = "slect id,usr,time,admin,admin_name,op_time,selected from pending_user_log where op_time > curdate() and admin = ?";

    @SuppressWarnings("unchecked")
    public List<PendingUserLog> getPendingLogToday(final int id) throws SQLException {
        OpList op = new OpList(sqlGetPendingLogToday, DataAccessMgr.BIZ_ADMIN_USER) {
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            public Object parse(ResultSet rs) throws SQLException {
                PendingUserLog log = new PendingUserLog();
                log.setId(rs.getInt("id"));
                log.setUsr(rs.getInt("usr"));
                log.setTime(rs.getTimestamp("time"));
                log.setAdmin(rs.getInt("admin"));
                log.setAdminName(rs.getString("admin_name"));
                log.setOptime(rs.getTimestamp("op_time"));
                log.setOp(rs.getInt("selected"));
                return log;
            }
        };
        return (ArrayList<PendingUserLog>) DataAccessMgr.getInstance().queryList(op);
    }

    private final static String sqlGetPendingCountToday = "select count(1) from pending_user_log where op_time > curdate() and admin = ?";

    public int getPendingCountToday(final int id) throws SQLException {

        OpUniq op = new OpUniq(sqlGetPendingCountToday, DataAccessMgr.BIZ_ADMIN_USER) {
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlGetCountByAdmin = "select count(*) from pending_user_log where selected = ? and admin = ? and op_time between ? and ? ";

    public int getPendingListCountByAdmin(final int select, final int id, final String btime, final String etime) throws SQLException {
        OpUniq op = new OpUniq(sqlGetCountByAdmin, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, select);
                ps.setInt(2, id);
                ps.setString(3, btime);
                ps.setString(4, etime);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlGetCount = "select count(id) from pending_user where audit = 0";

    public int getCount() throws SQLException {
        OpUniq op = new OpUniq(sqlGetCount, DataAccessMgr.BIZ_ADMIN_USER);
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlGetCountByStage = "select count(pu.id) from pending_user pu where pu.stage = ? and pu.audit = 0";

    public int getCountByStage(final int stage) throws SQLException {
        OpUniq op = new OpUniq(sqlGetCountByStage, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, stage);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlGetCountByAudit = "select count(id) from pending_user where audit = ?";

    public int getCountByAudit(final int audit) throws SQLException {
        OpUniq op = new OpUniq(sqlGetCountByAudit, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, audit);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlGetCountByAuditAndStage = "select count(id) from pending_user where audit = ? and stage = ?";

    public int getCountByAuditAndStage(final int audit, final int stage) throws SQLException {
        OpUniq op = new OpUniq(sqlGetCountByAuditAndStage, DataAccessMgr.BIZ_ADMIN_USER) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, audit);
                ps.setInt(2, stage);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlInsert = "insert into pending_user (usr, time , flag, stage) values (?, ?, ?, ?)";

    public int save(final PendingUser pu) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, pu.getUser());
                ps.setTimestamp(2, new Timestamp(pu.getTime().getTime()));
                ps.setInt(3, PendingUser.FLAG_DEFAULT);
                ps.setInt(4, pu.getStage());
            }
        };
        int result = 0;
        try {
            result = DataAccessMgr.getInstance().update(op);
        }
        catch (Exception e) {
            if (!e.getMessage().startsWith("Duplicate")) {
                e.printStackTrace();
                throw new SQLException();
            }
        }

        return result;
    }

    public int save(final PendingUser pu, final int flag) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, pu.getUser());
                ps.setTimestamp(2, new Timestamp(pu.getTime().getTime()));
                ps.setInt(3, flag);
                ps.setInt(4, pu.getStage());
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    //private final static String sqlUpdateNotAudit = "update pending_user set audit = 0,audit_time = null where audit != 0 and unix_timestamp(now()) - unix_timestamp(audit_time) >= 3 * 60";
    private final static String sqlUpdateNotAudit = "update pending_user set audit = 0,audit_time = null where audit != 0 and audit_time < subdate(now(),interval ? minute)";

    public int updateNotAudit(final int minute) throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateNotAudit, DataAccessMgr.BIZ_ADMIN_USER) {
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, minute);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlRemoveAll = "delete from pending_user where usr = ?";

    public int removeAll(final PendingUser pu) throws SQLException {
        OpUpdate op = new OpUpdate(sqlRemoveAll, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, pu.getUser());
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlGetLogByOpTime = "select admin,admin_name,count(*) c from pending_user_log where op_time between ? and ? group by admin";

    @SuppressWarnings("unchecked")
    public List<PendingUserLogView> getLogList(final String btime, final String etime) throws SQLException {
        OpList op = new OpList(sqlGetLogByOpTime, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, btime);
                ps.setString(2, etime);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                PendingUserLogView view = new PendingUserLogView();
                view.setAdmin(rs.getInt("admin"));
                view.setAdminName(rs.getString("admin_name"));
                view.setCount(rs.getInt("c"));
                return view;
            }

        };
        return (ArrayList<PendingUserLogView>) DataAccessMgr.getInstance().queryList(op);
    }


    private final static String sqlGetLogByUserId = "select id,usr,time,admin,admin_name,op_time,selected from pending_user_log where usr = ?";

    @SuppressWarnings("unchecked")
    public List<PendingUserLog> getLogByUserId(final int userId) throws SQLException {
        OpList op = new OpList(sqlGetLogByUserId, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                PendingUserLog log = new PendingUserLog();
                log.setId(rs.getInt("id"));
                log.setUsr(rs.getInt("usr"));
                log.setTime(rs.getTimestamp("time"));
                log.setAdmin(rs.getInt("admin"));
                log.setAdminName(rs.getString("admin_name"));
                log.setOptime(rs.getTimestamp("op_time"));
                log.setOp(rs.getInt("selected"));
                return log;
            }

        };
        return (ArrayList<PendingUserLog>) DataAccessMgr.getInstance().queryList(op);
    }

    private final static String sqlUpdateAudit = "update pending_user set audit = ?,audit_time = now() where id = ?";

    public int updateAudit(final int id, final int audit) throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateAudit, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, audit);
                ps.setInt(2, id);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlSaveAuditLog = "insert into reaudit_log(admin,admin_name,audit_count,op_time) values(?,?,1,now())";

    public int saveAuditLog(final int admin, final String adminName) throws SQLException {
        OpUpdate op = new OpUpdate(sqlSaveAuditLog, DataAccessMgr.BIZ_ADMIN_USER) {
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, admin);
                ps.setString(2, adminName);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlSaveAuditLog2 = "insert into reaudit_log(admin,admin_name,auditor,auditor_name,audit_count,page_count,op_time) values(?,?,?,?,?,1,now()) ";

    public int saveAuditLog2(final int admin, final String adminName, final int auditor, final String auditorName, final int count) throws SQLException {
        OpUpdate op = new OpUpdate(sqlSaveAuditLog2, DataAccessMgr.BIZ_ADMIN_USER) {
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, admin);
                ps.setString(2, adminName);
                ps.setInt(3, auditor);
                ps.setString(4, auditorName);
                ps.setInt(5, count);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }


    private final static String sqlInsertLog = "insert into pending_user_log (usr,time,admin,admin_name,op_time,selected) values (?,?,?,?,now(),?)";

    public int saveLog(final int usr, final Date time, final int admin, final String adminName, final int selected) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertLog, DataAccessMgr.BIZ_ADMIN_USER) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {

                ps.setInt(1, usr);
                if (time != null)
                    ps.setTimestamp(2, new Timestamp(time.getTime()));
                else
                    ps.setTimestamp(2, null);
                ps.setInt(3, admin);
                ps.setString(4, adminName);
                ps.setInt(5, selected);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

}
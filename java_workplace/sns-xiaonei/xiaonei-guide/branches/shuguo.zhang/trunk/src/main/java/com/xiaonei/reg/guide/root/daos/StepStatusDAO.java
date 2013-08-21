package com.xiaonei.reg.guide.root.daos;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.guide.model.StepStatus;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * @author wangtai
 *
 */
public class StepStatusDAO extends XnDAO {

    private static StepStatusDAO instance = new StepStatusDAO();

    public static StepStatusDAO getInstance() {
        return instance;
    }

    private StepStatusDAO() {
        super();
    }

    private final static String sqlGetStatus = "SELECT status FROM info_status WHERE id=?";

    /**
     * @param userId
     * @return
     * @throws SQLException
     */
    public int getStatusById(final int userId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetStatus, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlInsert = "INSERT INTO info_status (id, status) VALUES(?, ?)";

    public int add(final int userId, final int step) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
                ps.setInt(2, step);
            }
        };
        return DataAccessMgr.getInstance().insertReturnId(op);
    }

    public int updateStatus(final int userId, final int status) throws SQLException {

        String sqlUpdateStatus = "UPDATE info_status SET status = (status | " + status + ") WHERE id=" + userId;
        OpUpdate op = new OpUpdate(sqlUpdateStatus, DataAccessMgr.BIZ_COMMON) {
//		
//			@Override
//			public void setParam(PreparedStatement ps) throws SQLException {
//				ps.setInt(1, userId);
//				ps.setInt(2, status);
//			}
        };
        return DataAccessMgr.getInstance().update(op);
    }

    

    public int save(final int userId, final int status) throws SQLException {

        String sqlSave = "INSERT INTO info_status (id, status) VALUES("+userId+", "+status+") ON DUPLICATE KEY " +
        		"UPDATE status = (status | " + status + ") ";
        OpUpdate op = new OpUpdate(sqlSave, DataAccessMgr.BIZ_COMMON) {
//		
//			@Override
//			public void setParam(PreparedStatement ps) throws SQLException {
//				ps.setInt(1, userId);
//				ps.setInt(2, status);
//			}
        };
        return DataAccessMgr.getInstance().update(op);
    }

    
    
    private final static String sqlGetStepStatusById = "SELECT * FROM `info_status` WHERE `id`=? LIMIT 1";

    public StepStatus getStepStatusById(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetStepStatusById, DataAccessMgr.BIZ_COMMON) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

            @Override
            public StepStatus parse(ResultSet rs) throws SQLException {
                return parseStepStatusInfoView(rs);
            }
        };
        return (StepStatus) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlGetCount = "SELECT id FROM `info_status` WHERE `id`=? LIMIT 1";

    public int getCountById(final int id) throws SQLException {
        OpUniq op = new OpUniq(sqlGetCount, DataAccessMgr.BIZ_COMMON) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private StepStatus parseStepStatusInfoView(ResultSet rs) throws SQLException {
        StepStatus stepStatus = new StepStatus();
        stepStatus.setId(rs.getInt("id"));
        stepStatus.setStatus(rs.getInt("status"));
        return stepStatus;
    }
}

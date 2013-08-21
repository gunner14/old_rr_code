/**
 *
 */
package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.PendingUser;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;

/**
 * PendingUserDAO.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-12 下午08:41:36
 */
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


    private final static String sqlInsert = "insert into pending_user (usr, time , flag, stage) values (?, ?, ?, ?)";

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
}

/**
 *
 */
package com.xiaonei.reg.guide.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.common.RegUtil;

import java.sql.PreparedStatement;
import java.sql.SQLException;

/**
 * MiddleSchoolUserLogDAO
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:15:21
 */
public class MiddleSchoolUserLogDAO extends XnDAO {

    public static MiddleSchoolUserLogDAO getInstance() {
        return instance;
    }

    private MiddleSchoolUserLogDAO() {
        super();
    }

    private static MiddleSchoolUserLogDAO instance = new MiddleSchoolUserLogDAO();

    public void saveSchoolType(final int id, final int type) throws SQLException {
        final String sqlInsertSchoolType = "insert into middle_school_user_log" + RegUtil.audit_table + " (id, type) values (?, ?)";
        OpUpdate op = new OpUpdate(sqlInsertSchoolType, DataAccessMgr.BIZ_LOGGING) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
                ps.setInt(2, type);
            }
        };
        DataAccessMgr.getInstance().update(op);
    }

    public int getSchoolType(final int id) throws SQLException {
        final String sqlGetSchoolType = "select type from middle_school_user_log" + RegUtil.audit_table + " where id = ?";
        OpUniq op = new OpUniq(sqlGetSchoolType, DataAccessMgr.BIZ_LOGGING) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
            }

        };
        return DataAccessMgr.getInstance().queryInt(op);
    }
}

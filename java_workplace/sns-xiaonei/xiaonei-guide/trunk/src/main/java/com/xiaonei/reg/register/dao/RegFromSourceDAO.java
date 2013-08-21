package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * RegFromSourceDAO
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午03:27:53
 */
public class RegFromSourceDAO extends XnDAO {
    private static RegFromSourceDAO _instance = new RegFromSourceDAO();

    public static RegFromSourceDAO getInstance() {
        return _instance;
    }

    private RegFromSourceDAO() {
    }

    /**
     * 保存个人注册对应的来源
     *
     * @param id
     * @param from
     * @return 1 成功，0 失败
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午03:30:57
     */
    public int save(final String id, final String from) throws SQLException {
        final String sqlInsert = "insert into reg_from_source (id, from_source) values (?, ?) on duplicate key update from_source=?";
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, id);
                ps.setString(2, from);
                ps.setString(3, from);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    /**
     * 使用用户id，反查注册来源
     *
     * @param id
     * @return
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com) 2009-2-16 - 下午03:37:35
     */
    public String getFrom(final String id) throws SQLException {
        final String sqlGetFrom = "select from_source from reg_from_source where id=?";
        OpUniq op = new OpUniq(sqlGetFrom, DataAccessMgr.BIZ_STAT) {
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, id);
            }

            @Override
            public String parse(ResultSet rs) throws SQLException {
                return rs.getString("from_source");
            }
        };
        return (String) DataAccessMgr.getInstance().queryUnique(op);

	}
}

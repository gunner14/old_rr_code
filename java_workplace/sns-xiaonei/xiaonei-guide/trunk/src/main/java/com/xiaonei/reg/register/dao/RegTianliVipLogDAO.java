package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * RegTianliVipLogDAO
 *
 * @author wang-tai(tai.wang@opi-corp.com) 
 */
public class RegTianliVipLogDAO extends XnDAO {
    private static RegTianliVipLogDAO _instance = new RegTianliVipLogDAO();

    public static RegTianliVipLogDAO getInstance() {
        return _instance;
    }

    private RegTianliVipLogDAO() {
    }

    /**
     * @param tlAccount
     * @param xnAccount
     * @return 1 成功，0 失败
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com)
     */
    public int save(final String tlAccount, final String xnAccount) throws SQLException {
        final String sqlInsert = "insert into reg_tianli_vip_log (tl_account, xn_account) values (?, ?)";
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, tlAccount);
                ps.setString(2, xnAccount);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    /**
     * @param tlAccount
     * @return
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com)
     */
    public String getXnAccount(final String tlAccount) throws SQLException {
        final String sqlGetFrom = "select xn_account from reg_tianli_vip_log where tl_account=?";
        OpUniq op = new OpUniq(sqlGetFrom, DataAccessMgr.BIZ_REG_INVITE) {
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, tlAccount);
            }
            @Override
            public String parse(ResultSet rs) throws SQLException {
                return rs.getString("xn_account");
            }
        };
        return (String) DataAccessMgr.getInstance().queryUnique(op);
	}
    
    /**
     * @param xnAccount
     * @return
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com)
     */
    public String getTlAccount(final String xnAccount) throws SQLException {
        final String sqlGetFrom = "select tl_account from reg_tianli_vip_log where xn_account=?";
        OpUniq op = new OpUniq(sqlGetFrom, DataAccessMgr.BIZ_REG_INVITE) {
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, xnAccount);
            }
            @Override
            public String parse(ResultSet rs) throws SQLException {
                return rs.getString("tl_account");
            }
        };
        return (String) DataAccessMgr.getInstance().queryUnique(op);
	}
}

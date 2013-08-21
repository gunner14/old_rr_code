package com.xiaonei.reg.guide.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.guide.model.DeptAdd;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class DeptAddDAO extends XnDAO {

    public DeptAddDAO() {
        super();
    }

    private final static String sqlGetByDept = "select count(id) from deptadd where dept = ? and univ = ?";

    public int getByDept(final String dept, final int univ) throws SQLException {
        OpUniq op = new OpUniq(sqlGetByDept, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, dept);
                ps.setInt(2, univ);
            }
        };
        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlGetByUser = "select id, usr, univ, dept from deptadd where usr = ?";

    public DeptAdd getByUser(final int user) throws SQLException {
        OpUniq op = new OpUniq(sqlGetByUser, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, user);
            }

            public Object parse(ResultSet rs) throws SQLException {
                DeptAdd da = new DeptAdd(rs.getInt("univ"), rs.getInt("usr"), rs.getString("dept"));
                da.setId(rs.getInt("id"));
                return da;
            }

        };
        return (DeptAdd) DataAccessMgr.getInstance().queryUnique(op);//ForStat(op);
    }

    private final static String sqlInsert = "insert into deptadd (usr, univ, dept) values (?, ?, ?)";

    public int save(final DeptAdd da) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, da.getUser());
                ps.setInt(2, da.getUniv());
                ps.setString(3, da.getDept());
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlUpdate = "update deptadd set dept = ? where id = ?";

    public int update(final DeptAdd da) throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdate, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, da.getDept());
                ps.setInt(2, da.getId());
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
}

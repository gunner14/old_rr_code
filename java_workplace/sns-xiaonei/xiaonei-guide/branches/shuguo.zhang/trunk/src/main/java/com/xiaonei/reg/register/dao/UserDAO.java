/**
 * xiaonei-reg
 * com.xiaonei.reg.register.dao
 * UserDAO.java
 * Sep 10, 2009 - 11:12:06 AM
 */
package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

/**
 * UserDAO
 *
 * @author wangtai(tai.wang@opi-corp.com) Sep 10, 2009 - 11:12:06 AM
 *
 */
public class UserDAO extends XnDAO {


    private static UserDAO instance = new UserDAO();

    public static UserDAO getInstance() {
        return instance;
    }

    private UserDAO() {
        super();
    }
    
    /**
     * 得到一个未被使用过的用户id ^_^
     *
     * @return
     * @throws SQLException
     */
    public int getNext() throws SQLException {
    	final String sqlGetNext = "select nextval('users_id_seq'::text)";
        OpUniq op = new OpUniq(sqlGetNext, DataAccessMgr.BIZ_IDSEQUENCE);
        return DataAccessMgr.getInstance().queryId(op);
    }


    /**
     * 从验证码获得一个用户的信息<br>
     * 此方法可以检查验证码是否被使用过
     *
     * @param vc 验证码
     * @return 不存在返回null
     * @throws SQLException
     */
    public int getByVc(final String vc) throws SQLException {
    	final String sqlGetByVc = "select usr.id from users usr where usr.verify_code = ?";
        OpUniq op = new OpUniq(sqlGetByVc, DataAccessMgr.BIZ_USER_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, vc);
            }

        };
        return DataAccessMgr.getInstance().queryInt(op);
    }
    
}

package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.Email;

/**
 * 负责注册时发送邮件
 * <p/>
 * EmailDAO.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-23 下午08:56:37
 */
public class EmailDAO extends XnDAO {

    private static EmailDAO instance = new EmailDAO();

    public static EmailDAO getInstance() {
        return instance;
    }
    
    public EmailDAO() {
        super();
    }

    private final static String sqlInsert = "insert into email (email_to, email_from, body, title, addtime, disc) values (?, ?, ?, ?, ?, ?)";

    public int save(final Email email) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, email.getEmailTo());
                ps.setString(2, email.getEmailFrom());
                ps.setString(3, email.getBody());
                ps.setString(4, email.getTitle());
                ps.setTimestamp(5, new Timestamp(email.getAddtime().getTime()));
                ps.setInt(6, email.getDisc());
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
}
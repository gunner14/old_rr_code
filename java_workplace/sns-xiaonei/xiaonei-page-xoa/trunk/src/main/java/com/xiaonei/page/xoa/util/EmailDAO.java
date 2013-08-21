package com.xiaonei.page.xoa.util;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;

import com.xiaonei.admin.biz.model.invite.Email;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.xce.XceAdapter;

class EmailDAO extends XnDAO {

    private static EmailDAO instance = new EmailDAO();

    public static EmailDAO getInstance() {
        return instance;
    }

    public EmailDAO() {
        super();
    }

    /**
     * 获得对数据库的链接
     * 
     * @param isWrite
     * @return
     * @throws java.sql.SQLException
     */
    public static Connection getConn(boolean isWrite)
            throws java.sql.SQLException {
        if (isWrite) {
            return XceAdapter.getInstance().getWriteConnection(
                    XceAdapter.DB_GIFT);
        } else {
            return XceAdapter.getInstance().getReadConnection(
                    XceAdapter.DB_GIFT);
        }
    }

    private final static String sqlGetSender = "select host, usr, pass from email_sender es where ? regexp es.email limit 1";

    public String[] getSender(final String email) throws SQLException {
        OpUniq op = new OpUniq(sqlGetSender, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, email);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                String[] ob = new String[5];
                ob[0] = rs.getString("host");
                ob[1] = rs.getString("usr");
                ob[2] = rs.getString("pass");
                return ob;
            }

        };
        return (String[]) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlIsEyouEmail = "select em.email as ee from eyou_email em where ? regexp em.email";

    public boolean isEyouEmail(final String email) throws SQLException {
        OpUniq op = new OpUniq(sqlIsEyouEmail, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, email);
            }

        };
        return DataAccessMgr.getInstance().queryExist(op);
    }

    private final static String sqlGetDup = "select id, email_to, email_from, body, title, addtime, disc from email where email_to = ? and title = ? limit 1";

    public Email getDup(final String email_to, final String title)
            throws SQLException {
        OpUniq op = new OpUniq(sqlGetDup, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, email_to);
                ps.setString(2, title);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                Email email = new Email(rs.getString("email_to"), rs
                        .getString("email_from"), rs.getString("title"), rs
                        .getString("body"));
                email.setId(rs.getInt("id"));
                email.setDisc(rs.getInt("disc"));
                email.setAddtime(rs.getTimestamp("addtime"));
                return email;
            }

        };
        return (Email) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlGetFirst = "select id, email_to, email_from, body, title, addtime, disc from email order by disc desc, id limit 1";

    public Email getFirst() throws SQLException {
        OpUniq op = new OpUniq(sqlGetFirst, DataAccessMgr.BIZ_STAT) {

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                Email email = new Email(rs.getString("email_to"), rs
                        .getString("email_from"), rs.getString("title"), rs
                        .getString("body"));
                email.setId(rs.getInt("id"));
                email.setDisc(rs.getInt("disc"));
                email.setAddtime(rs.getTimestamp("addtime"));
                return email;
            }

        };
        return (Email) DataAccessMgr.getInstance().queryUnique(op);
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

    private final static String sqlReInsert = "insert into email (email_to, email_from, body, title, addtime, disc) values (?, ?, ?, ?, ?, ?)";

    /**
     * 给未注册的用户重新发送邀请邮件，要插入到另外一个库里面
     * 
     * @param email
     * @return
     * @throws SQLException
     */
    public int[] save_resend(final Email email) throws SQLException {
        int[] returnValue = new int[0];
        Connection conn = getConn(true);
        if (null != conn) {
            Statement stat = conn.createStatement();
            stat.addBatch(sqlReInsert);
            returnValue = stat.executeBatch();
            if (null != stat) {
                stat.close();
            }
            if (null != conn) {
                conn.close();
            }
        }

        return returnValue;
    }

    private final static String sqlRemove = "delete from email where id = ?";

    public int remove(final Email email) throws SQLException {
        OpUpdate op = new OpUpdate(sqlRemove, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, email.getId());
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
}
package com.kaixin.register.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;

import com.kaixin.register.model.Email;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

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
		OpUpdate op = new OpUpdate(sqlInsert,DataAccessMgr.BIZ_KX_STAT) {
		
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
	
	/**
	 * 写入邮件到数据库并返回邮件ID
	 * @param email
	 * @return
	 * @throws SQLException
	 */
	public int saveWithIdReturn(final Email email) throws SQLException {
		OpUpdate op = new OpUpdate(sqlInsert,DataAccessMgr.BIZ_ACTIVATION_LETTER) {
		
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
		return DataAccessMgr.getInstance().insertReturnId(op);
	}
	
	private final static String sqlIsEyouEmail = "select em.email as ee from eyou_email em where ? regexp em.email";
    public boolean isEyouEmail(final String email) throws SQLException {
        OpUniq op = new OpUniq(sqlIsEyouEmail,DataAccessMgr.BIZ_STAT) {
            
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, email);
            }
            
        };
        return DataAccessMgr.getInstance().queryExist(op);
    }
}

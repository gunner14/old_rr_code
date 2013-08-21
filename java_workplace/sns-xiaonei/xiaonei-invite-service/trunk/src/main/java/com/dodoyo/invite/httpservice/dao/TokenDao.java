package com.dodoyo.invite.httpservice.dao;

import com.dodoyo.invite.httpservice.entity.Token;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class TokenDao extends XnDAO {
    public TokenDao() {
        super();
    }

    private static final String sqlInsert = "insert ignore into invite_service_token_history" +
            "(token, client_ip, action, params, create_time) values(?,?,?,?,now())";

    public int addToken(final Token token)
            throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, token.getToken());
                ps.setString(2, token.getClientIp());
                ps.setString(3, token.getAction());
                ps.setString(4, token.getParams());
            }
        };
        return DataAccessMgr.getInstance().update(op);

    }

    private static final String sqlSel = "select * from invite_service_token_history where token=?";

    public Token getToken(final String token)
            throws SQLException {
        OpUniq op = new OpUniq(sqlSel, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, token);
            }

            public Object parse(ResultSet rs) throws SQLException {
                return parseResult(rs);
            }
        };
        Object obj = DataAccessMgr.getInstance().queryUnique(op);
        if (obj == null)
            return null;
        return (Token) obj;

    }

    private Token parseResult(ResultSet rs) throws SQLException {
        Token token = null;
        token = new Token();
        token.setId(rs.getInt("id"));
        token.setAction(rs.getString("action"));
        token.setClientIp(rs.getString("client_ip"));
        token.setCreateTime(rs.getTimestamp("create_time"));
        token.setParams(rs.getString("params"));
        token.setToken(rs.getString("token"));
        return token;
    }

    public static void main(String[] args) throws Exception {
//		Token t = new Token();
//		t.setAction("test");
//		t.setClientIp("1.1.1.1");
//		t.setParams("ip,action,");
//		t.setToken("12312asdfasdfasdfadfasdf");
        Token t = new TokenDao().getToken("12312asdfasdfasdfadfasdf");
        System.out.println(t.getClientIp());
    }
}
package com.dodoyo.invite.httpservice.dao;

import com.dodoyo.invite.httpservice.entity.ClientInfo;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

public class ClientIPDao extends XnDAO {
    private ClientIPDao() {
        super();
    }

    private static final String sqlInsert = "insert ignore into invite_service_ip_list" +
            "(client_name,client_ip,client_key,type,expire_time,create_time) values(?,?,?,?,?,now())";

    public int addClientInfo(final ClientInfo ci)
            throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, ci.getClientName());
                ps.setString(2, ci.getClientIp());
                ps.setString(3, ci.getClientKey());
                ps.setInt(4, ci.getType());
                ps.setTimestamp(5, ci.getExpireTime());
            }
        };
        return DataAccessMgr.getInstance().update(op);

    }

    private static final String sqlSel = "select * from invite_service_ip_list where client_ip=?";

    public ClientInfo getClientInfo(final String clientIp)
            throws SQLException {
        OpUniq op = new OpUniq(sqlSel, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, clientIp);
            }

            public Object parse(ResultSet rs) throws SQLException {
                return parseResult(rs);
            }
        };
        Object obj = DataAccessMgr.getInstance().queryUnique(op);
        if (obj == null)
            return null;
        return (ClientInfo) obj;

    }

    private static final String sqlGetList = "select * from `invite_service_ip_list` limit 200";

    @SuppressWarnings("unchecked")
    public List<ClientInfo> getClientInfoList()
            throws SQLException {
        OpList op = new OpList(sqlGetList, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseResult(rs);
            }
        };

        return DataAccessMgr.getInstance().queryList(op);

    }

    private static final String sqlUpdate = "update invite_service_ip_list " +
            "set client_name=?, client_ip=?,type=?,client_key=?,expire_time=? where id=?";

    public int updateClientInfo(final ClientInfo ci)
            throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdate, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, ci.getClientName());
                ps.setString(2, ci.getClientIp());
                ps.setInt(3, ci.getType());
                ps.setString(4, ci.getClientKey());
                ps.setTimestamp(5, ci.getExpireTime());
                ps.setInt(6, ci.getId());
            }
        };
        return DataAccessMgr.getInstance().update(op);

    }

    private ClientInfo parseResult(ResultSet rs) throws SQLException {
        ClientInfo ci = new ClientInfo();
        ci.setId(rs.getInt("id"));
        ci.setClientName(rs.getString("client_name"));
        ci.setClientIp(rs.getString("client_ip"));
        ci.setClientKey(rs.getString("client_key"));
        ci.setType(rs.getInt("type"));
        ci.setExpireTime(rs.getTimestamp("expire_time"));
        ci.setCreateTime(rs.getTimestamp("create_time"));
        return ci;
    }

    public static void main(String[] args) throws Exception {
//		ClientInfo ci = new ClientInfo();
//		ci.setClientIp("2.2.2.2");
//		ci.setClientName("testclient");
//		ci.setExpireTime(new Timestamp(new Date().getTime()));
//		ci.setType(1);
//		new ClientIPDao().addClientInfo(ci);
//		ClientInfo ci = new ClientIPDao().getClientInfo("2.2.2.2");
//		System.out.println(ci.getType());

        List<ClientInfo> list = new ClientIPDao().getClientInfoList();
        for (ClientInfo ci : list) {
            System.out.println("---------" + ci.getClientName() + ", " + ci.getClientIp());
        }
        System.exit(0);
    }
}
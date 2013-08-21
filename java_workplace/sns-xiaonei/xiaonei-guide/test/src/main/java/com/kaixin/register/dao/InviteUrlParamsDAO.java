package com.kaixin.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.apache.commons.lang.StringUtils;

public class InviteUrlParamsDAO extends XnDAO {
    
    private static final String UUid = "eb89cbd0-80a0-a721-925d-6e8ba25eb43b";
    
    private static InviteUrlParamsDAO instance = new InviteUrlParamsDAO();

    public static InviteUrlParamsDAO getInstance() {
        return instance;
    }

    private InviteUrlParamsDAO() {
        super();
    }

    @SuppressWarnings("unused")
    private final static String sqlGetSubSourceByUuid =
            "select ss from invite_url_paras where uuid = ?";
    
    private final static String sqlGetSubSourceByUuid_base = "select ss from invite_url_paras";
    private final static String sqlGetSubSourceByUuid_end  = " where uuid = ?";
        
    public String getSubSourceByUuid(final String uuid) throws SQLException {
        if(StringUtils.isEmpty(uuid) || uuid.length() != UUid.length()){
            return "";
        }
        String uend = uuid.substring(uuid.length()-2);
        int tableTail = 0;
        try {
            tableTail = Integer.parseInt(uend,16);
        } catch (NumberFormatException e) {
            return "";
        }
        if(tableTail < 0) return "";
        String getSql = sqlGetSubSourceByUuid_base + "_" + tableTail + sqlGetSubSourceByUuid_end; 
        OpUniq op = new OpUniq(getSql, DataAccessMgr.BIZ_KX_INVITE_URL_PARAS) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, uuid);
            }

            @Override
            public String parse(ResultSet rs) throws SQLException {
                return rs.getString("ss");
            }
        };
        return (String) DataAccessMgr.getInstance().queryUnique(op);
    }
}
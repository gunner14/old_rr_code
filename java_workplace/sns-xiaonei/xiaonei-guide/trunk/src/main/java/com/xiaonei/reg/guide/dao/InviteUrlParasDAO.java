/**
 *
 */
package com.xiaonei.reg.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.register.model.InviteUrlParas;
import com.xiaonei.reg.usertrace.q.UTUtils;

/**
 * InviteUrlParasDAO.java
 *
 * @author tai.wang@opi-corp.com (2008-10-24 - 下午05:40:37)
 */
public class InviteUrlParasDAO extends XnDAO {
    
    private static final String UUid = "eb89cbd0-80a0-a721-925d-6e8ba25eb43b";
    private RegLogger logger = RegLogger.getLoggerWithType(this.getClass().getSimpleName(),UTUtils.authorEmail);
    private static final InviteUrlParasDAO _instance = new InviteUrlParasDAO();

    private InviteUrlParasDAO() {
        super();
    }

    public static InviteUrlParasDAO getInstance() {
        return _instance;
    }

    @SuppressWarnings("unused")
    private final static String sqlGetByUser = "select * from invite_url_paras where uuid = ?";
    private final static String sqlGetSubSourceByUuid_base = "select * from invite_url_paras";
    private final static String sqlGetSubSourceByUuid_end  = " where uuid = ?";
    
    public InviteUrlParas getParas(final String uuid) throws SQLException {
        if(StringUtils.isEmpty(uuid) || uuid.length() != UUid.length()){
            return null;
        }
        String uend = uuid.substring(uuid.length()-2);
        int tableTail = 0;
        try {
            tableTail = Integer.parseInt(uend,16);
        } catch (NumberFormatException e) {
            return null;
        }
        if(tableTail < 0) return null;
        String getSql = sqlGetSubSourceByUuid_base + "_" + tableTail + sqlGetSubSourceByUuid_end;
        logger.debug("[InviteUrlParamsDAO]uuid=" + uuid + ",sql=" + getSql);
        OpUniq op = new OpUniq(getSql, DataAccessMgr.BIZ_INVITE_URL_PARAS) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, uuid);
            }

            public Object parse(ResultSet rs) throws SQLException {
                InviteUrlParas paras = new InviteUrlParas();
                paras.setUuid(rs.getString("uuid"));
                paras.setUser_id(rs.getInt("user_id"));
                paras.setEmail_from(rs.getString("email_from"));
                paras.setEmail_to(rs.getString("email_to"));
                paras.setApp_id(rs.getInt("app_id"));
                paras.setApp_token(rs.getString("app_token"));
                paras.setBuddy_group(rs.getString("buddy_group"));
                paras.setCode(rs.getString("code"));
                paras.setSs(rs.getString("ss"));
                return paras;
            }
        };

        return (InviteUrlParas) DataAccessMgr.getInstance().queryUnique(op);
    }
}

package com.renren.sns.wiki.dao.adminwiki;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Collections;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

/**
 * com.xiaonei.sns.platform.core.rose.wiki.dao.WikiUserIdDAO
 * 没有insert方法,这个带insert方法
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-13
 * 
 */
public class WikiUserIdDAO extends XnDAO {

    private static WikiUserIdDAO instance = new WikiUserIdDAO();

    public static WikiUserIdDAO getInstance() {
        return instance;
    }

    private WikiUserIdDAO() {
    }

    /**
     * 加载开放策略
     * 
     * @param type : WikiUserIdsManager.WikiPolicyType
     * @return
     */
    @SuppressWarnings("unchecked")
    public List<Integer> getUserIds(final int type) {
        final OpList op = new OpList("SELECT userid FROM wiki_user_id where `type`=?",
                DataAccessMgr.BIZ_REG_INVITE) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, type);
            }

            @Override
            public Object parse(final ResultSet rs) throws SQLException {
                return rs.getInt("userid");
            }
        };
        try {
            return DataAccessMgr.getInstance().queryList(op);
        } catch (final SQLException e) {
            return Collections.emptyList();
        }
    }

    public int getUserIdByUserId(final int userId) {
        final OpUniq ou = new OpUniq("select userid from wiki_user_id where `userid`=?",
                DataAccessMgr.BIZ_REG_INVITE) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }
        };
        try {
            return DataAccessMgr.getInstance().queryId(ou);
        } catch (final SQLException e) {
            return 0;
        }
    }

    public int insert(final int userId, final int type) {
        final OpUpdate op = new OpUpdate("INSERT INTO wiki_user_id(userid,`type`) values(?,?)",
                "reg_invite") {

            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
                ps.setInt(2, type);
            }
        };
        try {
            return DataAccessMgr.getInstance().update(op);
        } catch (final SQLException e) {
            e.printStackTrace();
        }
        return 0;
    }
}

package com.xiaonei.sns.platform.core.rose.wiki.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Collections;
import java.util.List;

/**
 * Wiki开放策略DAO
 * User: shunlong.jin/David
 * Date: 12-5-29
 * Time: 下午3:24
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
    public List<Integer> getUserIds(final int type) {
        final OpList op = new OpList("SELECT userid FROM wiki_user_id where `type`=?", DataAccessMgr.BIZ_REG_INVITE) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1,type);
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
}

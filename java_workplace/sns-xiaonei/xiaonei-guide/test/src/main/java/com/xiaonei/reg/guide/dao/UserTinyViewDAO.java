package com.xiaonei.reg.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Date;
import java.util.List;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.permission.define.RelationDef;
import com.xiaonei.platform.core.utility.PartitionTableUtils;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.reg.guide.view.UserTinyView;

public class UserTinyViewDAO extends XnDAO {

    // doing
    private static final String view_doing = "select ub.id, ub.name, ub.tinyurl, ub.doing ";        

    @SuppressWarnings("unchecked")
    public List<UserTinyView> getOnLineFriendList(final int host, final int off, final int limit) throws SQLException {
        MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
        Object obj = mcm.get(MemCacheKeys.home_friends + host);

        if (obj != null)
            return ((List<UserTinyView>) obj);
        String sql = view_doing + "from online right join " + getTableName(host)
                + " rl1 on guest=online.id, user_basic ub where rl1.host = ? and rl1.disc = ? and rl1.guest = ub.id and ub.status < ? "
                + "order by online.type = 2 desc, online.time desc limit ?, ?";// 2008
        // -
        // 08
        // -
        // 18
        // 去掉
        // /*and
        // ub.selected
        // =1*/
        // 星级的限制（需求提出者：jing.huang/zui.zhang）
        OpList op = new OpList(sql, DataAccessMgr.BIZ_RELATION, PartitionTableUtils.getTableSuffix(host, 100)) {

            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, host);
                ps.setInt(2, RelationDef.uuFriend);
                ps.setInt(3, User.statusActivateVerified);
                ps.setInt(4, off);
                ps.setInt(5, limit);
            }

            @Override
            public Object parse(final ResultSet rs) throws SQLException {
                return parseUserDoingView(rs);
            }

        };
        List<UserTinyView> list = DataAccessMgr.getInstance().queryList(op);
        if (list != null && list.size() > 0) {
            mcm.set(MemCacheKeys.home_friends + host, list, (int) new Date(MemCacheKeys.minute * 30).getTime());
        }
        return list;
    }

    private UserTinyView parseUserDoingView(final ResultSet rs) throws SQLException {
        UserTinyView utv = new UserTinyView();
        utv.setId(rs.getInt("ub.id"));
        utv.setName(rs.getString("name"));
        utv.setTinyUrl(rs.getString("tinyurl"));
        utv.setDoing(rs.getString("doing"));
        return utv;
    }

    /**
     * 得到散表表名
     *
     * @param id
     * @return
     */
    private String getTableName(int id) {
        String table = "relation_";
        try {
            table += (int) (id % 100);
        } catch (Exception e) {
        	LogTimeUtils.logger.error(e);
        }
        return table;
	}

}

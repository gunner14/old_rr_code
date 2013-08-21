package com.renren.sns.guide.dao;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.xce.XceAdapter;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-7-7 下午01:53:52
 */
public class InviteListDAO {

    private InviteListDAO() {
    }

    private static InviteListDAO instance = null;

    public static synchronized InviteListDAO getInstance() {
        if (instance == null) {
            instance = new InviteListDAO();
        }
        return instance;
    }

    private static String TABLE_NAME = "invite_list";

    /**
     * 如果能查到就是导过人，如果不能查都就是没有导过人
     * 
     * @author wei.xiang
     * @throws SQLException
     * @return List<Integer>
     */
    @SuppressWarnings("unchecked")
    public List<Integer> getInviteList(final int userId) throws SQLException {

        StringBuffer sb = new StringBuffer();
        sb.append("select * from " + InviteListDAO.TABLE_NAME);
        sb.append(" where owner_id = " + userId + " LIMIT 0, 1");
        String bizName = XceAdapter.DB_STAT;
        if (OpiConstants.isKaixin()) {
            bizName = XceAdapter.DB_KX_STAT;
        }
        OpList op = new OpList(sb.toString(), bizName) {

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return rs.getInt("owner_id");
            }

        };
        return DataAccessMgr.getInstance().queryList(op);
    }

}

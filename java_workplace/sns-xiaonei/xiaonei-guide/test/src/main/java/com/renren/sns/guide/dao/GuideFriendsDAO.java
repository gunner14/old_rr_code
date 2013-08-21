package com.renren.sns.guide.dao;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.renren.sns.guide.utils.GuideUtils;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.utility.PartitionTableUtils;
import com.xiaonei.xce.XceAdapter;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-26 上午10:04:34
 */
public class GuideFriendsDAO {

    private GuideFriendsDAO() {
    }

    private static GuideFriendsDAO instance = null;

    public static synchronized GuideFriendsDAO getInstance() {
        if (instance == null) {
            instance = new GuideFriendsDAO();
        }
        return instance;
    }

    private static String TABLE_NAME = "buddyapply_applicant_";

    /**
     * 取出某个用户发出好友申请 接受方的id列表
     * 
     * @author michael
     * @throws SQLException
     * @return List<Integer>
     */
    @SuppressWarnings("unchecked")
    public List<Integer> getFriendRequestIdListByTimeDesc(final int hostId) throws SQLException {

        StringBuffer sb = new StringBuffer();
        sb.append("select accepter from ");
        sb.append(GuideUtils.getTable(GuideFriendsDAO.TABLE_NAME, hostId));
        sb.append(" where applicant = " + hostId + " order by time desc");
        OpList op = new OpList(sb.toString(), XceAdapter.DB_BUDDYAPPLY_APPLICANT,PartitionTableUtils.getTableSuffix(hostId, 100)) {

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return rs.getInt("accepter");
            }

        };

        return DataAccessMgr.getInstance().queryList(op);
    }

}

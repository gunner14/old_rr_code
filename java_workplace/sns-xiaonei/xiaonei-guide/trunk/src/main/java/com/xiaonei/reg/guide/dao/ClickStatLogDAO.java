package com.xiaonei.reg.guide.dao;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * <ul>
 * <li>
 * <p/>
 * <pre>
 * mysql&gt; desc stat_guide_clicklog;
 * mysql&gt; desc stat_click_log;
 * +-----------+----------+------+-----+---------+----------------+
 * | Field     | Type     | Null | Key | Default | Extra          |
 * +-----------+----------+------+-----+---------+----------------+
 * | id        | int(11)  | NO   | PRI | NULL    | auto_increment |
 * | userid    | int(11)  | NO   |     | NULL    |                |
 * | clicktime | datetime | NO   |     | NULL    |                |
 * | bid       | int(11)  | NO   |     | 0       |                |
 * | type1     | int(11)  | YES  |     | NULL    |                |
 * | type2     | int(11)  | YES  |     | NULL    |                |
 * | type3     | int(11)  | YES  |     | NULL    |                |
 * | type4     | int(11)  | YES  |     | NULL    |                |
 * | type5     | int(11)  | YES  |     | NULL    |                |
 * +-----------+----------+------+-----+---------+----------------+
 * </pre>
 * <p/>
 * </li>
 * <li>2008-12-23 下午08:19:39</li>
 * <li></li>
 * </ul>
 */
final public class ClickStatLogDAO {
    // private static String sqlInsert =
    // "insert ignore into stat_click_log (userid,clicktime,bid,type1,type2,type3,type4,type5) values (?,now(),?,?,?,?,?,?)";
    private static final String LOG_SPLIT = "|";
    private static final Log LOGGER = LogFactory.getLog("xnguide.clickstat");

    public void insertCountClick(final int bid, final int userid,
                                 final int type1, final int type2, final int type3, final int type4,
                                 final String str) {
        /*
           * OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {
           *
           * @Override public void setParam(PreparedStatement ps) throws
           * SQLException { ps.setInt(1, bid); ps.setInt(2, userid); ps.setInt(3,
           * type1); ps.setInt(4, type2); ps.setInt(5, type3); ps.setInt(6,
           * type4); ps.setInt(7, type5); } }; try {
           * DataAccessMgr.getInstance().insert(op); } catch (SQLException e) {
           * e.printStackTrace(); }
           */
        StringBuffer sb = new StringBuffer();
        sb.append(bid).append(LOG_SPLIT);
        sb.append(userid).append(LOG_SPLIT);
        sb.append(type1).append(LOG_SPLIT);
        sb.append(type2).append(LOG_SPLIT);
        sb.append(type3).append(LOG_SPLIT);
        sb.append(type4).append(LOG_SPLIT);
        sb.append(str).append(LOG_SPLIT);
        LOGGER.info(sb);
    }

    /**
     * 单例
     */
    private ClickStatLogDAO() {
        super();
    }

    private static ClickStatLogDAO _instance = new ClickStatLogDAO();

    public static ClickStatLogDAO getInstance() {
        return _instance;
    }

}

package com.xiaonei.reg.guide.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.SQLException;

public class GuideLogDAO extends XnDAO {

    private static GuideLogDAO instance = new GuideLogDAO();

    public static GuideLogDAO getInstance() {
        return instance;
    }

    private GuideLogDAO() {
        super();
    }

    private final static String sqlGetCount = "SELECT COUNT(*) FROM to_guide_user WHERE userid = ? AND step = ?";

    public int getLogCount(final int userId, final int step) throws SQLException {
        OpUniq op = new OpUniq(sqlGetCount, DataAccessMgr.BIZ_LOGGING) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
                ps.setInt(2, step);
            }
        };

        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlInsert = "INSERT IGNORE INTO to_guide_user(userid) VALUES(?)";

    public int save(final int userId) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_LOGGING) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }
        };

        return DataAccessMgr.getInstance().insertReturnId(op);
    }

    private final static String sqlInsertNew = "INSERT IGNORE INTO to_guide_user(userid, user_type, step) VALUES(?, ?, ?)";

    public int save(final int userId, final int userType, final int step) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertNew, DataAccessMgr.BIZ_LOGGING) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
                ps.setInt(2, userType);
                ps.setInt(3, step);
            }
        };

        return DataAccessMgr.getInstance().insertReturnId(op);
    }

    /**
     * ********************** 大学引导统计*****************************
     */
    private final static String sqlGetCountForUniv = "SELECT COUNT(*) FROM guide_stat_univ WHERE userid = ? AND step = ?";

    public int getLogCountForUniv(final int userId, final int step) throws SQLException {
        OpUniq op = new OpUniq(sqlGetCountForUniv, DataAccessMgr.BIZ_LOGGING) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
                ps.setInt(2, step);
            }
        };

        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlInsertNewForUniv = "INSERT IGNORE INTO guide_stat_univ(userid, step) VALUES(?, ?)";

    public int saveForUniv(final int userId, final int step) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertNewForUniv, DataAccessMgr.BIZ_LOGGING) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
                ps.setInt(2, step);
            }
        };

        return DataAccessMgr.getInstance().insertReturnId(op);
    }
    /**********
     * 高中引导统计，分别是完善资料页和guide页
     */
    /**
     * @author seraph
     * 给高中用户进行统计，返回插入结果
     *
     * @param userId 用户的Id
     * @param step 统计的步骤
     * @return 插入结果
     * @throws SQLException
     */
    /*private final static String sqlInsertNewForHighSchool = "INSERT IGNORE INTO guide_highschool(userid, step) VALUES(?,?)";
     public int saveForHighSchool(final int userId, final int step)throws SQLException{
         OpUpdate op = new OpUpdate(sqlInsertNewForHighSchool, DataAccessMgr.BIZ_LOGGING){
             @Override
             public void setParam(PreparedStatement ps) throws SQLException {
                 ps.setInt(1, userId);
                 ps.setInt(2, step);
             }
         };
         return DataAccessMgr.getInstance().insertReturnId(op);
     }*/
    /**
     * @author seraph
     * 给高中用户进行统计，返回统计数字
     * @param userId 用户的Id
     * @param step 统计的步骤
     * @return 统计数字
     * @throws SQLException
     */
    /*private final static String sqlGetCountForHighSchool = "SELECT COUNT(*) FORM guide_highschool WHERE userid = ? AND step = ?";
     public int getLogCountForHighSchool(final int userId, final int step)throws SQLException{

         OpUniq op = new OpUniq(sqlGetCountForHighSchool, DataAccessMgr.BIZ_LOGGING){
             @Override
             public void setParam(PreparedStatement ps) throws SQLException {
                 ps.setInt(1, userId);
                 ps.setInt(2, step);
             }
         };
         return DataAccessMgr.getInstance().queryInt(op);
     }*/

}

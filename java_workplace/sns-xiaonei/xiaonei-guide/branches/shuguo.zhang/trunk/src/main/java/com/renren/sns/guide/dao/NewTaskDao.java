package com.renren.sns.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.renren.sns.guide.model.GuideNewTask;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

public class NewTaskDao extends XnDAO {

    private static NewTaskDao instance = new NewTaskDao();

    public static NewTaskDao getInstance() {
        return instance;
    }

    private NewTaskDao() {
        super();
    }

    private final static String sqlGetTaskByUser = "select task_id, task_finished from guide_new_task where user_id = ? order by task_id";

    @SuppressWarnings("unchecked")
    public List<GuideNewTask> getTaskByUser(final int userId) throws SQLException {

        OpList op = new OpList(sqlGetTaskByUser, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseGuideNewTask(rs);
            }
        };
        List<GuideNewTask> list = (ArrayList<GuideNewTask>) DataAccessMgr.getInstance().queryList(op);
        return list;
    }

    private final static String sqlGetByUserAndTask = "select * from guide_new_task where user_id = ? and task_id = ? limit 1";

    public GuideNewTask getByUserAndTask(final int userId, final int taskId) throws SQLException {

        OpUniq op = new OpUniq(sqlGetByUserAndTask, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
                ps.setInt(2, taskId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseGuideNewTask(rs);
            }

        };

        return (GuideNewTask) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlSetNewTaskFinished = "update guide_new_task set task_finished = 1 where user_id = ? and task_id = ?";

    public int setNewTaskFinished(final int userId, final int taskId) throws SQLException {
        OpUpdate op = new OpUpdate(sqlSetNewTaskFinished, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
                ps.setInt(2, taskId);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    public void insertNewTask(final int userId, final int taskId, final String taskName,
            final int reward_score, final int pra_reward, final int task_finished) {
        String sqlInsert = "replace into guide_new_task set user_id=?,task_id=?,task_name=?,reward_score=?,pra_reward=?,task_finished=?";
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
                ps.setInt(2, taskId);
                ps.setString(3, taskName);
                ps.setInt(4, reward_score);
                ps.setInt(5, pra_reward);
                ps.setInt(6, task_finished);
            }
        };
        try {
            DataAccessMgr.getInstance().update(op);
        } catch (SQLException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public static GuideNewTask parseGuideNewTask(ResultSet rs) throws SQLException {
        GuideNewTask gnt = new GuideNewTask();
        gnt.setTaskId(rs.getInt("task_id"));
        gnt.setTaskFinished(rs.getInt("task_finished"));
        return gnt;
    }

}

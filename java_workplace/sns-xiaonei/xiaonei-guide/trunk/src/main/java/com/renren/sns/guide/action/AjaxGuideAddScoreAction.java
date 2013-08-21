package com.renren.sns.guide.action;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.renren.sns.guide.dao.NewTaskDao;
import com.renren.sns.guide.model.GuideNewTask;
import com.renren.sns.guide.model.GuideNewTaskView;
import com.renren.sns.guide.model.TaskHashMap;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.BaseAction;
import com.xiaonei.wservice.userscore.client.StrategyType;
import com.xiaonei.wservice.userscore.client.UserScoreAdapter;

public class AjaxGuideAddScoreAction extends BaseAction {

    public ActionForward executeBt(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response) throws SQLException {
        String uid = request.getParameter("uid");
        String tid = request.getParameter("tid");
        int userId = 0, taskId = 0, reward_score = 0;
        try {
            if (uid != null && "".equals(uid)) {
                userId = Integer.parseInt(uid);
            } else {
                User host = BaseThreadUtil.currentHost(request);
                userId = host.getId();
            }
            if (tid != null && "".equals(tid)) {
                taskId = Integer.parseInt(tid);
            } else {
                taskId = 0;
            }    
            GuideNewTaskView gntv=TaskHashMap.getInstance().getTaskHashMap().get(taskId);
            if(gntv!=null){
                reward_score=gntv.getRewardScore();
            }            
            GuideNewTask gnt = NewTaskDao.getInstance().getByUserAndTask(userId, taskId);
            if (gnt != null && gnt.getTaskFinished() == 0) {
                UserScoreAdapter.getInstance().addScore(userId, StrategyType.GUIDE_TASK,reward_score);
                NewTaskDao.getInstance().setNewTaskFinished(userId, taskId);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

}

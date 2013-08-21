package com.xiaonei.reg.guide.action;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.guide.action.a090729.MyTimeCost;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveSupportFriendsUtil;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * 添加多个好友（preview.do）
 *
 * @author seraph  2008-3-17
 */
public class RequestMultiplyFriendAction extends ProtectAction {
	
	public ActionForward executePt(ActionMapping mapping, ActionForm form,final HttpServletRequest request, HttpServletResponse response)throws SQLException {
		MyTimeCost cost = MyTimeCost.begin();
		final User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		// 用户id组
		String user_id = request.getParameter("friends");
		cost.endStep();
		String[] user_ids = { "" };
		cost.endStep();
		if (user_id != null && !"".equalsIgnoreCase(user_id)) {
			user_ids = user_id.split(",");
			cost.endStep();
		}
		final String[] ids = user_ids;
		cost.endStep();
		try {
			cost.endStep();
			PrintWriter out = response.getWriter();
			final String currentDomain = OpiConstants.domain.toString();			
			//异步请求开始
			Runnable taskSave_NET = new Runnable() {
				public void run() {
					boolean error = false;
					MyTimeCost cost = MyTimeCost.begin();
					cost.endStep();
					if (ids != null && ids.length > 0) {// 分发多个请求
						for (int i = 0; i < ids.length; i++) {
							cost.endStep();
							int accepterId = 0;
							try {
								accepterId = Integer.parseInt(ids[i]);
							} catch (NumberFormatException e) {
								BaseActionUtil.addMsg(request, "您没有向任何人发出邀请");
								error = true;
								break;
							}
							cost.endStep();
							if (accepterId > 0) {
								User touser = SnsAdapterFactory.getUserAdapter().get(accepterId);
								if(host!=null && touser!=null){
								    //FriendApply fa=new FriendApply(host, touser,currentDomain);
						            try {
						            	//fa.setFrom("flash_old");
						                //FriendsHome.getInstance().requestFriendApply(fa);
						                GuideXFiveSupportFriendsUtil.beFriendsAsync(host, ids, currentDomain, "flash_old_action");
						            } catch (Exception e) {
						                // TODO Auto-generated catch block
						                e.printStackTrace();
						            }   									
								}
							}
							cost.endStep();
						}
						cost.endStep();
						if (!error){
							BaseActionUtil.addMsg(request, "已经成功向" + ids.length+ "人发出了添加为好友的请求，请等待对方确认");
						}
						cost.endStep();
						if (
						    host.getStage() == Stage.STAGE_MAJOR) {
							GuideLogger.writeLine(this,"to print 2 ...");
						} else {
							GuideLogger.writeLine(this,"to print 1 ...");
						}
						cost.endStep();
					} else {
						GuideLogger.writeLine(this,"to print 0 ...");
					}					
					cost.endFinally();
				}
			};
			RunnableTaskExecutor.getInstance().runTask(taskSave_NET);
			//异步请求结束
			GuideLogger.writeLine(this,"to print 1 ...");
			out.print("1");
			cost.endStep();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		cost.endFinally();
		return null;
	}
}

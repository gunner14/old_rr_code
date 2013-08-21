package com.xiaonei.platform.taglib.task;

import java.sql.SQLException;
import java.util.TimerTask;

import com.xiaonei.platform.taglib.dao.TaglibTempletDAO;
import com.xiaonei.platform.taglib.helper.TempletData;
import com.xiaonei.platform.taglib.model.TaglibTemplet;
import com.xiaonei.platform.taglib.utils.TagUtils;

/**
 * 
 * 
 * @author lifs
 * 
 */
public class NavigationTagTask extends TimerTask {

	/**
	 * 读取db中的tag的templet到变量中
	 */
	@Override
	public void run() {
		// 
		try {
			TaglibTemplet t1 = TaglibTempletDAO.getInstance().get(TempletData.Navigation_Utility);
			TaglibTemplet t2 = TaglibTempletDAO.getInstance().get(TempletData.Navigation_NavLeft);
			TaglibTemplet t3 = TaglibTempletDAO.getInstance().get(TempletData.Navigation_NavRight);
			TaglibTemplet t4 = TaglibTempletDAO.getInstance().get(TempletData.Navigation_Replace_SuiBianKanKan);
			TaglibTemplet t5 = TaglibTempletDAO.getInstance().get(TempletData.Navigation_Replace_NewEvent);
			TaglibTemplet t6 = TaglibTempletDAO.getInstance().get(TempletData.Navigation_Replace_NewMessageCount);
			TaglibTemplet t7 = TaglibTempletDAO.getInstance().get(TempletData.Navigation_Replace_ImNotice);

			if (t1 != null) {
				String templet = t1.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Navigation_Utility, templet);
			}
			if (t2 != null) {
				String templet = t2.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Navigation_NavLeft, templet);
			}
			if (t3 != null) {
				String templet = t3.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Navigation_NavRight, templet);
			}

			if (t4 != null) {
				String templet = t4.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Navigation_Replace_SuiBianKanKan, templet);
			}
			if (t5 != null) {
				String templet = t5.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Navigation_Replace_NewEvent, templet);
			}

			if (t6 != null) {
				String templet = t6.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Navigation_Replace_NewMessageCount, templet);
			}
			if (t7 != null) {
				String templet = t7.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Navigation_Replace_ImNotice, templet);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
}
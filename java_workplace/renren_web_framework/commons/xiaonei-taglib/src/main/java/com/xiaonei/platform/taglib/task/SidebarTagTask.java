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
public class SidebarTagTask extends TimerTask {

	/**
	 * 读取db中的tag的templet到变量中
	 */
	@Override
	public void run() {
		// 
		try {
			TaglibTemplet wrap1 = TaglibTempletDAO.getInstance().get(TempletData.Sidebar_Wrapper_sidebar);
			TaglibTemplet wrap2 = TaglibTempletDAO.getInstance().get(TempletData.Sidebar_Wrapper_loginpanel);
			TaglibTemplet t1 = TaglibTempletDAO.getInstance().get(TempletData.Sidebar_Login);
			TaglibTemplet t2 = TaglibTempletDAO.getInstance().get(TempletData.Sidebar_QuickLink);
			TaglibTemplet t3 = TaglibTempletDAO.getInstance().get(TempletData.Sidebar_YouCan);

			TaglibTemplet t4 = TaglibTempletDAO.getInstance().get(TempletData.Sidebar_Search);
			TaglibTemplet t5 = TaglibTempletDAO.getInstance().get(TempletData.Sidebar_AdminMenu);
			TaglibTemplet t6 = TaglibTempletDAO.getInstance().get(TempletData.Sidebar_SuperAdminMenu);
			if (wrap1 != null) {
				String templet = wrap1.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Sidebar_Wrapper_sidebar, templet);
			}
			if (wrap2 != null) {
				String templet = wrap2.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Sidebar_Wrapper_loginpanel, templet);
			}
			if (t1 != null) {
				String templet = t1.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Sidebar_Login, templet);
			}
			if (t2 != null) {
				String templet = t2.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Sidebar_QuickLink, templet);
			}
			if (t3 != null) {
				String templet = t3.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Sidebar_YouCan, templet);
			}
			//

			if (t4 != null) {
				String templet = t4.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Sidebar_Search, templet);
			}
			if (t5 != null) {
				String templet = t5.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Sidebar_AdminMenu, templet);
			}
			if (t6 != null) {
				String templet = t6.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Sidebar_SuperAdminMenu, templet);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
}
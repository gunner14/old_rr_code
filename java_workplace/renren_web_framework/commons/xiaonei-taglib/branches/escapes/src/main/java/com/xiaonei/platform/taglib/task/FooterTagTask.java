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
public class FooterTagTask extends TimerTask {

	/**
	 * 读取db中的tag的templet到变量中
	 */
	@Override
	public void run() {
		// 
		try {
			TaglibTemplet t1 = TaglibTempletDAO.getInstance().get(TempletData.Footer_LinkBar);
			TaglibTemplet t2 = TaglibTempletDAO.getInstance().get(TempletData.Footer_Icp);
			TaglibTemplet t3 = TaglibTempletDAO.getInstance().get(TempletData.Footer_Net_Police);
			
			if (t1 != null) {
				String templet = t1.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Footer_LinkBar, templet);
			}

			if (t2 != null) {
				String templet = t2.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Footer_Icp, templet);
			}
			if (t3 != null) {
				String templet = t3.getTemplet();
				templet = TagUtils.replaceTempletURL(templet);
				TempletData.getInstance().setMap(TempletData.Footer_Net_Police, templet);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}

}
package com.xiaonei.platform.taglib.task;

import java.sql.SQLException;
import java.util.TimerTask;

import com.xiaonei.platform.taglib.dao.TaglibTempletDAO;
import com.xiaonei.platform.taglib.helper.TempletData;
import com.xiaonei.platform.taglib.model.TaglibTemplet;

/**
 * 
 * 
 * @author lifs
 * 
 */
public class HeaderTagTask extends TimerTask {

	/**
	 * 读取db中的tag的templet到变量中
	 */
	@Override
	public void run() {
		// 
//		try {
//			TaglibTemplet tt = TaglibTempletDAO.getInstance().get("header");
//			if (tt != null) {
//				String templet = tt.getTemplet();
//				TempletData.getInstance().setHeaderTemplet(templet);
//			}
//		} catch (SQLException e) {
//			e.printStackTrace();
//		}
	}

}
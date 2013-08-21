package com.xiaonei.reg.register.logic.additional;

import java.sql.SQLException;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.register.dao.RegReferDAO;

public class RegReferLogic {

	private RegReferLogic() {
	}

	public static RegReferLogic getInstance() {
		return Singleton.getInstance(RegReferLogic.class);
	}

	/**
	 * set
	 * 
	 * @param userid
	 * @param sPageId
	 * @param refer
	 * 
	 * @author tai.wang@opi-corp.com Dec 15, 2009 - 5:41:17 PM
	 */
	public void set(final int userid, final String sPageId, final String refer) {
		if ("none".equals(refer) || StringUtils.isEmpty(refer)) {
			RegLogger.getLoggerWithType(this.getClass()).debug(
					"refer is empty " + refer);
			return;
		}
		if (!StringUtils.isNumeric(sPageId) || StringUtils.isEmpty(sPageId)) {
			RegLogger.getLoggerWithType(this.getClass()).debug(
					"pageId is not numeric " + sPageId);
			return;
		}
		final int pageId = Integer.valueOf(sPageId);
		if (206212 != pageId// 主动注册
				&& 206207 != pageId // 首次访问
		) {
			RegLogger.getLoggerWithType(this.getClass()).debug(
					"pageId is not in range " + pageId);
			return;
		}
		final Runnable task = new Runnable() {
			@Override
			public void run() {
				try {
					RegLogger.getLoggerWithType("RegReferLogic").info(
							"save ok "+userid+" "+pageId+" "+refer);
					RegReferDAO.getInstance().save(userid, pageId, refer);
				} catch (final SQLException e) {
					e.printStackTrace();
				}
			}
		};
		RunnableTaskExecutor.getInstance().runTask(task);
	}
}

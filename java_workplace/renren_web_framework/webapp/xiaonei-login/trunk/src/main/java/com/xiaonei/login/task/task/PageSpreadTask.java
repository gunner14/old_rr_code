package com.xiaonei.login.task.task;


import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.logic.logger.PpLogger;
import com.xiaonei.login.task.followmission.IFollowMissionTask;
import com.xiaonei.login.util.StatUtil;
import com.xiaonei.page.service.PageSpreadOnloginService;
import com.xiaonei.page.util.Keys;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

/**
 * TODO PageSpreadTask <br>
 *
 * @author tai.wang@opi-corp.com Apr 16, 2010 - 7:46:58 PM
 */
public class PageSpreadTask implements IFollowMissionTask {

	private int userid;
	private PpLogger log = LoginLoggerDef.feedTask;;
	@SuppressWarnings("unused")
    private String lastip;

	public PageSpreadTask(int userid, String lastip) {
		super();
		this.userid = userid;
		this.lastip = lastip;
	}

	@Override
	public String getName() {
		return "PageSpread";
	}

	@Override
	public void run() {
		StatUtil su = new StatUtil(log);
		try {
			log.info("execute fly task userid = " + userid);
			{
				MemCacheManager mem = MemCacheFactory.getInstance()
						.getManagerByPoolName(MemCacheKeys.pool_default);
				if (mem.get(Keys.PAGE_INPROCEEDING.getPrefix() + userid) == null) {
					su.stepTimeCost("1:FlyerTaskSet:get");
					PageSpreadOnloginService.es
							.submit(new PageSpreadOnloginService.Worker(userid));
					mem
							.set(
							        Keys.PAGE_INPROCEEDING.getPrefix()
											+ userid, "1");
					su.stepTimeCost("2:FlyerTaskSet:set");
				}
				log.info("execute PageSpreadOnloginService task userid = "
						+ userid);
			}
			su.stepTimeCost("3:FlyerTaskSet:execute");
			log.info(userid + " in lastip" + " over");
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		su.getWholeTime("FlyerTaskSuccFinish");
	}

}

package com.xiaonei.reg.guide.flows.xfive.logics;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.util.GuideLogger;

public class GuideXFiveUserPageLogic {
	
	//@Autowired PageServiceHome psh;
	
	/*private static GuideXFiveUserPageLogic instance = new GuideXFiveUserPageLogic();
	public static GuideXFiveUserPageLogic getInstance() {
		return instance;
	}*/
    @SuppressWarnings("unused")
    private void asyncBeFansofPage(final int pageid, final User host) {
		if (!"renren.com".equals(Globals.domain.toString())) {
			GuideLogger.printLog(" kaixin host:" + host.getId()	+ " will add renren pageid:[" + pageid + "] ");
			return;
		}
		GuideLogger.printLog(" host:" + host.getId() + " pageid:[" + pageid	+ "] ");
		if (pageid != 0) {
			try {
				Runnable task = new Runnable() {
					@Override
					public void run() {
						beFansofPage(pageid, host);
					}

				};
				RunnableTaskExecutor.getInstance().runTask(task);
			} catch (Exception e) {
				GuideLogger.printLog("beFans Error - host:" + host.getId() + " pageid:[" + pageid + "] " + e.toString(),GuideLogger.ERROR);
			}
		} else {
			return;
		}
	}
	@SuppressWarnings("unused")
    private void beFansofPage(String pageid, User host) {
		int pid = 0;
		try {
			pid = Integer.parseInt(pageid);
		} catch (Exception e) {
			GuideLogger.printLog("Parse Error - host:" + host.getId() + " pageid:[" + pageid + "] " + e.toString(),GuideLogger.ERROR);
		}
		if (pid != 0) {
			try {
				GuideLogger.printLog(" host:" + host.getId() + " pageid:[" + pageid + "] ");
			} catch (Exception e) {
				GuideLogger.printLog("beFans Error - host:" + host.getId() + " pageid:[" + pageid + "] " + e.toString(), GuideLogger.ERROR);
			}
		} else {
			return;
		}
	}
	private void beFansofPage(int pageid, User host) {
		int pid = pageid;
		if (pid != 0) {
			try {
				GuideLogger.printLog(" host:" + host.getId() + " pageid:[" + pageid + "] ");
			} catch (Exception e) {
				GuideLogger.printLog("beFans Error - host:" + host.getId() + " pageid:[" + pageid + "] " + e.toString(),GuideLogger.ERROR);
			}
		} else {
			return;
		}
	}
}

package com.xiaonei.commons.interceptors.unifiedentrance;

import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.dao.CoreConfigDAO;

/**
 * 通过动态配置来控制那些用户应用新版首页策略。
 * 这是个暂时性的接口，等新版首页全面上线之后就不用了。
 * 
 * @author Li Weibo
 * @deprecated 分尾号的控制策略已经下线
 */
public class UnifiedEntranceControlMgr {

	private static final int RELOAD_DELAY = 60 * 5;

	private static final String CONFIG_KEY1 = "unified_entrance_condition";

	//private static final String CONFIG_KEY2 = "unified_entrance_condition_v2";
	
	private static UnifiedEntranceControlMgr instance = new UnifiedEntranceControlMgr();

	public static UnifiedEntranceControlMgr getInstance() {
		return instance;
	}

	private ScheduledExecutorService timer = Executors
			.newSingleThreadScheduledExecutor();

	private Log logger = LogFactory.getLog(this.getClass());

	/**
	 * 初始化的时候给controlCondition一个默认值，"1:2"是永远不可能成立的一个条件，也就是说默认关闭
	 */
	private ControlCondition controlCondition1 = new ControlCondition("1:2");

	//private ControlCondition controlCondition2 = new ControlCondition("1:0");	//默认打开
	
	private UnifiedEntranceControlMgr() {
		startTimer();
	}

	private void startTimer() {
		try {
			timer.scheduleWithFixedDelay(new Runnable() {

				@Override
				public void run() {
					try {
						ControlCondition con1 = loadControlCondition(CONFIG_KEY1);
						if (con1 != null) {
							controlCondition1 = con1;
						}
					} catch (Throwable e) {
						e.printStackTrace();
					}
				}
			}, 0, RELOAD_DELAY, TimeUnit.SECONDS);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private ControlCondition loadControlCondition(String key) {
		if (logger.isDebugEnabled()) {
			logger.debug("reloading control condition:" + key);
		}
		try {
			String sCondition = CoreConfigDAO.getInstance().getCoreConfigValue(
					key);
			if (logger.isInfoEnabled()) {
				logger.info("new control condition loaded: " + key + "=" + sCondition);
			}
			return new ControlCondition(sCondition);
		} catch (Throwable e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * 尾号709，双栏新鲜事测试版
	 * @param host
	 * @return
	 */
	public boolean isUnifiedEntrance(User host) {
		if (host == null) {
			return false;
		}

		// guide用户看不到新版
		if (!host.getUserStateInfo().isGuideDone()) {
			return false;
		}
		ControlCondition condition = this.controlCondition1;

		if (condition == null) {
			return false;
		}

		int mod = condition.mod;
		int[] remainders = condition.remainders;
		if (mod == 0 || remainders == null) {
			return false;
		}

		int remainder = host.getId() % mod;
		for (int r : remainders) {
			if (r == remainder) {
				return true;
			}
		}
		return isInWhiteList(host.getId());
	}
	
	/**
	 * 线上版本。
	 * 完全上线后始终返回true
	 * @param host
	 * @return
	 */
	public boolean isUnifiedEntrance2(User host) {
		return true;
		/*if (host == null) {
			return false;
		}

		// guide用户看不到新版
		if (!host.getUserStateInfo().isGuideDone()) {
			return false;
		}
		ControlCondition condition = this.controlCondition2;

		if (condition == null) {
			return false;
		}

		int mod = condition.mod;
		int[] remainders = condition.remainders;
		if (mod == 0 || remainders == null) {
			return false;
		}

		int remainder = host.getId() % mod;
		for (int r : remainders) {
			if (r == remainder) {
				return true;
			}
		}
		return isInWhiteList2(host.getId());*/
	}

	private static Set<Integer> whiteList = new HashSet<Integer>();
	static {
		whiteList.add(1556200763);
		whiteList.add(224931222);
		whiteList.add(32715654);
		whiteList.add(33501183);
		whiteList.add(318826069);
		whiteList.add(258601212);
		whiteList.add(4402);
		whiteList.add(116710603);
		whiteList.add(244861456);
		whiteList.add(255329305);
		whiteList.add(260190016);
		whiteList.add(32715654);
		whiteList.add(258601212);
		whiteList.add(190136739);
		//whiteList.add(200000032);
		whiteList.add(229603878);
		whiteList.add(236469074);
		whiteList.add(228242829);
		whiteList.add(322719068);
		whiteList.add(278768721);
		whiteList.add(321352179);
		whiteList.add(273743367);
		whiteList.add(286418052);
		whiteList.add(230961237);
		whiteList.add(238489851);
		whiteList.add(220678672);
	}

	public static boolean isInWhiteList(int userId) {
		return whiteList.contains(userId);
	}
	
	private static class ControlCondition {
		private int mod;

		private int[] remainders;

		/**
		 * @param con
		 *            控制条件，格式为"10:3,5"，表示除以10余3或者5
		 */
		ControlCondition(String con) {
			String[] ss = con.split(":");
			mod = Integer.parseInt(ss[0]);
			String[] rs = ss[1].split(",");
			remainders = new int[rs.length];
			for (int i = 0; i < rs.length; i++) {
				remainders[i] = Integer.parseInt(rs[i]);
			}
		}
		
		public String toString() {
			StringBuilder sb = new StringBuilder();
			sb.append(mod);
			sb.append(":");
			for (int r : remainders) {
				sb.append(r);
				sb.append(",");
			}
			if (sb.charAt(sb.length() - 1) == ',') {
				sb.deleteCharAt(sb.length() - 1);
			}
			return sb.toString();
		}
	}

	public static void main(String[] args) {
		UnifiedEntranceControlMgr app = UnifiedEntranceControlMgr.getInstance();
		System.out.println(app.controlCondition1.toString());
		System.out.println("done");
	}
}

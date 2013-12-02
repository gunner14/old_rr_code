package xce.tripod.monitor;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import xce.tripod.Constants;
import xce.tripod.TripodException;
import xce.tripod.ZkClient;
import xce.tripod.monitor.logger.Logger;
import xce.tripod.monitor.util.WarningSender;

class ExitHandler extends Thread {

	TripodMonitor monitor = null;

	public ExitHandler(TripodMonitor monitor) {
		this.monitor = monitor;
	}

	public void run() {
		Logger.log("Exit");
		if (monitor != null) {
			monitor.stop();
		}
	}
}

class MonitorTask extends Thread {
	public static final int STATE_NA = -1;
	public static final int STATE_ALIVE = 0;
	public static final int STATE_DOWN = 1;

	private TripodMonitoredObject obj = null;
	private int state = STATE_NA;
	private long monitorInterval = 10; // ms
	private long warningInterval = 5000; // ms , 5min
	private boolean stopFlag = false;
	private int deadTimes = 0;

	public MonitorTask(TripodMonitoredObject obj, long monitorInterval, long warninginterval) {
		this.obj = obj;
		this.monitorInterval = monitorInterval;
		this.warningInterval = warninginterval;
	}

	public String getObjName() {
		return obj.getDesc();
	}

	public int getMonitorState() {
		return state;
	}

	public TripodMonitoredObject getMonitoredObject() {
		return obj;
	}

	public void safeStop() {
		stopFlag = true;
	}

	@Override
	public void run() {
		long leftTime = warningInterval;
		stopFlag = false;
		obj.init();
		if (obj != null) {
			Logger.log("Thread start. " + "Name: " + obj.getDesc() + " monitorInterval: " + monitorInterval
					+ " warningInterval: " + warningInterval);
			while (!stopFlag) {
				if (obj.isAlive()) {
					deadTimes = 0;
					Logger.log("[Alive] " + obj.getDesc());

					state = STATE_ALIVE;
				} else {
					Logger.log("[Dead] " + obj.getDesc());
					++deadTimes;
					if (deadTimes > 3) {
						deadTimes = 3;
						if (state != STATE_DOWN) {
							// 第一次检测到挂掉
							Logger.log("Warning: " + obj.getDesc() + " is down!");
							WarningSender.sendWarning(obj.getDesc() + " is down!");
						} else {
							// !! 此处假设warningInterval 大于monitorInterval
							leftTime -= monitorInterval;
							if (leftTime < 0) {
								leftTime = warningInterval;
								Logger.log("Warning: " + obj.getDesc() + " is down!");
								WarningSender.sendWarning(obj.getDesc() + " is down!");
							}
						}
						state = STATE_DOWN;
					}

					// Logger.log("[Alive] " + obj.getName());
				}
				try {
					Thread.sleep(monitorInterval);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			obj.cleanUp();
			Logger.log("Thread stoped. " + "Name: " + obj.getDesc() + " Interval: " + monitorInterval);
			state = STATE_NA;
		}
	}
}

/**
 * <p>
 * Title: TripodMonitor.java
 * </p>
 * <p>
 * Description:
 * </p>
 * <p>
 * Company: RenRen
 * </p>
 * 
 * @author xin.huang@renren-inc.com
 * @date 2012-5-29
 */
public class TripodMonitor {

	// public static final Logger logger =
	// xce.util.log.Log.getInstance().getLog();
	// public static final Log log = LogFactory.getLog("monitor_log");

	private Map<String, MonitorTask> monitorTasks = new HashMap<String, MonitorTask>();

	public void run() {
		for (Map.Entry<String, MonitorTask> entry : monitorTasks.entrySet()) {
			MonitorTask task = entry.getValue();
			task.start();
		}
	}

	public void stop() {
		for (Map.Entry<String, MonitorTask> entry : monitorTasks.entrySet()) {
			MonitorTask task = entry.getValue();
			task.safeStop();
		}

		for (Map.Entry<String, MonitorTask> entry : monitorTasks.entrySet()) {
			MonitorTask task = entry.getValue();
			try {
				task.join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public void registerAll(List<MonitorTask> tasksList) {
		for (MonitorTask task : tasksList) {
			register(task);
		}
	}

	public void update(List<MonitorTask> tasksList) {
		Set<String> nameSet = new HashSet<String>();
		for (MonitorTask task : tasksList) {
			nameSet.add(task.getObjName());
		}

		// 删除已经不需要监控的对象
		List<MonitorTask> removeList = new ArrayList<MonitorTask>();
		for (Map.Entry<String, MonitorTask> entry : monitorTasks.entrySet()) {
			String name = entry.getKey();
			if (!nameSet.contains(name)) {
				removeList.add(entry.getValue());
			}
		}

		for (MonitorTask task : removeList) {
			unregister(task);
		}

		for (MonitorTask task : tasksList) {
			if (!monitorTasks.containsKey(task.getObjName())) {
				register(task);
				// startMonitoring(task);
			}
		}

		printTasks();
	}

	public void register(MonitorTask task) {
		if (!monitorTasks.containsKey(task.getObjName())) {
			monitorTasks.put(task.getObjName(), task);
		}
		startMonitoring(task);
	}

	public void unregister(MonitorTask task) {
		stopMonitoring(task);
		monitorTasks.remove(task.getObjName());
	}

	public boolean stopMonitoring(MonitorTask task) {
		if (task.isAlive()) {
			task.safeStop();
		}
		int timeout = 100;
		while (task.isAlive()) {
			try {
				Thread.sleep(10);
				--timeout;
				if (timeout == 0) {
					return false;
				}
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return true;
	}

	public void startMonitoring(MonitorTask task) {
		if (!task.isAlive()) {
			task.start();
		}
	}

	public void clear() {
		stop();
		monitorTasks.clear();
	}

	public void printTasks() {
		for (Map.Entry<String, MonitorTask> entry : monitorTasks.entrySet()) {
			String desc = entry.getKey();
			Logger.log("[TaskDesc] " + desc);
		}
	}

	public void statistic() {
		for (Map.Entry<String, MonitorTask> entry : monitorTasks.entrySet()) {
			MonitorTask task = entry.getValue();
			int state = task.getMonitorState();
			String stateDesc = "";
			if (state == MonitorTask.STATE_ALIVE) {
				stateDesc = "[ALIVE]";
			} else {
				stateDesc = "[DOWN]";

			}
			Logger.log(stateDesc + " " + task.getMonitoredObject().getDesc());
		}
	}

	public static List<MonitorTask> getMonitorTaskList(long monitorInterval, long warningInterval) {
		List<MonitorTask> taskList = new ArrayList<MonitorTask>();

		ZkClient zkCli = null;
		try {
			zkCli = ZkClient.getInstance(Constants.DEFAULT_ZK_ADDRESS);
		} catch (TripodException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if (zkCli != null) {
			List<String> namespaceList = new ArrayList<String>();

			// 获取RedisCache列表-------------------------------------------------
			List<String> redisCacheList = new ArrayList<String>();
			namespaceList = zkCli.getNodes(Constants.DEFAULT_ZK_CACHE_ZNODE);
			for (String namespace : namespaceList) {
				redisCacheList = zkCli.getNodes(Constants.DEFAULT_ZK_CACHE_ZNODE + "/" + namespace);
				for (String redisCacheStr : redisCacheList) {
					RedisCacheObject obj = new RedisCacheObject(redisCacheStr);
					taskList.add(new MonitorTask(obj, monitorInterval, warningInterval));
				}
			}

			// 获取CacheManager列表-------------------------------------------------
			List<String> cacheManagerList = new ArrayList<String>();
			namespaceList = zkCli.getNodes(Constants.DEFAULT_ZK_CACHEMANAGERZNODE);
			for (String namespace : namespaceList) {
				cacheManagerList = zkCli.getNodes(Constants.DEFAULT_ZK_CACHEMANAGERZNODE + "/" + namespace);
				for (String cacheManagerStr : cacheManagerList) {
					CacheManagerObject obj = new CacheManagerObject(cacheManagerStr);
					taskList.add(new MonitorTask(obj, monitorInterval, warningInterval));
				}
			}

			// //
			// 获取ProducerManager列表-------------------------------------------------
			List<String> producerManagerList = new ArrayList<String>();
			namespaceList = zkCli.getNodes(Constants.DEFAULT_ZK_PRODUCERMANAGER_ZNODE);
			for (String namespace : namespaceList) {

				producerManagerList = zkCli.getNodes(Constants.DEFAULT_ZK_PRODUCERMANAGER_ZNODE + "/" + namespace);

				for (String producerManagerStr : producerManagerList) {
					ProducerManagerObject obj = new ProducerManagerObject(producerManagerStr);
					taskList.add(new MonitorTask(obj, monitorInterval, warningInterval));
				}
			}

		}
		return taskList;
	}

	public static void main(String[] args) {
		// DOMConfigurator.configure("log4j.xml");

		// Logger.log("123");

		if (args.length < 3) {
			System.out
					.println("Usage: TripodMonitor monitorInterval warningInterval updateInterval sendEmail sendSms [emailPropertyFile] [phoneNumberFile]");
			System.exit(-1);
		}

		long monitorInterval = Integer.parseInt(args[0]);
		long warningInterval = Integer.parseInt(args[1]);
		long updateInterval = Integer.parseInt(args[2]);

		if (args.length > 3) {
			int b = Integer.parseInt(args[3]);
			if (b > 0) {
				WarningSender.isSendEmail = true;
			} else {
				WarningSender.isSendEmail = false;
			}
		}

		if (args.length > 4) {
			int b = Integer.parseInt(args[4]);
			if (b > 0) {
				WarningSender.isSendSms = true;
			} else {
				WarningSender.isSendSms = false;
			}
		}

		if (args.length > 5) {
			WarningSender.propertyFile = args[5];
		}
		
		if (args.length > 6) {
			WarningSender.numberFile = args[6];
		}

		TripodMonitor monitor = new TripodMonitor();
		Logger.log("Start Monitor-----------------------------");

		monitor.run();

		Runtime.getRuntime().addShutdownHook(new ExitHandler(monitor));

		while (true) {
			try {
				Logger.log("Update---------------------------");
				List<MonitorTask> taskList = getMonitorTaskList(monitorInterval, warningInterval);
				Logger.log("Task Size: " + taskList.size());
				monitor.update(taskList);
				Thread.sleep(updateInterval);
			} catch (InterruptedException e) {
				e.printStackTrace();
				System.exit(0);
			}
		}
	}
}

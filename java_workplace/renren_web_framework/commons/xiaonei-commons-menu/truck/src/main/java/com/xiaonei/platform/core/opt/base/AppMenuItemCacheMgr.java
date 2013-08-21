package com.xiaonei.platform.core.opt.base;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.apache.log4j.Logger;

import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.base.menu.util.AppDynamicMenuItemUtil;
import com.xiaonei.platform.core.opt.jdbc.dao.AppMenuItemDAO;
import com.xiaonei.xce.storm.XceStormAdapter;
import com.xiaonei.xce.storm.XceStormListener;

/**
 * 
 * AppMenuItem本地缓存
 * 
 * @author Li Weibo
 * @since 2009-09-20
 */
public class AppMenuItemCacheMgr {

	/**
	 * 订阅XceStorm通知服务时用到的SVC ID
	 */
	private static final String SVC_ID = AppMenuItemCacheMgr.class.getName();
	
	private Logger logger = Logger.getLogger(AppMenuItemCacheMgr.class); 
	
	private static AppMenuItemCacheMgr instance = new AppMenuItemCacheMgr();
	
	/**
	 * 自动更新的时间间隔，单位分钟
	 */
	private static final int UPDATE_INTERVAL = 30;	//min
	
	/**
	 * XceStorm通知服务传递map中appId对应的key
	 */
	private static final String KEY_APP_ID = "app_id";
	
	/**
	 * XceStorm通知服务传递map中操作类型对应的key
	 */
	private static final String KEY_OP = "op";
	
	/**
	 * 更新操作
	 */
	private static final String VALUE_OP_UPDATE = "update";
	
	/**
	 * 删除操作
	 */
	private static final String VALUE_OP_REMOVE = "remove";
	
	public static AppMenuItemCacheMgr getInstance() {
		return instance;
	}
	
	/**
	 * 自动更新的调度线程
	 */
	private ScheduledExecutorService updateMonitor = Executors.newSingleThreadScheduledExecutor();
	
	/**
	 * AppMenuItem本地缓存容器
	 */
	private Map<Integer, AppMenuItem> map = new ConcurrentHashMap<Integer, AppMenuItem>();
	
	/**
	 * 用这个对象标识某个AppMenuItem不在DB中，防止重复查询
	 */
	private final AppMenuItem notInDBFlag = new AppMenuItem(AppMenuItem.THE_APP_NOT_IN_DB);
	
	private AppMenuItemCacheMgr() {
		try {
			startUpdateMoniter();
			subscribe();
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * 订阅XceStorm通知服务
	 */
	private void subscribe() {
		String msg = "subscribing XceStorm:" + SVC_ID;
		System.out.println(msg);
		if (logger.isInfoEnabled()) {
			logger.info(msg);
		}
		final AppMenuItemCacheMgr mgr = this;
		XceStormAdapter.getInstance().subscribe(SVC_ID, new XceStormListener(){
			
			private static final long serialVersionUID = 1L;

			@Override
			public void handle(Map<String, String> context) {
				
				String s_appId = context.get(KEY_APP_ID);
				if (s_appId == null) {
					logger.error("app_id param is null");
					return;
				}
				int appId = -1;
				try {
					appId = Integer.parseInt(s_appId);
				} catch (NumberFormatException e) {
					//do nothing
				}
				if (appId == -1) {
					logger.error("bad app_id:" + s_appId);
					return;
				}
				
				String op = context.get(KEY_OP);
				if (op != null) {
					
					if (op.equals(VALUE_OP_UPDATE)) {
						
						String msg = "AppMenuItemCacheMgr received a " + VALUE_OP_UPDATE + " notification:" + appId;
						System.out.println(msg);
						if (logger.isInfoEnabled()) {
							logger.info(msg);
						}
						mgr.update(appId);
					} else if (op.equals(VALUE_OP_REMOVE)) {
						
						String msg = "AppMenuItemCacheMgr received a " + VALUE_OP_REMOVE + " notification:" + appId;
						System.out.println(msg);
						if (logger.isInfoEnabled()) {
							logger.info(msg);
						}
						mgr.remove(appId);
					} else {
						logger.error("bad op param:" + op);
					}
					
				} else {
					logger.error("op param is null");
				}
			}
		});
	}
	
	/**
	 * 启动自动更新的调度任务
	 */
	private void startUpdateMoniter() {
		
		if (logger.isInfoEnabled()) {
			logger.info("start update monitor...");
		}
		updateMonitor.scheduleWithFixedDelay(new Runnable() {
			@Override
			public void run() {
				try {
					int count = 0;
					for (Integer appId : map.keySet()) {
						update(appId);
						count ++;
					}
					String msg = "Auto update AppMenuItem:" + count + " updated";
					System.out.println(msg);
					if (logger.isInfoEnabled()) {
						logger.info(msg);
					}
				} catch (Throwable e) {
					e.printStackTrace();
				}
			}
			
		}, UPDATE_INTERVAL, UPDATE_INTERVAL, TimeUnit.MINUTES);
	}
	

	/**
	 * 更新缓存中的指定对象
	 * @param appId
	 */
	public void update(int appId) {
		if (logger.isInfoEnabled()) {
			logger.info("update AppMenuItem:" + appId);
		}
		AppMenuItem item = getAppMenuItemFromDAO(appId);
		if (item != null) {
			put(appId, item);
		}
	}

	/**
	 * 将制定AppMenuItem对象放入本地缓存
	 * @param item
	 */
	private void put(int appId, AppMenuItem item) {
		if (item == null) {
			logger.error("IllegalArgument: AppMenuItem null");
			return;
		}
		if (logger.isInfoEnabled()) {
			logger.info("put AppMenuItem into cache:" + appId);
		}
		map.put(appId, item);
	}
	
	/**
	 * 获取AppMenuItem对象
	 * @param appId
	 * @return
	 */
	public AppMenuItem get(int appId) {
		AppMenuItem item = map.get(appId);
		if (item == null) {
			if (logger.isInfoEnabled()) {
				logger.info("AppMenuItem not hit in cache:" + appId);
			}
			item = getAppMenuItemFromDAO(appId);
			if (item != null) {
				put(appId, item);
			} else {
				logger.info("AppMenuItem not found in DB:" + appId);
				put(appId, notInDBFlag);
				return null;
			}
		} else {
			
			if (item == notInDBFlag) {
				if (logger.isDebugEnabled()) {
					logger.debug("AppMenuItem notInDBFlag hit in cache:" + appId + ", return null");
				}
				return null;
			} else {
				if (logger.isDebugEnabled()) {
					logger.debug("AppMenuItem hit in cache:" + appId);
				}
			}
		}
		return item;
	}
	
	/**
	 * 从数据库加载AppMenuItem对象，并动态计算Bookmark Url和urlTarget
	 * @param appId
	 * @return
	 */
	private AppMenuItem getAppMenuItemFromDAO(int appId) {
		AppMenuItem item = null;
		try {
			item = AppMenuItemDAO.getInstance().get(appId);
			if (item != null) {
				//将AppMenuItem对象中的动态字段（bookmark url & urlTarget）计算出来
				item = AppDynamicMenuItemUtil.populateDynamic(item);
			}
		}
		catch (SQLException e) {
			e.printStackTrace();
		}
		
		return item;
	}
	
	
	/**
	 * 删除指定AppMenuItem对象
	 * @param appId
	 * @return
	 */
	public AppMenuItem remove(int appId) {
		return map.remove(appId);
	}
	
	/**
	 * 发出更新通知
	 * @param appId
	 */
	public void notifiyToUpdate(int appId) {
		if (logger.isInfoEnabled()) {
			logger.info("sending an AppMenuItem update notification:" + appId);
		}
		try {
			Map<String, String> map = new HashMap<String, String>(1);
			map.put(KEY_OP, VALUE_OP_UPDATE);
			map.put(KEY_APP_ID, appId + "");
			XceStormAdapter.getInstance().notify(SVC_ID, map);
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * 发出删除通知
	 * @param appId
	 */
	public void notifiyToRemove(int appId) {
		if (logger.isInfoEnabled()) {
			logger.info("sending an AppMenuItem remove notification:" + appId);
		}
		try {
			Map<String, String> map = new HashMap<String, String>(1);
			map.put(KEY_OP, VALUE_OP_REMOVE);
			map.put(KEY_APP_ID, appId + "");
			XceStormAdapter.getInstance().notify(SVC_ID, map);
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}
}

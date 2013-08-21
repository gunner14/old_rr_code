package com.xiaonei.platform.core.opt.base.menu.util;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.BeanFactoryUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.stereotype.Component;

import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.model.ReserveMenu;
import com.xiaonei.platform.core.model.SubMenu;
import com.xiaonei.platform.core.opt.jdbc.home.AppMenuItemHome;
import com.xiaonei.platform.core.opt.jdbc.home.ReserveMenuHome;
import com.xiaonei.platform.core.opt.jdbc.home.SubMenuHome;
import com.xiaonei.xce.storm.XceStormAdapter;
import com.xiaonei.xce.storm.XceStormListener;

//没有使用getInstance那种单例模式，rose的Autowired 不就是一个单例吗，所以别自己new
@Component
public class MenuServiceImpl implements MenuService {
	
	@Autowired
	private ReserveMenuHome reserveMenuHome;
	
	@Autowired
	private SubMenuHome subMenuHome;
	
	private static final String SYNC_ID  = "com.xiaonei.platform.component.application.service.menu.MenuService";
	
	private Map<Integer, List<SubMenu>> subMenuMap;
	
	private Map<Integer, ReserveMenu> reserveMenuMap;
	
	private static ApplicationContext applicationContext;
	
	@Autowired
    public void setApplicationContext(ApplicationContext ac){
		MenuServiceImpl.applicationContext = ac;
    }
	
	private static MenuServiceImpl instance;
	
	public static MenuServiceImpl getInstance() {
		if (instance == null) {
			instance = (MenuServiceImpl)BeanFactoryUtils.beanOfType(applicationContext, MenuServiceImpl.class);
		}
        return instance;
	}
	
	public MenuServiceImpl() {
		register();
	}

	//从数据库加载
	private void getDataFromDB() {
		Map<Integer, ReserveMenu> reserveMenuMap = new HashMap<Integer, ReserveMenu>();
		Map<Integer, List<SubMenu>> subMenuMap = new HashMap<Integer, List<SubMenu>>();
		List<ReserveMenu> reserveMenuList = reserveMenuHome.getByEnable(1);
		if(reserveMenuList != null) {
			for(ReserveMenu rm : reserveMenuList) {
				int appId = rm.getAppId();
				AppMenuItem ami = AppMenuItemHome.getInstance().get(appId);
				if(ami != null) {
					rm.setAppName(ami.getAppName());
					rm.setAppIcon(ami.getAppIcon());
					rm.setSideNavUrl(ami.getSideNavUrl());
				}
				reserveMenuMap.put(appId, rm);
				List<SubMenu> subMenuList = subMenuHome.getEnableByAppId(appId, 1);
				subMenuMap.put(appId, subMenuList);
			}
		}
		
		this.reserveMenuMap = reserveMenuMap;
		this.subMenuMap = subMenuMap;
	}
	
	@PostConstruct
	public void preLoad() {
		getDataFromDB();
	}

	private void register() {
		final MenuServiceImpl menuService = this;
		
		XceStormAdapter.getInstance().subscribe(SYNC_ID, new XceStormListener() {
			private static final long serialVersionUID = 1L;
			@Override
			public void handle(Map<String, String> context) {
				menuService.getDataFromDB();
			}
		});
	}
	
	@Override
	public boolean notifyToReload() {
		try {
			Map<String, String> map = new HashMap<String, String>();
			XceStormAdapter.getInstance().notify(SYNC_ID, map);
			return true;
		} catch (Throwable e) {
			e.printStackTrace();
			return false;
		}
	}

	@Override
	public Set<Integer> getReserveMenuSet() {
		return subMenuMap.keySet();
	}

	@Override
	public Map<Integer, List<SubMenu>> getSubMenuMap() {
		return subMenuMap;
	}

	@Override
	public List<SubMenu> getSubMenuList(int appId) {
		return subMenuMap.get(appId);
	}

	@Override
	public ReserveMenu getReserveMenu(int appId) {
		return reserveMenuMap.get(appId);
	}

	@Override
	public Map<Integer, ReserveMenu> getReserveMenuMap() {
		return reserveMenuMap;
	}
}

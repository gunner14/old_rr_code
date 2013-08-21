package com.xiaonei.platform.core.opt.base.menu.util;

import java.util.List;
import java.util.Map;
import java.util.Set;

import com.xiaonei.platform.core.model.ReserveMenu;
import com.xiaonei.platform.core.model.SubMenu;

public interface MenuService {

	//public MenuService getInstance();
	
	//获得保留菜单所对应的appId的集合
	public Set<Integer> getReserveMenuSet();
	
	//返回一个map，key是一级菜单的appId，value是这个appId对应的二级菜单
	//二级菜单根据orderNum升序排列
	public Map<Integer, List<SubMenu>> getSubMenuMap();
	
	//根据给出的appId，返回appId对应的二级菜单列表
	//二级菜单根据orderNum升序排列
	public List<SubMenu> getSubMenuList(int appId);

	boolean notifyToReload();

	//根据appId返回保留一级菜单数据对象
	public ReserveMenu getReserveMenu(int appId);
	
	//返回一个map，key是一级菜单的appId，value是这个appId对应的一级菜单数据对象
	public Map<Integer, ReserveMenu> getReserveMenuMap();
	
}

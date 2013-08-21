package com.xiaonei.platform.core.opt.jdbc.home;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.xiaonei.platform.core.model.SubMenu;
import com.xiaonei.platform.core.opt.jdbc.dao.SubMenuDAO;


@Component
public class SubMenuHome {

	@Autowired
	private SubMenuDAO dao;
	
	public void save(SubMenu sm) {
		try {
			dao.save(sm);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void update(SubMenu sm) {
		try {
			dao.update(sm);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public void remove(int id) {
		try {
			dao.remove(id);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public SubMenu get(int id) {
		try {
			return dao.get(id);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public List<SubMenu> getByAppId(int appId) {
		try {
			return dao.getByAppId(appId);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public List<SubMenu> getEnableByAppId(int appId, int enable) {
		try {
			return dao.getEnableByAppId(appId, enable);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
}

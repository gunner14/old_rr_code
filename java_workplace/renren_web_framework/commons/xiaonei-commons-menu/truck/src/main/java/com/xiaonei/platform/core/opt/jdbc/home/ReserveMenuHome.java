package com.xiaonei.platform.core.opt.jdbc.home;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.xiaonei.platform.core.model.ReserveMenu;
import com.xiaonei.platform.core.opt.jdbc.dao.ReserveMenuDAO;


@Component
public class ReserveMenuHome {

	@Autowired
	private ReserveMenuDAO dao;
	
	public void saveOrUpdate(ReserveMenu rm) {
		try {
			dao.saveOrUpdate(rm);
		} catch (Exception e) {
            e.printStackTrace();
        }
	}
	
	public void remove(int appId) {
		try {
			dao.remove(appId);
		} catch (Exception e) {
            e.printStackTrace();
        }
	}
	
	public ReserveMenu get(int appId) {
		try {
			return dao.get(appId);
		} catch (Exception e) {
            e.printStackTrace();
        }
		return null;
	}
	
	public List<ReserveMenu> list() {
		try {
			return dao.list();
		} catch (Exception e) {
            e.printStackTrace();
        }
		return null;
	}
	
	public List<ReserveMenu> getByEnable(int enable) {
		try {
			return dao.getByEnable(enable);
		} catch (Exception e) {
            e.printStackTrace();
        }
		return null;
	}
}

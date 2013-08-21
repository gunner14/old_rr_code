package com.xiaonei.platform.core.opt.jdbc.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.xiaonei.jade.datasource.Catalogs;
import com.xiaonei.platform.core.model.ReserveMenu;

@DAO(catalog = Catalogs.APP_USER_MENU)
public interface ReserveMenuDAO {

	public static final String TABLE_NAME = " reserve_menu ";
	
	public static final String TABLE_FIELD = " app_id, enable, remove_flag, new_flag, operation_name, operation_uri, count_flag, new_window_flag, current_frame_flag ";
	
	//INSERT INTO reserve_menu(app_id, enable) VALUES (23163, false) ON DUPLICATE KEY UPDATE enable=false;
	@SQL("INSERT INTO " +
			  TABLE_NAME + "(" + TABLE_FIELD + ") " +
		 "VALUES " + 
		 	  "(:1.appId, :1.enable, :1.removeFlag, :1.newFlag, :1.operationName, :1.operationUri, :1.countFlag, :1.newWindowFlag, :1.currentFrameFlag) " +
		 "ON DUPLICATE KEY UPDATE " +
		 	  "enable=:1.enable, " +
		 	  "remove_flag=:1.removeFlag, " +
		 	  "new_flag=:1.newFlag, " +
		 	  "operation_name=:1.operationName, " +
		 	  "operation_uri=:1.operationUri, " +
		 	  "count_flag=:1.countFlag, " +
		 	  "new_window_flag=:1.newWindowFlag, " +
		 	  "current_frame_flag=:1.currentFrameFlag")
	public void saveOrUpdate(ReserveMenu rm);
	
	@SQL("DELETE FROM " +
			  TABLE_NAME + 
		 "WHERE " +
		 	  "app_id=:1")
	public void remove(int appId);
	
	@SQL("SELECT " +
			  TABLE_FIELD +
		 "FROM " +
		 	  TABLE_NAME +
		 "WHERE " +
		 	  "app_id=:1")
	public ReserveMenu get(int appId);
	
	@SQL("SELECT " +
			  TABLE_FIELD +
		 "FROM " +
		 	  TABLE_NAME)
	public List<ReserveMenu> list();
	
	@SQL("SELECT " +
			  TABLE_FIELD +
		 "FROM " +
		 	  TABLE_NAME +
		 "WHERE " +
		 	  "enable=:1")
	public List<ReserveMenu> getByEnable(int enable);
	
}

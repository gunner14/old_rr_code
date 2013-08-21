package com.xiaonei.platform.core.opt.jdbc.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.xiaonei.jade.datasource.Catalogs;
import com.xiaonei.platform.core.model.SubMenu;

@DAO(catalog = Catalogs.APP_USER_MENU)
public interface SubMenuDAO {

	public static final String TABLE_NAME = " sub_menu ";
	
	public static final String TABLE_FIELD = " id, app_id, menu_name, menu_uri, order_num, enable, remove_flag, new_flag, operation_name, operation_uri, count_flag, new_window_flag, current_frame_flag ";
	
	@SQL("INSERT INTO " +
			  TABLE_NAME + "(app_id, menu_name, menu_uri, order_num, enable, remove_flag, new_flag, operation_name, operation_uri, count_flag, new_window_flag, current_frame_flag ) " +
		 "VALUES " + 
		 	  "(:1.appId, :1.menuName, :1.menuUri, :1.orderNum, :1.enable, :1.removeFlag, :1.newFlag, :1.operationName, :1.operationUri, :1.countFlag, :1.newWindowFlag, :1.currentFrameFlag)")
	public void save(SubMenu sm);
	
	@SQL("UPDATE " +
			  TABLE_NAME +
		 "SET " + 
		 	  "app_id=:1.appId, " +
		 	  "menu_name=:1.menuName, " +
		 	  "menu_uri=:1.menuUri, " +
		 	  "order_num=:1.orderNum, " +
		 	  "enable=:1.enable, " +
		 	  "remove_flag=:1.removeFlag, " +
		 	  "new_flag=:1.newFlag, " +
		 	  "operation_name=:1.operationName, " +
		 	  "operation_uri=:1.operationUri, " +
		 	  "count_flag=:1.countFlag, " +
		 	  "new_window_flag=:1.newWindowFlag, " +
		 	  "current_frame_flag=:1.currentFrameFlag " +
		 "WHERE " +
		 	  "id=:1.id")
	public void update(SubMenu sm);
	
	@SQL("DELETE FROM " +
			  TABLE_NAME + 
		 "WHERE " +
		 	  "id=:1")
	public void remove(int id);
	
	@SQL("SELECT " +
			  TABLE_FIELD +
		 "FROM " +
		 	  TABLE_NAME +
		 "WHERE " +
		 	  "id=:1")
	public SubMenu get(int id);
	
	@SQL("SELECT " +
			  TABLE_FIELD +
		 "FROM " +
		 	  TABLE_NAME +
		 "WHERE " +
		 	  "app_id=:1 " +
		 "ORDER BY " +
		 	  "order_num ASC")
	public List<SubMenu> getByAppId(int appId);
	
	@SQL("SELECT " +
			  TABLE_FIELD +
		 "FROM " +
		 	  TABLE_NAME +
		 "WHERE " +
		 	  "app_id=:1 and enable=:2 " +
		 "ORDER BY " +
		 	  "order_num ASC")
	public List<SubMenu> getEnableByAppId(int appId, int enable);
}

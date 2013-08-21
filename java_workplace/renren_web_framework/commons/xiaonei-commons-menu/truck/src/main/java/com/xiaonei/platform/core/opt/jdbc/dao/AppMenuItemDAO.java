package com.xiaonei.platform.core.opt.jdbc.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
/**
 * 操作app_menu_item 表的DAO类
 * @author fusong.li@opi-corp.com
 * company site : www.xiaonei.com
 */
public class AppMenuItemDAO extends XnDAO{

	private static AppMenuItemDAO instance = new AppMenuItemDAO();

	public static AppMenuItemDAO getInstance() {
		return instance;
	}

	private AppMenuItemDAO() {
		super();
	}
    public static String fields = " app_id , app_name , app_icon , app_icon_24 , side_nav_url, wap_side_nav_url ";

	/**
	 * 启用新的接口
	 * @param appName
	 * @param appIcon 16X16 尺寸的icon
	 * @param appIcon24  24X24 尺寸的icon
	 * @param sideNavUrl
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public int update(final String appName,final String appIcon, final String appIcon24, final String sideNavUrl ,final int appId)throws SQLException{
		StringBuffer sqlInsert = new StringBuffer();
		sqlInsert.append("update app_menu_item set app_name = ? , app_icon=? , app_icon_24=? ,side_nav_url=? where app_id=?");
		OpUpdate op = new OpUpdate(sqlInsert.toString(),DataAccessMgr.BIZ_APP) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, appName);
				if(appIcon!=null && appIcon.length()>255){
					String icon = appIcon.substring(0, 254);
					ps.setString(2, icon);
				}else{
					ps.setString(2, appIcon);
				}
				if(appIcon24!=null && appIcon24.length()>255){
					String icon = appIcon24.substring(0, 254);
					ps.setString(3, icon);
				}else{
					ps.setString(3, appIcon24);
				}
				ps.setString(4, sideNavUrl);
				ps.setInt(5, appId);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	/**
	 * 
	 * @param appName
	 * @param appIcon
	 * @param sideNavUrl
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public int update(final String appName, final String sideNavUrl ,final int appId)throws SQLException{
		StringBuffer sqlInsert = new StringBuffer();
		sqlInsert.append("update app_menu_item set app_name = ? , side_nav_url=? where app_id=?");
		OpUpdate op = new OpUpdate(sqlInsert.toString(),DataAccessMgr.BIZ_APP) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, appName);
				ps.setString(2, sideNavUrl);
				ps.setInt(3, appId);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	/**
	 * 
	 * @param appName
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public int update(final String appName,final int appId)throws SQLException{
		StringBuffer sqlInsert = new StringBuffer();
		sqlInsert.append("update app_menu_item set app_name = ? where app_id=?");
		OpUpdate op = new OpUpdate(sqlInsert.toString(),DataAccessMgr.BIZ_APP) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, appName);
				ps.setInt(2, appId);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	
	public AppMenuItem get(final int appId)throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append("select ").append(fields).append(" from app_menu_item where app_id =?");
		OpUniq op = new OpUniq(sb.toString(),DataAccessMgr.BIZ_APP) {
		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, appId);
			}
		
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseAppMenuItem(rs);
			}
		
		};
		return (AppMenuItem) DataAccessMgr.getInstance().queryUnique(op);
	}
	
	/**
	 * 根据appId从主库读取appmenuitem对象，大访问量需求慎用此方法
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public AppMenuItem getFromMaster(final int appId)throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append("select ").append(fields).append(" from app_menu_item where app_id =?");
		OpUniq op = new OpUniq(sb.toString(),DataAccessMgr.BIZ_APP) {
		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, appId);
			}
		
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseAppMenuItem(rs);
			}
		
		};
		return (AppMenuItem) DataAccessMgr.getInstance().queryUnique(op,true);
	}
	
	/**
	 * 
	 * @return
	 * @throws SQLException
	 */
	@SuppressWarnings("unchecked")
	public List<AppMenuItem> getAll()throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append("select ").append(fields).append(" from app_menu_item ");
		OpList op = new OpList(sb.toString(),DataAccessMgr.BIZ_APP) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
			}
		
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseAppMenuItem(rs);
			}
		
		};
		return (ArrayList<AppMenuItem>)DataAccessMgr.getInstance().queryList(op);
	}
	@SuppressWarnings("unchecked")
	public List<AppMenuItem> gets(String appIds)throws SQLException{
		StringBuffer sb = new StringBuffer();
		sb.append("select ").append(fields).append(" from app_menu_item where app_id in("+ appIds +") ");
		OpList op = new OpList(sb.toString(),DataAccessMgr.BIZ_APP) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
			}
		
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseAppMenuItem(rs);
			}
		
		};
		return (ArrayList<AppMenuItem>)DataAccessMgr.getInstance().queryList(op);
	}

	/**
	 * 增加一条记录
	 * @param menuItem
	 * @return
	 * @throws SQLException
	 */
	public int save(final AppMenuItem menuItem)throws SQLException{
		if(menuItem.getAppId()<=0){
			throw new SQLException("menuItem.appId is must");
		}
		StringBuffer sqlInsert = new StringBuffer();
		sqlInsert.append("insert into app_menu_item( ").append(fields).append(" )values(?,?,?,?,?,?)");
		OpUpdate op = new OpUpdate(sqlInsert.toString(),DataAccessMgr.BIZ_APP) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, menuItem.getAppId());
				ps.setString(2, menuItem.getAppName());
				ps.setString(3, menuItem.getAppIcon());
				ps.setString(4, menuItem.getAppIcon24());
				ps.setString(5, menuItem.getSideNavUrl());
                ps.setString(6, menuItem.getWapSideNavUrl());
			}
		};
		return DataAccessMgr.getInstance().insertReturnId(op);
	}
	
	/**
	 * 更新appIcon
	 * @param appName
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public int updateIcon(final String appIcon,final int appId)throws SQLException{
		StringBuffer sqlInsert = new StringBuffer();
		sqlInsert.append("update app_menu_item set app_icon = ? where app_id=?");
		OpUpdate op = new OpUpdate(sqlInsert.toString(),DataAccessMgr.BIZ_APP) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, appIcon);
				ps.setInt(2, appId);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}

	/**
	 * 更新appIcon 24尺寸的
	 * @param appName
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public int updateIcon24(final String appIcon24,final int appId)throws SQLException{
		StringBuffer sqlInsert = new StringBuffer();
		sqlInsert.append("update app_menu_item set app_icon_24 = ? where app_id=?");
		OpUpdate op = new OpUpdate(sqlInsert.toString(),DataAccessMgr.BIZ_APP) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, appIcon24);
				ps.setInt(2, appId);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	/**
	 * 启用新的接口，更新appIcon 和appIcon24
	 * @param appName
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public int updateIcon(final String appIcon,final String appIcon24,final int appId)throws SQLException{
		StringBuffer sqlInsert = new StringBuffer();
		sqlInsert.append("update app_menu_item set app_icon = ? , app_icon_24 = ? where app_id=?");
		OpUpdate op = new OpUpdate(sqlInsert.toString(),DataAccessMgr.BIZ_APP) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, appIcon);
				ps.setString(2, appIcon24);
				ps.setInt(3, appId);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	/**
	 * 删除一条记录
	 * @param appId
	 * @return
	 * @throws SQLException
	 */
	public int delete(final int appId)throws SQLException{
		StringBuffer sqlInsert = new StringBuffer();
		sqlInsert.append("delete from app_menu_item where app_id=?");
		OpUpdate op = new OpUpdate(sqlInsert.toString(),DataAccessMgr.BIZ_APP) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, appId);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	private AppMenuItem parseAppMenuItem(ResultSet rs) throws SQLException {
		AppMenuItem ab = new AppMenuItem();
		ab.setAppId(rs.getInt("app_id"));
		ab.setAppName(rs.getString("app_name"));
		ab.setAppIcon(rs.getString("app_icon"));
		ab.setAppIcon24(rs.getString("app_icon_24"));
		ab.setSideNavUrl(rs.getString("side_nav_url"));
		ab.setWapSideNavUrl(rs.getString("wap_side_nav_url"));
		return ab;
	}
	public int updateSideNavUrl(final String sideNavUrl, final int appId) throws SQLException {
        StringBuffer sqlInsert = new StringBuffer();
        sqlInsert.append("update app_menu_item set side_nav_url = ? where app_id=?");
        OpUpdate op = new OpUpdate(sqlInsert.toString(), DataAccessMgr.BIZ_APP) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, sideNavUrl);
                ps.setInt(2, appId);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
    public int updateWapSideNavUrl(final String wapSideNavUrl, final int appId) throws SQLException {
        StringBuffer sqlInsert = new StringBuffer();
        sqlInsert.append("update app_menu_item set wap_side_nav_url = ? where app_id=?");
        OpUpdate op = new OpUpdate(sqlInsert.toString(), DataAccessMgr.BIZ_APP) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, wapSideNavUrl);
                ps.setInt(2, appId);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
    /**
     * 同时更新 web 和 wap 的应用访问URL地址
     * @param sideNavUrl
     * @param wapSideNavUrl
     * @param appId
     * @return
     * @throws SQLException
     */
    public int updateWebWapSideNavUrl(final String sideNavUrl, final String wapSideNavUrl, final int appId) throws SQLException {
        StringBuffer sqlInsert = new StringBuffer();
        sqlInsert.append("update app_menu_item set side_nav_url = ? , wap_side_nav_url = ? where app_id=?");
        OpUpdate op = new OpUpdate(sqlInsert.toString(), DataAccessMgr.BIZ_APP) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, sideNavUrl);
                ps.setString(2, wapSideNavUrl);
                ps.setInt(3, appId);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
}

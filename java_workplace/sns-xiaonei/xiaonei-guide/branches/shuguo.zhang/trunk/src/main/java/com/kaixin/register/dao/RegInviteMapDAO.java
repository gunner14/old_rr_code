package com.kaixin.register.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.kaixin.register.model.RegInviteMap;
import com.kaixin.register.utils.RegUtil;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

public class RegInviteMapDAO extends XnDAO {

	private static RegInviteMapDAO instance = new RegInviteMapDAO();

	public static RegInviteMapDAO getInstance() {
		return instance;
	}

	private RegInviteMapDAO() {
		super();
	}

	private final static String exceptInviter = " and inviter_id <> "+RegUtil.exceptInviterId;
	/**
	 * 插入被邀请注册的人和邀请人的映射关系表
	 * @param regInviteMapTable
	 * @return
	 * @throws SQLException
	 */
	public int save(final RegInviteMap regInviteMapTable) throws SQLException {
		String sqlInsert = "insert into reg_invite_map (owner_id, inviter_id, inviter_group_name, time,app_id) values (?, ?, ?, now(), ?)";
		OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, regInviteMapTable.getOwnerId());
				ps.setInt(2, regInviteMapTable.getInviterId());
				ps.setString(3, regInviteMapTable.getInviterGroupName());
				ps.setInt(4, regInviteMapTable.getAppId());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	/**
	 * 如果是app，则把该appid更新到对应记录上
	 * @param regInviteMapTable
	 * @return
	 * @throws SQLException
	 */
	public int updateAppId(final RegInviteMap regInviteMapTable) throws SQLException {
		String sqlUpdate = "update reg_invite_map set app_id=? where owner_id=? and inviter_id=?";
		OpUpdate op = new OpUpdate(sqlUpdate, DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, regInviteMapTable.getAppId());
				ps.setInt(2, regInviteMapTable.getOwnerId());
				ps.setInt(3, regInviteMapTable.getInviterId());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	/**
	 * update reg invite map's params' value
	 * @param regInviteMapTable
	 * @return
	 * @throws SQLException
	 * @author: Chris Gao
	 */
	public int updateParams(final RegInviteMap regInviteMapTable) throws SQLException {
		String sqlUpdate = "update reg_invite_map set params=? where owner_id=? and inviter_id=?";
		OpUpdate op = new OpUpdate(sqlUpdate, DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, regInviteMapTable.getParams());
				ps.setInt(2, regInviteMapTable.getOwnerId());
				ps.setInt(3, regInviteMapTable.getInviterId());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	/**
	 * 获取用户的appid邀请人数目
	 * @param appid		app
	 * @param ownerId	用户的id
	 * @return
	 * @throws SQLException
	 */
	public int getUserAppCount(final int appid, final int ownerId) throws SQLException {
		String sqlGet = "select count(1) size from reg_invite_map where app_id=? and inviter_id=?";
		OpUniq op = new OpUniq(sqlGet,DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, appid);
				ps.setInt(2, ownerId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseResult(rs);
			}

		};
		return DataAccessMgr.getInstance().queryInt(op);
	}
	
	/**
	 * 获取用户的邀请人数目(invited and reged invitee count)
	 * @throws SQLException
	 */
	public int getUserInviteeCount(final int inviterId) throws SQLException {
		String sqlGet = "select count(1) size from reg_invite_map where inviter_id=?";
		OpUniq op = new OpUniq(sqlGet,DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviterId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseResult(rs);
			}

		};
		return DataAccessMgr.getInstance().queryInt(op);
	}
	
	/**
	 * 通过被邀请人的id获取邀请人的id
	 * @param ownerId
	 * @return
	 * @throws SQLException
	 */
	public int getInviterId(final int ownerId) throws SQLException {
		String sqlGet = "select inviter_id from reg_invite_map where owner_id=?"+exceptInviter;
		OpUniq op = new OpUniq(sqlGet,DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, ownerId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseResult(rs);
			}

		};
		return DataAccessMgr.getInstance().queryInt(op);
	}
	
	/**
	 * 通过被邀请人的id获取邀请人的id
	 * @param ownerId
	 * @return
	 * @throws SQLException
	 */
	public RegInviteMap getInviterIdByHost(final int ownerId) throws SQLException {
		String sqlGet = "select * from reg_invite_map where owner_id=?" + exceptInviter;
		OpUniq op = new OpUniq(sqlGet,DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, ownerId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseResult(rs);
			}

		};
		return (RegInviteMap)DataAccessMgr.getInstance().queryUnique(op);
	}
	
	/**
	 * 通过被邀请人的id获取邀请人的分组
	 * @param ownerId
	 * @return
	 * @throws SQLException
	 */
	public String getInviterGroupName(final int ownerId) throws SQLException {
		String sqlGet = "select inviter_group_name from reg_invite_map where owner_id=?"+exceptInviter;
		OpUniq op = new OpUniq(sqlGet,DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, ownerId);
			}

			@Override
			public String parse(ResultSet rs) throws SQLException {
				return rs.getString("inviter_group_name");
			}

		};
		return (String)DataAccessMgr.getInstance().queryUnique(op);
	}
	
	/**
	 * 根据邀请人得到被邀请人的list
	 * @param inviterId 邀请人
	 * @param appId		app
	 * @return
	 * @throws SQLException
	 */
	@SuppressWarnings("unchecked")
	public List<RegInviteMap> getByInviterId(final int inviterId, final int appId) throws SQLException {
		String sqlGetList = "select * from reg_invite_map where inviter_id=? and app_id=?";
		OpList op = new OpList(sqlGetList, DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviterId);
				ps.setInt(2, appId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseResult(rs);
			}

		};
		return (List<RegInviteMap>) DataAccessMgr.getInstance().queryList(op);
	}
	
	/**
	 * 根据被邀请注册的人得到记录信息
	 * @param ownerId 被邀请注册的人
	 * @return
	 * @throws SQLException
	 */
	public RegInviteMap getByOwnerId(final int ownerId) throws SQLException {
		String sqlGet = "select * from reg_invite_map where owner_id = ?"+exceptInviter;
		OpUniq op = new OpUniq(sqlGet,DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, ownerId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseResult(rs);
			}

		};
		return (RegInviteMap) DataAccessMgr.getInstance().queryUnique(op);
	}
	
	/**
	 * 根据邀请人得到被邀请人的list
	 * @param inviterId 邀请人
	 * @return
	 * @throws SQLException
	 */
	@SuppressWarnings("unchecked")
	public List<RegInviteMap> getByInviterId(final int inviterId) throws SQLException {
		String sqlGetList = "select * from reg_invite_map where inviter_id = ?";
		OpList op = new OpList(sqlGetList, DataAccessMgr.BIZ_KX_STAT) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, inviterId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseResult(rs);
			}

		};
		return (List<RegInviteMap>) DataAccessMgr.getInstance().queryList(op);
	}

	private RegInviteMap parseResult(ResultSet rs) throws SQLException {
		RegInviteMap regInviteMapTable = new RegInviteMap();
		regInviteMapTable.setId(rs.getInt("id"));
		regInviteMapTable.setOwnerId(rs.getInt("owner_id"));
		regInviteMapTable.setInviterId(rs.getInt("inviter_id"));
		regInviteMapTable.setTime(rs.getTimestamp("time"));
		regInviteMapTable.setInviterGroupName(rs.getString("inviter_group_name"));
		regInviteMapTable.setAppId(rs.getInt("app_id"));
		return regInviteMapTable;
	}

}

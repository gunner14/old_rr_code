package com.renren.sns.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.renren.sns.guide.view.UserTinyView;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.framework.dao.XnDAO;

public class StarDAO extends XnDAO {

	private static StarDAO instance = new StarDAO();

	public static StarDAO getInstance() {
		return instance;
	}

	private StarDAO() {
		super();
	}

	private final static String sqlGetByTop = "select id, name, tinyurl from pop_user where univ is null order by view_count desc limit ?, ?";

	private final static int fBegin = 0;

	private final static int fCount = 37;

	@SuppressWarnings( { "unchecked" })
	public List<UserTinyView> getByTop() throws SQLException {
		List<UserTinyView> list = null;
		MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
		if (mcm != null) {
			list = (List<UserTinyView>) mcm.get("star_toph_");
			if (list != null) {
				return list;
			}
		}

		OpList op = new OpList(sqlGetByTop, DataAccessMgr.BIZ_POP_USER) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, fBegin);
				ps.setInt(2, fCount);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseUserTinyView(rs);
			}
		};
		list = (ArrayList<UserTinyView>) DataAccessMgr.getInstance().queryList(
				op);
		if (mcm != null) {
			mcm.set("star_toph_", list, (int) MemCacheKeys.day * 3);
		}
		return list;
	}

	private final static String sqlGetByNetwork = "select id, name, tinyurl from pop_user where univ = ? order by view_count desc limit ?, ?";

	@SuppressWarnings( { "unchecked" })
	public List<UserTinyView> getByNetwork(final int networkId)
			throws SQLException {
		List<UserTinyView> list = null;
		MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
		if (mcm != null) {
			list = (List<UserTinyView>) mcm.get("star_network_h_" + networkId);
			if (list != null) {
				return list;
			}
		}

		OpList op = new OpList(sqlGetByNetwork, DataAccessMgr.BIZ_POP_USER) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, networkId);
				ps.setInt(2, fBegin);
				ps.setInt(3, fCount);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseUserTinyView(rs);
			}

		};
		list = (ArrayList<UserTinyView>) DataAccessMgr.getInstance().queryList(op);
		if (mcm != null) {
			mcm.set("star_network_h_" + networkId, list,(int) MemCacheKeys.day * 3);
		}
		return list;
	}

	private final static String sqlGetByWorkplaceName = "select userid, user_name, tinyurl from workplace_view_count where workplace_name = ? order by view_count desc limit ?, ?";

	@SuppressWarnings( { "unchecked" })
	public List<UserTinyView> getByWorkplaceName(final String workplaceName)
			throws SQLException {
		List<UserTinyView> list = null;
		MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
		String keyWorkplaceName = "";
		if (mcm != null) {
			if (workplaceName != null) {
				keyWorkplaceName = workplaceName.replaceAll(" ", "").replaceAll("　", "");
			}
			list = (List<UserTinyView>) mcm.get("star_network_h_" + keyWorkplaceName);
			if (list != null) {
				return list;
			}
		}
		OpList op = new OpList(sqlGetByWorkplaceName,DataAccessMgr.BIZ_POP_USER) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, workplaceName);
				ps.setInt(2, fBegin);
				ps.setInt(3, fCount);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseUserTinyViewForWorkspace(rs);
			}

		};
		list = (ArrayList<UserTinyView>) DataAccessMgr.getInstance().queryList(op);
		if (mcm != null) {
			mcm.set("star_network_h_" + keyWorkplaceName, list,
					(int) MemCacheKeys.day * 3);
		}
		return list;
	}

	private final static String sqlGetByStage = "select id, name, tinyurl from pop_user where stage = ? order by view_count desc limit ?, ?";

	@SuppressWarnings("unchecked")
	public List<UserTinyView> getByStage(final int stage) throws SQLException {
		OpList op = new OpList(sqlGetByStage, DataAccessMgr.BIZ_POP_USER) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, stage);
				ps.setInt(2, fBegin);
				ps.setInt(3, fCount);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseUserTinyView(rs);
			}

		};
		return (ArrayList<UserTinyView>) DataAccessMgr.getInstance().queryList(op);
	}

	public static UserTinyView parseUserTinyView(ResultSet rs)
			throws SQLException {
		UserTinyView utv = new UserTinyView();
		utv.setId(rs.getInt("id"));
		utv.setName(rs.getString("name"));
		utv.setTinyUrl(rs.getString("tinyurl"));
		return utv;
	}

	public static UserTinyView parseUserTinyViewForWorkspace(ResultSet rs)
			throws SQLException {
		UserTinyView utv = new UserTinyView();
		utv.setId(rs.getInt("userid"));
		utv.setName(rs.getString("user_name"));
		utv.setTinyUrl(rs.getString("tinyurl"));
		return utv;
	}
}

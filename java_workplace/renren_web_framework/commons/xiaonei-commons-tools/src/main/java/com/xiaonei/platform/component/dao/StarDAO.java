package com.xiaonei.platform.component.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Random;

import com.xiaonei.platform.component.view.UserTinyView;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

public class StarDAO extends XnDAO {

	private static StarDAO instance = new StarDAO();

	public static StarDAO getInstance() {
		return instance;
	}

	private StarDAO() {
		super();
	}

	private final static String sqlGetCountByTop = "select count(1) from pop_user where univ is null";

	private final static String sqlGetCountByNetwork = "select count(1) from pop_user where univ = ";

	private final static String sqlGetCountByStage = "select count(1) from pop_user where stage = ";

	private int getCount(String sql) throws SQLException {
		OpUniq op = new OpUniq(sql, DataAccessMgr.BIZ_POP_USER);
		return DataAccessMgr.getInstance().queryInt(op);
	}

	private final static String sqlGetByTop = "select id, name, tinyurl from pop_user where univ is null order by view_count desc limit ?, ?";

	public List<UserTinyView> getByTop(final int starNumber) throws SQLException {
		List<UserTinyView> list = null;
		MemCacheManager mcm = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
		if (mcm != null) {
			list = (List<UserTinyView>) mcm.get("star_toph_xw");
			if (list != null) {
				return list;
			}
		}
		int count = getCount(sqlGetCountByTop);
		int begin = 0;
		if (count > starNumber) {
			begin = new Random().nextInt(count - starNumber);
		}
		final int fBegin = begin;
		OpList op = new OpList(sqlGetByTop, DataAccessMgr.BIZ_POP_USER) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, fBegin);
				ps.setInt(2, starNumber);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseUserTinyView(rs);
			}

		};
		list = (ArrayList<UserTinyView>) DataAccessMgr.getInstance().queryList(op);
		if (mcm != null) {
			mcm.set("star_toph_xw", list, new Date(MemCacheKeys.hour * 12));
		}
		return list;
	}

	private final static String sqlGetByNetwork = "select id, name, tinyurl from pop_user where univ = ? order by view_count desc limit ?, ?";

	public List<UserTinyView> getByNetwork(final int networkId,final int starNumber)
			throws SQLException {
		List<UserTinyView> list = null;
		MemCacheManager mcm = MemCacheFactory.getInstance()
				.getManagerByPoolName(MemCacheKeys.pool_default);
		if (mcm != null) {
			list = (List<UserTinyView>) mcm.get("star_network_xw" + networkId);
			if (list != null) {
				return list;
			}
		}
		int count = getCount(sqlGetCountByNetwork + networkId);
		int begin = 0;
		if (count > starNumber) {
			begin = new Random().nextInt(count - starNumber);
		}
		final int fBegin = begin;
		OpList op = new OpList(sqlGetByNetwork, DataAccessMgr.BIZ_POP_USER) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, networkId);
				ps.setInt(2, fBegin);
				ps.setInt(3, starNumber);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseUserTinyView(rs);
			}

		};
		list = (ArrayList<UserTinyView>) DataAccessMgr.getInstance().queryList(op);
		if (mcm != null) {
			mcm.set("star_network_xw" + networkId, list, new Date(MemCacheKeys.hour * 12));
		}
		return list;
	}

	private final static String sqlGetByStage = "select id, name, tinyurl from pop_user where stage = ? order by view_count desc limit ?, ?";

	public List<UserTinyView> getByStage(final int stage,final int starNumber) throws SQLException {
		int count = getCount(sqlGetCountByStage + stage);
		int begin = 0;
		if (count > starNumber) {
			begin = new Random().nextInt(count - starNumber);
		}
		final int fBegin = begin;
		OpList op = new OpList(sqlGetByStage, DataAccessMgr.BIZ_POP_USER) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, stage);
				ps.setInt(2, fBegin);
				ps.setInt(3, starNumber);
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

}

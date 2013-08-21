package com.xiaonei.platform.taglib.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.platform.taglib.model.TaglibTemplet;
import com.xiaonei.xce.XceAdapter;

/**
 * 
 * @author lifs
 * 
 */
public class TaglibTempletDAO  extends XnDAO{

	private static TaglibTempletDAO instance = new TaglibTempletDAO();

	public static TaglibTempletDAO getInstance() {
		if (instance == null) {
			synchronized (TaglibTempletDAO.class) {
				if (instance == null)
					instance = new TaglibTempletDAO();
			}
		}
		return instance;
	}

	private TaglibTempletDAO() {
		super();
	}

	private static String sql = " templet,tag_id,description,cutline,alias ";

	private static String view = "select id," + sql;

	public int save(final TaglibTemplet ab) throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append("insert into taglib_templet");
		sb.append(" (" + sql + ") ");
		sb.append("values (?,?,?,?,?)");
		OpUpdate op = new OpUpdate(sb.toString(), DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, ab.getTemplet());
				ps.setInt(2 ,ab.getTagId());
				ps.setString(3, ab.getDescription());
				ps.setString(4, ab.getCutline());
				ps.setString(5, ab.getAlias());
			}
		};
		return DataAccessMgr.getInstance().insertReturnId(op);
	}

	public int updateTemplet(final TaglibTemplet ab) throws SQLException {

		StringBuffer sb = new StringBuffer();
		sb.append("update taglib_templet ");
		sb.append(" set templet=?,description=?,cutline=?,alias=? ");
		sb.append("where id = ?");
		OpUpdate op = new OpUpdate(sb.toString(), DataAccessMgr.BIZ_COMMON) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, ab.getTemplet());
				ps.setString(2, ab.getDescription());
				ps.setString(3, ab.getCutline());
				ps.setString(4, ab.getAlias());
				ps.setInt(5, ab.getId());
			}

		};
		return DataAccessMgr.getInstance().update(op);
	}

	public TaglibTemplet get(final int id) throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append(view);
		sb.append("from taglib_templet ");
		sb.append(" where id = ? ");
		OpUniq op = new OpUniq(sb.toString(), DataAccessMgr.BIZ_COMMON) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseTaglibTemplet(rs);
			}

		};
		return (TaglibTemplet) DataAccessMgr.getInstance().queryUnique(op);
	}
	public TaglibTemplet get(final String alias) throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append(view);
		sb.append("from taglib_templet ");
		sb.append(" where alias = ? ");
		OpUniq op = new OpUniq(sb.toString(), DataAccessMgr.BIZ_COMMON) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, alias);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseTaglibTemplet(rs);
			}

		};
		return (TaglibTemplet) DataAccessMgr.getInstance().queryUnique(op);
	}

	public List<TaglibTemplet> gets() throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append(view + " from taglib_templet ");
		OpList op = new OpList(sb.toString(), DataAccessMgr.BIZ_COMMON) {

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseTaglibTemplet(rs);
			}

		};
		return (ArrayList<TaglibTemplet>) DataAccessMgr.getInstance()
				.queryList(op);
	}

	public List<TaglibTemplet> gets(final int tagId) throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append(view + " from taglib_templet where tag_id=?");
		OpList op = new OpList(sb.toString(), DataAccessMgr.BIZ_COMMON) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, tagId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseTaglibTemplet(rs);
			}

		};
		return (ArrayList<TaglibTemplet>) DataAccessMgr.getInstance()
				.queryList(op);
	}
	private final TaglibTemplet parseTaglibTemplet(ResultSet rs)
			throws SQLException {
		TaglibTemplet tt = new TaglibTemplet();
		tt.setId(rs.getInt("id"));
		tt.setTemplet(rs.getString("templet"));
		tt.setDescription(rs.getString("description"));
		tt.setCutline(rs.getString("cutline"));
		tt.setAlias(rs.getString("alias"));
		return tt;
	}

}

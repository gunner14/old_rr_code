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
import com.xiaonei.platform.taglib.model.TaglibInfo;
import com.xiaonei.xce.XceAdapter;

/**
 * 操作表taglib_info的DAO对象
 * @author lifs
 * 
 */
public class TaglibInfoDAO extends XnDAO{

	private static TaglibInfoDAO instance = new TaglibInfoDAO();

	public static TaglibInfoDAO getInstance() {
		if (instance == null) {
			synchronized (TaglibInfoDAO.class) {
				if (instance == null)
					instance = new TaglibInfoDAO();
			}
		}
		return instance;
	}

	private TaglibInfoDAO() {
		super();
	}

	private static String sql = " name ";

	private static String view = "select id," + sql;
	/**
	 * 保存，taglib_info表增加一条记录
	 * @param ti
	 * @return
	 * @throws SQLException
	 */
	public int save(final TaglibInfo ti) throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append("insert into taglib_info");
		sb.append(" (" + sql + ") ");
		sb.append("values (?)");
		OpUpdate op = new OpUpdate(sb.toString(), DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, ti.getName());
			}
		};
		return DataAccessMgr.getInstance().insertReturnId(op);
	}
	
	public int updateInfo(final TaglibInfo ab) throws SQLException {

		StringBuffer sb = new StringBuffer();
		sb.append("update taglib_info ");
		sb.append(" set name=? ");
		sb.append("where id = ?");
		OpUpdate op = new OpUpdate(sb.toString(), DataAccessMgr.BIZ_COMMON) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, ab.getName());
				ps.setInt(2, ab.getId());
			}

		};
		return DataAccessMgr.getInstance().update(op);
	}

	public TaglibInfo get(final int id) throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append(view);
		sb.append("from taglib_info ");
		sb.append(" where id = ? ");
		OpUniq op = new OpUniq(sb.toString(), DataAccessMgr.BIZ_COMMON) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseTaglibInfo(rs);
			}

		};
		return (TaglibInfo) DataAccessMgr.getInstance().queryUnique(op);
	}
	public TaglibInfo get(final String name) throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append(view);
		sb.append("from taglib_info ");
		sb.append(" where id = ? ");
		OpUniq op = new OpUniq(sb.toString(), DataAccessMgr.BIZ_COMMON) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, name);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseTaglibInfo(rs);
			}

		};
		return (TaglibInfo) DataAccessMgr.getInstance().queryUnique(op);
	}

	public List<TaglibInfo> gets() throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append(view + " from taglib_info ");
		OpList op = new OpList(sb.toString(), DataAccessMgr.BIZ_COMMON) {

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseTaglibInfo(rs);
			}

		};
		return (ArrayList<TaglibInfo>) DataAccessMgr.getInstance()
				.queryList(op);
	}

	private final TaglibInfo parseTaglibInfo(ResultSet rs)
			throws SQLException {
		TaglibInfo tt = new TaglibInfo();
		tt.setId(rs.getInt("id"));
		tt.setName(rs.getString("name"));
		return tt;
	}

}

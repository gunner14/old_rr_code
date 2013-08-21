package com.xiaonei.platform.core.opt.jdbc.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;

public class WorkplaceDAO {

	private static WorkplaceDAO instance = new WorkplaceDAO();

	public static WorkplaceDAO getInstance() {
		if (instance == null) {
			synchronized (WorkplaceDAO.class) {
				if (instance == null) {
                    instance = new WorkplaceDAO();
                }
			}
		}
		return instance;
	}

	private WorkplaceDAO() {
		super();
	}
	
	protected static final String sqlGet = "select id, name, email, count_a, tribe, building from workplace where id = ?";

	public Workplace get(final int id) throws SQLException {
		OpUniq op = new OpUniq(sqlGet, DataAccessMgr.BIZ_COMMON) {
			@Override
            public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}

			@Override
            public Object parse(ResultSet rs) throws SQLException {
				return parseWorkplace(rs);
			}
		};
		return (Workplace) DataAccessMgr.getInstance().queryUnique(op);
	}
	
	private static final String sqlGetWorkplace = " select id, name, email, count_a ,tribe, building from workplace ";
	@SuppressWarnings("unchecked")
    public List<Workplace> getWorkplaceList() throws SQLException {
		OpList op = new OpList(sqlGetWorkplace, DataAccessMgr.BIZ_COMMON) {		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseWorkplace(rs);
			}
		
		};
		return DataAccessMgr.getInstance().queryList(op);
	}	
	private static final String sqlGetWorkplaceByAlpha="select id,name,email,count_a,tribe,building from workplace where alpha=?";
	/**
	 * get workplace list sort by alpha
	 * @author zhangjie
	 * @param alpha
	 * @return
	 * @throws SQLException
	 */
	@SuppressWarnings("unchecked")
    public List<Workplace> getWorkplaceListByAlpha(final String alpha) throws SQLException{
		OpList op = new OpList(sqlGetWorkplaceByAlpha, DataAccessMgr.BIZ_COMMON){
			@Override
            public void setParam(PreparedStatement ps) throws SQLException{
				ps.setString(1, alpha);
			}
			@Override
            public Object parse(ResultSet rs) throws SQLException{
				return parseWorkplace(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	private final static String sqlIncStuCount = "update workplace set count_a = count_a + 1 where id = ?";
	public int incStuCount(final int id) throws SQLException {
		OpUpdate op = new OpUpdate(sqlIncStuCount, DataAccessMgr.BIZ_COMMON) {

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}

		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	protected static Workplace parseWorkplace(ResultSet rs) throws SQLException {
		Workplace w = new Workplace();
		w.setId(rs.getInt("id"));
		w.setName(rs.getString("name"));
		w.setEmail(rs.getString("email"));
		w.setCountAll(rs.getInt("count_a"));
		w.setTribeId(rs.getInt("tribe")) ;
		w.setBuilding(rs.getString("building"));
		return w;
	}

	/**
	 * 公司的team数+1
	 * @param companyId
	 * @return
	 * @throws SQLException
	 */
	public int incCount(final int companyId) throws SQLException {
		String sql = "update workplace set team_count= team_count+1 where id = ? ";
		OpUpdate op = new OpUpdate(sql, DataAccessMgr.BIZ_COMMON) {		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {				
				ps.setInt(1, companyId);
			}		
		};
		return DataAccessMgr.getInstance().update(op);
	}
	/**
	 * 公司的team数-1
	 * @param companyId
	 * @return
	 * @throws SQLException
	 */
	public int decCount(final int companyId) throws SQLException {
		String sql = "update workplace set team_count= team_count-1 where id = ? ";
		OpUpdate op = new OpUpdate(sql, DataAccessMgr.BIZ_COMMON) {		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {				
				ps.setInt(1, companyId);
			}		
		};
		return DataAccessMgr.getInstance().update(op);
	}	
	
}

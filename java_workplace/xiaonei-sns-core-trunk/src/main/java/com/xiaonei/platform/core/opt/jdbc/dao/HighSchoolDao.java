package com.xiaonei.platform.core.opt.jdbc.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.model.HighSchool;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;

public class HighSchoolDao {
	private static HighSchoolDao instance = new HighSchoolDao();

	public static HighSchoolDao getInstance() {
		
		return instance;
	}
	
	private final static String highSchoolView = "select id,high_school.name,high_school.tribe,code_pro,pro.name name_pro,code_city," +
		"city.name name_city,code_county,county.name name_county,high_school.code ";
	private final static String sqlGetSystemHighSchoolList = highSchoolView +
			"from high_school left join china_area pro on high_school.code_pro = pro.code left join china_area city on high_school.code_city = city.code " +
			"left join china_area county on high_school.code_county = county.code " +
		    "where flag = 0 and high_school.name like ? limit ?,?";
	
	private final static String sqlGetSystemHighSchoolListByProCode = highSchoolView +
			"from high_school left join china_area pro on high_school.code_pro = pro.code left join china_area city on high_school.code_city = city.code " +
			"left join china_area county on high_school.code_county = county.code " +
			"where flag = 0 and code_pro = ? and high_school.name like ? limit ?,?";
	
	private final static String sqlGetSystemHighSchoolListByCityCode = highSchoolView +
			"from high_school left join china_area pro on high_school.code_pro = pro.code left join china_area city on high_school.code_city = city.code " +
			"left join china_area county on high_school.code_county = county.code " +
			"where flag = 0 and code_city = ? and high_school.name like ? limit ?,?";
	
	private final static String sqlGetSystemHighSchoolListByAreaCode = highSchoolView +
			"from high_school left join china_area pro on high_school.code_pro = pro.code left join china_area city on high_school.code_city = city.code " +
			"left join china_area county on high_school.code_county = county.code " +
			"where flag = 0 and code_county = ? and high_school.name like ? limit ?,?";
	
	private final static String sqlGetCustomHighSchoolList = "select id,name,code_pro,code_city,tribe from high_school where flag = 1 and name like ? order by student_count desc limit ?,?";
	
	private final static String sqlGetCount = "select count(id) from high_school where flag = ? and name like ?";
	
	private final static String sqlGetCountByProCode = "select count(id) from high_school where code_pro = ? and flag = ? and name like ?";
	
	private final static String sqlGetCountByCityCode = "select count(id) from high_school where code_city = ? and flag = ? and name like ?";
	
	private final static String sqlGetCountByAreaCode = "select count(id) from high_school where code = ? and flag = ? and name like ?";
	
	private final static String sqlgetByid = "select id,name,code,code_pro,code_city,code_county,student_count, tribe from high_school where id = ?";
	
	private final static String sqlgetByName = "select id, name from high_school where name = ?";
	
	private final static String sqlInsert = "insert into high_school (name,code,code_pro,code_city,code_county) values (?, ?, ?, ?, ?)";
	
	private final static String sqlUpdate = "update high_school set name = ?,code = ?,code_pro = ?,code_city = ?,code_county = ? where id = ?";
	
	private final static String sqlUpdateCustomHighSchool = "update high_school set flag = ?,code = ?,code_pro = ?,code_city = ?,code_county = ? where id = ?";
	
	private final static String sqlDelete = "delete from high_school where id = ?";
	private final static String sqlGetAll="select * from high_school ;" ;
	
	public HighSchoolDao(){
		super();
	}
	@SuppressWarnings("unchecked")
    public List<HighSchool> getAllHighSchool()throws SQLException{
		OpList op = new OpList(sqlGetAll, DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseCustomHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	@SuppressWarnings("unchecked")
	public List<HighSchool> findSystemHighSchoolList(final String key,final int offset, final int limit) throws SQLException{
		OpList op = new OpList(sqlGetSystemHighSchoolList, DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, "%" + key + "%");
				ps.setInt(2, offset);
				ps.setInt(3, limit);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseSystemHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	@SuppressWarnings("unchecked")
	public List<HighSchool> findSystemHighSchoolListByName(final String name,final int offset, final int limit) throws SQLException{
		String sqlGetSystemHighSchoolListByName = highSchoolView +
		"from high_school left join china_area pro on high_school.code_pro = pro.code left join china_area city on high_school.code_city = city.code " +
		"left join china_area county on high_school.code_county = county.code " +
	    "where flag = 0 and high_school.name in (" + name + ") limit ?,?";
		OpList op = new OpList(sqlGetSystemHighSchoolListByName, DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				//ps.setString(1, name);
				ps.setInt(1, offset);
				ps.setInt(2, limit);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseSystemHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	
	@SuppressWarnings("unchecked")
    public List<HighSchool> findSystemHighSchoolListByProCode(final String code,final String key,final int offset, final int limit) throws SQLException{
		OpList op = new OpList(sqlGetSystemHighSchoolListByProCode, DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, code);
				ps.setString(2, "%" + key + "%");
				ps.setInt(3, offset);
				ps.setInt(4, limit);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseSystemHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	
	@SuppressWarnings("unchecked")
    public List<HighSchool> findSystemHighSchoolListByCityCode(final String code,final String key,final int offset, final int limit) throws SQLException{
		OpList op = new OpList(sqlGetSystemHighSchoolListByCityCode, DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, code);
				ps.setString(2, "%" + key + "%");
				ps.setInt(3, offset);
				ps.setInt(4, limit);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseSystemHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	
	@SuppressWarnings("unchecked")
    public List<HighSchool> findSystemHighSchoolListByAreaCode(final String code,final String key,final int offset, final int limit) throws SQLException{
		OpList op = new OpList(sqlGetSystemHighSchoolListByAreaCode, DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, code);
				ps.setString(2, "%" + key + "%");
				ps.setInt(3, offset);
				ps.setInt(4, limit);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseSystemHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	
	@SuppressWarnings("unchecked")
    public List<HighSchool> findCustomHighSchoolList(final String key,final int offset, final int limit) throws SQLException{
		OpList op = new OpList(sqlGetCustomHighSchoolList, DataAccessMgr.BIZ_COMMON) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, "%" + key + "%");
				ps.setInt(2, offset);
				ps.setInt(3, limit);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseCustomHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	
	private static final String sqlGetOpendHighSchoolListByPro = highSchoolView + 
									"from high_school left join china_area pro on high_school.code_pro = pro.code " +
									"left join china_area city on high_school.code_city = city.code " +
									"left join china_area county on high_school.code_county = county.code " +
									"where flag = 0 and pro.code = ? and high_school.tribe is not null order by high_school.code";

	@SuppressWarnings("unchecked")
    public List<HighSchool> getOpendHighSchoolListByPro(final String pro) throws SQLException{
		OpList op = new OpList(sqlGetOpendHighSchoolListByPro, DataAccessMgr.BIZ_COMMON) {
			
			@Override
            public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, pro);
			}


			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseCustomHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	
	private static final String sqlGetOpendHighSchoolListByCounty = "select id,name,code_pro,code_city,tribe from high_school where code_county = ? and flag = 0 and tribe is not null";
	
	@SuppressWarnings("unchecked")
    public List<HighSchool> getOpendHighSchoolListByCounty(final String county) throws SQLException{
		OpList op = new OpList(sqlGetOpendHighSchoolListByCounty, DataAccessMgr.BIZ_COMMON) {
			
			@Override
            public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, county);
			}


			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseCustomHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	/**
	 * 上面那个方法不能用，自己写了个
	 * @author zhangjie
	 * @param pro
	 * @return
	 * @throws SQLException
	 */
	@SuppressWarnings("unchecked")
    public List<HighSchool> getHighSchoolListByProCode(final String pro) throws SQLException{
		OpList op = new OpList("select * from high_school where code_pro = ? and flag = 0", DataAccessMgr.BIZ_COMMON){
			@Override
            public void setParam(PreparedStatement ps) throws SQLException{
				ps.setString(1, pro);
			}
			@Override
            public Object parse(ResultSet rs) throws SQLException{
				return parseCustomHighSchoolView(rs);
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	
	public int getCount(final int flag,final String key) throws SQLException{
		OpUniq op = new OpUniq(sqlGetCount,DataAccessMgr.BIZ_COMMON) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, flag);
				ps.setString(2, "%" + key + "%");
			}
		};
		return DataAccessMgr.getInstance().queryInt(op);
	}
	
	public int getCountByProCode(final String code,final int flag,final String key) throws SQLException{
		OpUniq op = new OpUniq(sqlGetCountByProCode, DataAccessMgr.BIZ_COMMON) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, code);
				ps.setInt(2, flag);
				ps.setString(3, "%" + key + "%");
			}
		};
		return DataAccessMgr.getInstance().queryInt(op);
	}
	
	public int getCountByCityCode(final String code,final int flag,final String key) throws SQLException{
		OpUniq op = new OpUniq(sqlGetCountByCityCode, DataAccessMgr.BIZ_COMMON) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, code);
				ps.setInt(2, flag);
				ps.setString(3, "%" + key + "%");
			}
		};
		return DataAccessMgr.getInstance().queryInt(op);
	}
	
	public int getCountByAreaCode(final String code,final int flag,final String key) throws SQLException{
		OpUniq op = new OpUniq(sqlGetCountByAreaCode, DataAccessMgr.BIZ_COMMON) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, code);
				ps.setInt(2, flag);
				ps.setString(3, "%" + key + "%");
			}
		};
		return DataAccessMgr.getInstance().queryInt(op);
	}
	
	public HighSchool get(final int id) throws SQLException{
		OpUniq op = new OpUniq(sqlgetByid, DataAccessMgr.BIZ_COMMON) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1,id);
			}
			
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				HighSchool highSchool = new HighSchool();
				highSchool.setId(rs.getInt("id"));
				highSchool.setName(rs.getString("name"));
				highSchool.setCode(rs.getString("code"));
				highSchool.setCode_pro(rs.getString("code_pro"));
				highSchool.setCode_city(rs.getString("code_city"));
				highSchool.setCode_county(rs.getString("code_county"));
				highSchool.setStudent_count(rs.getString("student_count"));
				highSchool.setTribeId(rs.getInt("tribe"));
				return highSchool;
			}
		};
		return (HighSchool) DataAccessMgr.getInstance().queryUnique(op);
	}

	public HighSchool getByName(final String name) throws SQLException{
		OpUniq op = new OpUniq(sqlgetByName, DataAccessMgr.BIZ_COMMON) {
			
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, name);
			}
			
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				HighSchool highSchool = new HighSchool();
				highSchool.setId(rs.getInt("id"));
				highSchool.setName(rs.getString("name"));
				return highSchool;
			}
		};
		return (HighSchool) DataAccessMgr.getInstance().queryUnique(op);
	}
	
	private HighSchool parseSystemHighSchoolView(ResultSet rs) throws SQLException {
		HighSchool highSchool = new HighSchool();
		highSchool.setId(rs.getInt("id"));
		highSchool.setName(rs.getString("name"));
		highSchool.setCode(rs.getString("code"));
		highSchool.setCode_pro(rs.getString("code_pro"));
		highSchool.setName_pro(rs.getString("name_pro"));
		highSchool.setCode_city(rs.getString("code_city"));
		highSchool.setName_city(rs.getString("name_city"));
		highSchool.setCode_county(rs.getString("code_county"));
		highSchool.setName_county(rs.getString("name_county"));
		highSchool.setTribeId(rs.getInt("tribe"));
		return highSchool;
	}
	
	private HighSchool parseCustomHighSchoolView(ResultSet rs) throws SQLException {
		HighSchool highSchool = new HighSchool();
		highSchool.setId(rs.getInt("id"));
		highSchool.setName(rs.getString("name"));
		highSchool.setCode_pro(rs.getString("code_pro"));
		highSchool.setCode_city(rs.getString("code_city"));
		highSchool.setTribeId(rs.getInt("tribe"));
		return highSchool;
	}
	
	public int save(final HighSchool highSchool) throws SQLException{
		OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_COMMON){
			@Override
            public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, highSchool.getName());
				ps.setString(2, highSchool.getCode());
				ps.setString(3, highSchool.getCode_pro());
				ps.setString(4, highSchool.getCode_city());
				ps.setString(5, highSchool.getCode_county());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	public int update(final HighSchool highSchool) throws SQLException{
		OpUpdate op = new OpUpdate(sqlUpdate, DataAccessMgr.BIZ_COMMON){
			@Override
            public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, highSchool.getName());
				ps.setString(2, highSchool.getCode());
				ps.setString(3, highSchool.getCode_pro());
				ps.setString(4, highSchool.getCode_city());
				ps.setString(5, highSchool.getCode_county());
				ps.setInt(6,highSchool.getId());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	private final static String sqlIncStuCount = "update high_school set student_count = student_count + 1 where id = ?";
	public int incStuCount(final int id) throws SQLException {
		OpUpdate op = new OpUpdate(sqlIncStuCount, DataAccessMgr.BIZ_COMMON) {
		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}
		
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	public int updateCustomHighSchool(final HighSchool highSchool) throws SQLException{
		OpUpdate op = new OpUpdate(sqlUpdateCustomHighSchool, DataAccessMgr.BIZ_COMMON){
			@Override
            public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, highSchool.getFlag());
				ps.setString(2, highSchool.getCode());
				ps.setString(3, highSchool.getCode_pro());
				ps.setString(4, highSchool.getCode_city());
				ps.setString(5, highSchool.getCode_county());
				ps.setInt(6,highSchool.getId());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	public int delete(final int id) throws SQLException{
		OpUpdate op = new OpUpdate(sqlDelete, DataAccessMgr.BIZ_COMMON){
			@Override
            public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1,id);
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
}

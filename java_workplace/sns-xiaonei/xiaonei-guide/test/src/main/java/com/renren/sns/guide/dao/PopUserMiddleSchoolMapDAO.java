package com.renren.sns.guide.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.renren.sns.guide.model.GuideCity;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.xce.XceAdapter;

public class PopUserMiddleSchoolMapDAO {

	private PopUserMiddleSchoolMapDAO() {
	}

	private static PopUserMiddleSchoolMapDAO instance = null;

	public static synchronized PopUserMiddleSchoolMapDAO getInstance() {
		if (instance == null) {
			instance = new PopUserMiddleSchoolMapDAO();
		}
		return instance;
	}

	public GuideCity getCityCodeByMiddleSchoolId(final int middleSchoolId)
			throws SQLException {
		StringBuffer sb = new StringBuffer();
		sb.append("select city_code,city_name from pop_user_middle_school_map where middle_school_id = ? limit 1");
		OpUniq op = new OpUniq(sb.toString(), XceAdapter.DB_POP_USER) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, middleSchoolId);
			}

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				return parseGuideCity(rs);
			}
		};
		return (GuideCity) DataAccessMgr.getInstance().queryUnique(op);
	}

	public static GuideCity parseGuideCity(ResultSet rs)
			throws SQLException {
		GuideCity gc = new GuideCity();
		gc.setCityCode(rs.getString("city_code"));
		gc.setCityName(rs.getString("city_name"));
		return gc;
	}
}

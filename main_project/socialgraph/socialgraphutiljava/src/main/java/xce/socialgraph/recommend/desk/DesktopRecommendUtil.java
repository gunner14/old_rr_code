package xce.socialgraph.recommend.desk;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.xce.XceAdapter;

/**
 * Get Desk recommendation from db
 * @author zhangnan
 *
 */
public class DesktopRecommendUtil {
	private static DesktopRecommendUtil instance = null;
	private static String dbSourceName = "socialgraph_recommend_app";
	
	private DesktopRecommendUtil() {
	}

	public static DesktopRecommendUtil getInstance() {
		if (null == instance) {
			synchronized (DesktopRecommendUtil.class) {
				if (null == instance) {
					try {
						instance = new DesktopRecommendUtil();
					} catch (Exception e) {
						e.printStackTrace();
						instance = null;
					}
				}
			}
		}
		return instance;
	}
	
	/**
	 * 根据用户提供userId获取为用户推荐的renren-desktop
	 * @param userId
	 * @return
	 */
	public List<Integer> getDeskRecommendList(int userId) {
		List<Integer> deskList = new ArrayList<Integer>();
		String tableName = "desktop_spread";
		Connection deskRecommendConn = null;
		Statement stmt = null;
		ResultSet result = null;
		try {
			deskRecommendConn = XceAdapter.getInstance().getReadConnection(dbSourceName);
			stmt = deskRecommendConn.createStatement();
			String sql = "";
			sql = "SELECT appid FROM " + tableName + " WHERE userid = " + userId;
			result = stmt.executeQuery(sql);
			if (null == result) {
				return deskList;
			}
			while (result.next()) {
				deskList.add(result.getInt("appid"));
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (null != deskRecommendConn) 		//clean
					deskRecommendConn.close();
				if (null != stmt)
					stmt.close();
				if (null != result)
					result.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		return deskList;
	}
	
	public static void main(String[] args) {
		List<Integer> result = DesktopRecommendUtil.getInstance().getDeskRecommendList(238111132);
		System.out.println("result size:" + result.size());
		
		for (Integer id : result) {
			System.out.println(id);
		}
	}
}

package xce.socialgraph.recommend.forum;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.xce.XceAdapter;

/**
 * 获取小组推荐数据类
 * @author zhangnan
 */
public class ForumRecommendUtil {
	private static ForumRecommendUtil instance = null;
	private static String dbSourceName = "socialgraph_recommend_app";
	
	private ForumRecommendUtil() {
	}
	
	public static ForumRecommendUtil getInstance() {
		if (null == instance) {
			synchronized (ForumRecommendUtil.class) {
				try {
					instance = new ForumRecommendUtil();
				} catch (Exception e) {
					e.printStackTrace();
					instance = null;
				}
			}
		}
		return instance;
	}
	
	/**
	 * 根据用户提供userId获取为用户推荐的小组列表
	 * @param userId
	 * @return
	 */
	public List<ForumData> getForumRecommendList(int userId) {
		List<ForumData> appList = new ArrayList<ForumData>();
		String tableName = "recommend_by_forum";
		Connection appRecommendConn = null;
		Statement stmt = null;
		ResultSet result = null;
		try {
			appRecommendConn = XceAdapter.getInstance().getReadConnection(dbSourceName);
			stmt = appRecommendConn.createStatement();
			String sql = "";
			sql = "SELECT userid, appid, sharescount, shares FROM " + tableName + " WHERE userid = " + userId;
			result = stmt.executeQuery(sql);
			if (null == result) {
				return appList;
			}
			while (result.next()) {
				ForumData obj = new ForumData();
				if (obj.Initialize(result)) {
					appList.add(obj);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (null != appRecommendConn) 		//clean
					appRecommendConn.close();
				if (null != stmt)
					stmt.close();
				if (null != result)
					result.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		return appList;
	}
	
	public static void main(String[] args) {
		int userId = 5423;
		List<ForumData> result = ForumRecommendUtil.getInstance().getForumRecommendList(userId);
		for (ForumData item : result) {
			System.out.println(item.getUserId());
			System.out.println(item.getAppId());
			System.out.println(item.getSharesCount());
			String sh = "";
			for (Integer id : item.getShareList()) {
				sh += id + " ";
			}
			System.out.println(sh);
		}
		System.exit(0);
	}
}

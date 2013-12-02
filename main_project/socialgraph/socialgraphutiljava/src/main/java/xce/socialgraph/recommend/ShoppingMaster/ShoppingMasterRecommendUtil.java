package xce.socialgraph.recommend.shoppingmaster;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.xce.XceAdapter;

/**
 * 获取逛街达人推荐数据类
 * @author yangxiaoyun
 *
 */
public class ShoppingMasterRecommendUtil {
	private static ShoppingMasterRecommendUtil instance = null;
	private static String dbSourceName = "friend_rank";
	
	private ShoppingMasterRecommendUtil() {
	}
	
	public static ShoppingMasterRecommendUtil getInstance() {
		if (null == instance) {
			synchronized (ShoppingMasterRecommendUtil.class) {
				try {
					instance = new ShoppingMasterRecommendUtil();
				} catch (Exception e) {
					e.printStackTrace();
					instance = null;
				}
			}
		}
		return instance;
	}
	
	/**
	 * 根据用户提供userId获取为用户推荐的masterid
	 * @param userId
	 * @return
	 */
	public List<ShoppingMasterData> getShoppingMasterRecommendList(int userId) {
		List<ShoppingMasterData> shoppingMasterList = new ArrayList<ShoppingMasterData>();
		String tableName = "recommend_shoppingmaster";
		Connection shoppingMasterRecommendConn = null;
		Statement stmt = null;
		ResultSet result = null;
		try {
			shoppingMasterRecommendConn = XceAdapter.getInstance().getReadConnection(dbSourceName);
			stmt = shoppingMasterRecommendConn.createStatement();
			String sql = "";
			sql = "SELECT user_id, master_id, commonfriendsid, share_count FROM " + tableName + " WHERE user_id = " + userId;
			result = stmt.executeQuery(sql);
			if (null == result) {
				return shoppingMasterList;
			}
			while (result.next()) {
				ShoppingMasterData obj = new ShoppingMasterData();
				if (obj.Initialize(result)) {
					shoppingMasterList.add(obj);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (null != shoppingMasterRecommendConn) 		//clean
					shoppingMasterRecommendConn.close();
				if (null != stmt)
					stmt.close();
				if (null != result)
					result.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		return shoppingMasterList;
	}
	
	public static void main(String[] args) {
		int userId = 2840;
		List<ShoppingMasterData> result = ShoppingMasterRecommendUtil.getInstance().getShoppingMasterRecommendList(userId);
		for (ShoppingMasterData item : result) {
			System.out.println(item.getUserId());
			System.out.println(item.getShoppingMasterId());
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

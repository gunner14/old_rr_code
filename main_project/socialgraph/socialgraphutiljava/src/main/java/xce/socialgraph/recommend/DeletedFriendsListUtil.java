package xce.socialgraph.recommend;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.xce.XceAdapter;

/**
 * Get friends list deleted by host
 * @author zhangnan
 *
 */
public class DeletedFriendsListUtil {
	private static DeletedFriendsListUtil instance = null;
	private static String dbSourceName = "socialgraph_friendrelation";
	
	private DeletedFriendsListUtil() {
	}
	
	public static DeletedFriendsListUtil getInstance() {
		if (null == instance) {
			synchronized (DeletedFriendsListUtil.class) {
				try {
					instance = new DeletedFriendsListUtil();
				} catch (Exception e) {
					e.printStackTrace();
					instance = null;
				}
			}
		}
		return instance;
	}
	
	/**
	 * Get deleted list from socialgraph_friendrelation_block
	 * @param userId
	 * @param limit
	 * @return
	 */
	public List<Integer> getDeletedFriensList(int userId, int limit) {
		List<Integer> deletedList = new ArrayList<Integer>();
		Connection conn = null;
		Statement stmt = null;
		ResultSet result = null;
		
		try {
			String pattern = getPattern("socialgraph_friendrelation_block_", userId, 100);
			conn = XceAdapter.getInstance().getReadConnection(dbSourceName, pattern);
			stmt = conn.createStatement();
			
			String sql = "";
			if (limit <= 0) {
				sql = "SELECT block_friend_id FROM " + pattern + " WHERE user_id = "
						+ userId + " LIMIT " + 10000; // max size is 10000
			} else {
				sql = "SELECT block_friend_id FROM " + pattern + " WHERE user_id = "
				+ userId + " LIMIT " + limit; 
			}
			
			result = stmt.executeQuery(sql);
			if (null == result) {
				return deletedList;
			}
			while (result.next()) {
				deletedList.add(result.getInt("block_friend_id"));
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (null != conn)		//clean
					conn.close();
				if (null != stmt)
					stmt.close();
				if (null != result)
					result.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		return deletedList;
	}
	
	/**
	 * 获取pattern
	 * @param prefix表名前缀
	 * @param id 用户id
	 * @param cluster 散的份数
	 * @return
	 */
	private String getPattern(String prefix, long id, int cluster) {
		StringBuffer buff = new StringBuffer();
		buff.append(prefix);
		buff.append(Math.abs(id) % cluster);
		return buff.toString();
	}
	
	public static void main(String[] args) {
		List<Integer> deletedList = DeletedFriendsListUtil.getInstance().getDeletedFriensList(100002372, -1);
		for (Integer id : deletedList) {
			System.out.println(id);
		}
	}
}

package checker;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.xce.XceAdapter;

public class DBHelper {

	public static List<Integer> getBuddyApplyCacheFromDB(int userId)
			throws SQLException {
		Connection conn = XceAdapter.getInstance().getReadConnection(
				"buddyapplycache_apply");
		List<Integer> res = new ArrayList<Integer>();
		try {
			Statement stmt = conn.createStatement();
			try {
				String sql = "SELECT applicant,UNIX_TIMESTAMP(time) AS unix_time"
						+ " FROM buddyapply"
						+ " WHERE accepter="
						+ userId
						+ " ORDER BY time DESC" + " LIMIT 100";
				System.out.println(sql);
				ResultSet rset = stmt.executeQuery(sql);
				try {
					while (rset.next()) {
						res.add(rset.getInt(1));
					}
				} catch (Exception e) {
					e.printStackTrace();
				} finally {
					try {
						rset.close();
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				try {
					stmt.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				conn.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return res;
	}

	public static List<Integer> getFriendsFromDB(int userId)
			throws SQLException {
		Connection conn = XceAdapter.getInstance().getReadConnection(
				XceAdapter.DB_RELATION, XceAdapter.PATTERN_RELATION(userId));
		List<Integer> res = new ArrayList<Integer>();
		try {
			Statement stmt = conn.createStatement();
			try {
				String sql = "SELECT guest FROM relation_"
						+ String.valueOf(userId % 100) + " WHERE host="
						+ String.valueOf(userId);
				System.out.println(sql);
				ResultSet rset = stmt.executeQuery(sql);
				try {
					while (rset.next()) {
						res.add(rset.getInt(1));
					}
				} catch (Exception e) {
					e.printStackTrace();
				} finally {
					try {
						rset.close();
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				try {
					stmt.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				conn.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return res;
	}

	public static int getUserCacheStatusFromDB(int userId) throws SQLException {
		Connection conn = XceAdapter.getInstance().getReadConnection(
				"user_passport_status");
		int res = -1;
		try {
			Statement stmt = conn.createStatement();
			try {
				String sql = "SELECT status" + " FROM user_passport" + " WHERE id="
						+ userId;
				System.out.println(sql);
				ResultSet rset = stmt.executeQuery(sql);
				try {
					if (rset.next()) {
						res = rset.getInt(1);
					}
				} catch (Exception e) {
					e.printStackTrace();
				} finally {
					try {
						rset.close();
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				try {
					stmt.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				conn.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return res;
	}
}

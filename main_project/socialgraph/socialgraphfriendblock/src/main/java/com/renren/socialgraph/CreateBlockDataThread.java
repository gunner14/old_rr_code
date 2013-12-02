package com.renren.socialgraph;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import com.xiaonei.xce.XceAdapter;

import xce.socialgraph.util.Clogging;

/**
 * Insert into data base
 * @author zhangnan
 *
 */
public class CreateBlockDataThread implements Runnable {
	private static String dbSourceName = "socialgraph_friendrelation";
	private BlockDataItem blockItem;
	
	public CreateBlockDataThread(BlockDataItem blockItem) {
		this.blockItem = blockItem;
	}

	public void run() {
		Connection blockFriendsConn = null;
		Statement stmt = null;
		ResultSet result = null;
		
		try {
			String pattern = getPattern("socialgraph_friendrelation_block_", this.blockItem.userId, 100);
			blockFriendsConn = XceAdapter.getInstance().getWriteConnection(dbSourceName, pattern);
			stmt = blockFriendsConn.createStatement();
			String sql = "";

			sql = "INSERT INTO " + getPattern("socialgraph_friendrelation_block_", this.blockItem.userId, 100) 
					+ " (user_id, block_friend_id, time) VALUES (" + this.blockItem.userId + " , " + this.blockItem.friendBlockId
					+ ", \"" + new java.sql.Date(new java.util.Date().getTime()) + "\") ON DUPLICATE KEY UPdATE time=VALUES(time)";
			stmt.execute(sql);
			Clogging.Debug("CreateBlockDataThread Insert " + this.blockItem.userId + " : " + this.blockItem.friendBlockId);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (null != blockFriendsConn) 		//clean
					blockFriendsConn.close();
				if (null != stmt)
					stmt.close();
				if (null != result)
					result.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
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
}

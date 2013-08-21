package com.xiaonei.platform.scheduleTask;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Date;

public class Domain {
	private ArrayList<Node> loadNodes = new ArrayList<Node>();

	private DatabaseHelper dsh;

	public Domain(DatabaseHelper dsh) throws Exception {
		this.dsh = dsh;
		try {
			loadConfig();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public ArrayList<Node> getLoadNodes() {
		if (this.loadNodes == null) {
			return new ArrayList<Node>();
		}
		return this.loadNodes;
	}

	public void loadConfig() {		
		Connection conn = dsh.getConnection();
		Statement stmt = null;
		ResultSet rs = null;
		try {
			stmt = conn.createStatement();
			rs = stmt.executeQuery("select * from "
					+ Constants.TABLE_STORAGE_NODE + " where "
					+ Node.COLUMN_STATUS + ">" + Constants.NODE_STATUS_READABLE
					+ "");

			System.out.println("select * from "
					+ Constants.TABLE_STORAGE_NODE + " where "
					+ Node.COLUMN_STATUS + ">" + Constants.NODE_STATUS_READABLE
					+ "");
			
			//假如没有可写组，报警！
			boolean hasWritableGroup = false;
			while (rs.next()) {
				try {
					hasWritableGroup = true;
					Node node = new Node();
					node.setId(rs.getInt(Node.COLUMN_NODE_ID));
					node.setName(rs.getString(Node.COLUMN_NODE_NAME));
					node.setDomain(rs.getString(Node.COLUMN_DOMAIN));
					node.setGroup(rs.getString(Node.COLUMN_GROUP_NAME)); 
					node.setPath(rs.getString(Node.COLUMN_PATH));
					node.setSpaceUsed(rs.getDouble(Node.COLUMN_SPACE_USED));
					node.setCpuLoad(rs.getDouble(Node.COLUMN_CPU_LOAD));
					node.setStatus(rs.getInt(Node.COLUMN_STATUS));
					node.setTime(new Date());
					node.setType(rs.getInt(Node.COLUMN_TYPE));
					System.out.println("Writable Node: " + node.getId());
					loadNodes.add(node);
				} catch (Exception e) {
					e.printStackTrace();
				}				
			}
			if (!hasWritableGroup)
			{
				System.out.println("!!!!!! Fatal: No any groups can be writable! !!!!!!");
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (rs != null) {
					rs.close();
				}
				if (stmt != null) {
					stmt.close();
				}
				dsh.closeConnection(conn);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	/*
	 * 更新数据库的node信息
	 */
	public void saveNodeState(Node node) {
		Connection conn = dsh.getConnection();
		Statement stmt = null;
		try {
			stmt = conn.createStatement();
			
			//将临时不可写节点置成可写
			if (node.getStatus() == Constants.NODE_STATUS_TEMP_READABLE && node.getCpuLoad() < 20 && node.getSpaceUsed() < 0.8)
			{
				System.out.println("!!!!!! Fix Temparary Readable Group To Writable !!!!!! NodeID : " + node.getId());
				stmt.execute("update " + Constants.TABLE_STORAGE_NODE + " set "
						+ Node.COLUMN_SPACE_USED + "='" + node.getSpaceUsed()
						+ "', " + Node.COLUMN_CPU_LOAD + "='" + node.getCpuLoad()
						+ "', " + Node.COLUMN_TIME + "='" + node.getTimeDesc()
						+ "', " + Node.COLUMN_STATUS + "='" + Constants.NODE_STATUS_WRITABLE_READABLE
						+ "' where id=" + node.getId());					
			}
			else if (node.getCpuLoad() >= 20 && node.getSpaceUsed() < 0.8) //临时不可写
			{
				System.out.println("!!!!!! Temparary Readable Group !!!!!! NodeID : " + node.getId());
				stmt.execute("update " + Constants.TABLE_STORAGE_NODE + " set "
						+ Node.COLUMN_SPACE_USED + "='" + node.getSpaceUsed()
						+ "', " + Node.COLUMN_CPU_LOAD + "='" + node.getCpuLoad()
						+ "', " + Node.COLUMN_TIME + "='" + node.getTimeDesc()
						+ "', " + Node.COLUMN_STATUS + "='" + Constants.NODE_STATUS_TEMP_READABLE
						+ "' where id=" + node.getId());				
			}
			else if (node.getSpaceUsed() >= 0.8) //置成只读
			{
				System.out.println("!!!!!! Only Readable Group !!!!!! NodeID : " + node.getId());
				stmt.execute("update " + Constants.TABLE_STORAGE_NODE + " set "
						+ Node.COLUMN_SPACE_USED + "='" + node.getSpaceUsed()
						+ "', " + Node.COLUMN_CPU_LOAD + "='" + node.getCpuLoad()
						+ "', " + Node.COLUMN_TIME + "='" + node.getTimeDesc()
						+ "', " + Node.COLUMN_STATUS + "='" + Constants.NODE_STATUS_READABLE
						+ "' where id=" + node.getId());				
			}
			else
			{
				System.out.println("!!!!!! Y! Writable Group !!!!!! NodeID : " + node.getId());
				stmt.execute("update " + Constants.TABLE_STORAGE_NODE + " set "
					+ Node.COLUMN_SPACE_USED + "='" + node.getSpaceUsed()
					+ "', " + Node.COLUMN_CPU_LOAD + "='" + node.getCpuLoad()
					+ "', " + Node.COLUMN_TIME + "='" + node.getTimeDesc()
					+ "' where id=" + node.getId());
			}

		} catch (Exception e) {
			System.out.println("Storage:Domain node[" + node.getName() + "] updated failure -> " + e);
		} finally {
			if (stmt != null) {
				try {
					stmt.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			dsh.closeConnection(conn);
		}
	}
}

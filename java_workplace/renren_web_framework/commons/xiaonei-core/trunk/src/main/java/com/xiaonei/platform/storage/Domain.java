package com.xiaonei.platform.storage;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;



/**
 * 如果需要跟踪存储为空的状态，需要配置logger，info级别
 * <pre>
 *  <logger name="com.xiaonei.platform.storage.Domain" additivity="false">
 *		<level value="info"/>
 *		<appender-ref ref="upload"/>
 *	</logger>
 * </pre>
 *
 */
public class Domain {

	private Log logger = LogFactory.getLog(getClass());
	
	private Map<String, Group> allGroups;

	private WritableGroupSet writableGroups;

	private Random rand = new Random();

	private DatabaseHelper dsh;

	public Domain(DatabaseHelper dsh) throws Exception {
		this.dsh = dsh;
		if (allGroups == null || writableGroups == null) {
			this.loadConfig();
		}
	}

	public synchronized Group getGroup(String groupName) {
		//return allGroups.get(groupName);
		Group group = allGroups.get(groupName);
		if (logger.isInfoEnabled()) {
			// 跟踪查询Group找不到的情况，需要配置Logger
			if (group == null) {
				logger.info("group "+groupName+" not found.");
				dumpGroup();
			}
		}
		return group;
	}
	
	private void dumpGroup() {
		if (allGroups == null) {
			if (logger.isInfoEnabled()) {
				logger.info("allGroups == null");
			}
		} else {
			if (logger.isInfoEnabled()) {
				logger.info("groups size: " + allGroups.size());
				if (allGroups.size() > 0) {
					logger.info("nodes: " + StringUtils.join(allGroups.keySet().toArray(), ","));
				}
			}
		}
	}

	public void loadConfig() {
		
		long startTime = System.currentTimeMillis();
		
		Map<String, Group> tempAllGroups = new HashMap<String, Group>();
		Connection conn = dsh.getConnection();
		Statement stmt = null;
		ResultSet rs = null;
		try {
			stmt = conn.createStatement();
			rs = stmt.executeQuery("select * from "
					+ Constants.TABLE_STORAGE_NODE + " where "
					+ Node.COLUMN_STATUS + ">" + Constants.NODE_STATUS_DISABLE
					+ "");
			
			if (logger.isInfoEnabled()) {
				logger.info("select * from " + Constants.TABLE_STORAGE_NODE
					+ " where " + Node.COLUMN_STATUS + ">"
					+ Constants.NODE_STATUS_DISABLE + "");
			}
			
			while (rs.next()) {
				try {
					Node node = new Node();
					node.setId(rs.getInt(Node.COLUMN_NODE_ID));
					node.setName(rs.getString(Node.COLUMN_NODE_NAME));
				
					node.setDomain(rs.getString(Node.COLUMN_DOMAIN));
					node.setGroup(rs.getString(Node.COLUMN_GROUP_NAME));
					node.setPath(rs.getString(Node.COLUMN_PATH));
					node.setSpaceUsed(rs.getDouble(Node.COLUMN_SPACE_USED));
					node.setCpuLoad(rs.getDouble(Node.COLUMN_CPU_LOAD));
					node.setStatus(rs.getInt(Node.COLUMN_STATUS));

					//System.out.println(node.getName() + " node name & status ==================="								+ node);
					node.setTime(new Date());
					node.setType(rs.getInt(Node.COLUMN_TYPE));
					Group group = tempAllGroups.get(node.getGroup());
					if (group == null) {
						group = new Group();
						group.setName(node.getGroup());
						tempAllGroups.put(group.getName(), group);
					}
					group.putNode(node);
				} catch (Exception e) {
				}
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
		synchronized (this) {
			if(tempAllGroups.size()!=0)
				this.allGroups = tempAllGroups;
			if (logger.isInfoEnabled()) {
				logger.info("all available nodes===>" + tempAllGroups.size());
			}
		}
		loadWriteGroups();
		
		long endTime = System.currentTimeMillis();
		long cost = endTime - startTime;
		
		logger.warn("load Domain config done, costs " + cost + "ms");
	}

	public void loadWriteGroups() {
		Map<String, Group> tempAllGroups = new HashMap<String, Group>();
		ArrayList<Group> tempWritableGroups = new ArrayList<Group>();
		Connection conn = dsh.getConnection();
		Statement stmt = null;
		ResultSet rs = null;
		try {
			stmt = conn.createStatement();
			rs = stmt.executeQuery("select * from "
					+ Constants.TABLE_STORAGE_NODE + " where "
                    + Node.COLUMN_STATUS + "="
					+ Constants.NODE_STATUS_WRITABLE_READABLE
					+ "");

			System.out.println("select * from " + Constants.TABLE_STORAGE_NODE
					+ " where "+ Node.COLUMN_STATUS + "="
					+ Constants.NODE_STATUS_WRITABLE_READABLE
					+ "");

			while (rs.next()) {
				try {
					Node node = new Node();
					node.setId(rs.getInt(Node.COLUMN_NODE_ID));
					node.setName(rs.getString(Node.COLUMN_NODE_NAME));
					if (logger.isInfoEnabled()) {
						logger.info("node name node name node name ====================="
									+ node.getName());
					}
					node.setDomain(rs.getString(Node.COLUMN_DOMAIN));
					node.setGroup(rs.getString(Node.COLUMN_GROUP_NAME));
					node.setPath(rs.getString(Node.COLUMN_PATH));
					node.setSpaceUsed(rs.getDouble(Node.COLUMN_SPACE_USED));
					node.setCpuLoad(rs.getDouble(Node.COLUMN_CPU_LOAD));
					node.setStatus(rs.getInt(Node.COLUMN_STATUS));

					node.setTime(new Date());
					node.setType(rs.getInt(Node.COLUMN_TYPE));
					Group group = tempAllGroups.get(node.getGroup());
					if (group == null) {
						group = new Group();
						group.setName(node.getGroup());
						tempAllGroups.put(group.getName(), group);
					}
					group.putNode(node);
					tempWritableGroups.add(group);
				} catch (Exception e) {
				}
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
		synchronized (this) {
			writableGroups = new WritableGroupSet(tempWritableGroups);
			
			if (logger.isInfoEnabled()) {
				logger.info("this.writableGroups size size size size ==========="
							+ writableGroups.size());
			}
		}
	}

	public synchronized void rankWritableGroups() {
		writableGroups.rank(allGroups.values().toArray());
	}

	public synchronized Group getWritableGroup(int type) {
		int rankLevel = Constants.RANK_LEVEL_IDLE;
		int r = rand.nextInt(10);
		if (r < 8) {
			rankLevel = Constants.RANK_LEVEL_IDLE;
		} else {
			rankLevel = Constants.RANK_LEVEL_NORMAL;
		}
		
		//System.out.println("writableGroups size========================="
		//		+ writableGroups.size());

		return writableGroups.getGroup(rankLevel, type);
	}

	public synchronized Object[] getAllGroups() {
		return allGroups.values().toArray();
	}
	public synchronized Map<String,Group > getAllGroup(){
		return allGroups ;
	}
}

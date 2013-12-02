/*
 * Function: SocialGraph group SocialGraphFoF friends recommendation service data items
 * accessing interfaces, including hostId, fofId, rank, mutualFriendNumber, mutualFriendIds 
 * Developer: xiongjunwu
 * Email: Junwu.xiong@renren-inc.com
 * Date: 2012-03-26
 */
package xce.socialgraph.recommend.fof;

import java.sql.Connection;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.io.InputStreamReader;

import xce.util.tools.IntSeqSerialize;

import com.xiaonei.xce.XceAdapter;


public class RcdFoFUtil {
	private static RcdFoFUtil instance = null;
	private final static String kDbInstanceName = "socialgraph_recommend_fof";
	private final static String kTableNamePrefix = "recommend_fof";
    private final static int kEmptyId = -1;
	
	private RcdFoFUtil() {		
	}
    
	public static RcdFoFUtil getInstance() {
		if (null == instance) {
			synchronized (RcdFoFUtil.class) {
				if (null == instance) { 
					try {
						instance = new RcdFoFUtil();				  	
					} catch (Exception e) {
						e.printStackTrace();
						instance = null;
					}				
				}
			}
		}
		return instance;
	}
	
	private String TableName(String TablePrefix, int hostId) {
		return TablePrefix + "_" + (hostId % 100);		
	}
	
	public List<RcdFoFData> getRcdFoFData(int hostId, int limit) {
		List<RcdFoFData> rcdFoFList = new ArrayList<RcdFoFData>();
		Connection rcdFoFConnection = null;
		Statement rcdFoFStatement = null;
		ResultSet rcdFoFResultSet = null;
	    try {
	    	final String kTableName = TableName(kTableNamePrefix, hostId);
	    	rcdFoFConnection = XceAdapter.getInstance().getReadConnection(kDbInstanceName, kTableName);
	    	rcdFoFStatement = rcdFoFConnection.createStatement();
	    	String sql = "";
	    	if (limit <= 0) {
	    		sql = "SELECT host_id, fof_id, rank, mutual_friend_number, mutual_friend_ids" +
	    				" FROM " + kTableName + " WHERE host_id = " + hostId + " LIMIT " + 100;	    				
	    	} else {
	    		sql = "SELECT host_id, fof_id, rank, mutual_friend_number, mutual_friend_ids" +
	    				" FROM " + kTableName + " WHERE host_id = " + hostId + " LIMIT " + limit;	 
	    	}
	    	rcdFoFResultSet = rcdFoFStatement.executeQuery(sql);
	    	if (null == rcdFoFResultSet) {
	    		return rcdFoFList;
	    	}
	    	
	    	while (rcdFoFResultSet.next()) {
	    		RcdFoFData record = new RcdFoFData();
	    		if (kEmptyId != rcdFoFResultSet.getInt("fof_id")) {
		    		record.setHostId(rcdFoFResultSet.getInt("host_id"));
		    		record.setFoFId(rcdFoFResultSet.getInt("fof_id"));
		    		record.setRank(rcdFoFResultSet.getFloat("rank"));
		    		record.setMutualFriendNumber(rcdFoFResultSet.getInt("mutual_friend_number"));
		    		record.setMutualFriendIds(IntSeqSerialize.unserialize(rcdFoFResultSet.getBytes("mutual_friend_ids")));
			    	rcdFoFList.add(record);
		    	} else {
		    		break;
		    	}
	    	}	    
	    	
	    } catch (SQLException e) {
	    	e.printStackTrace();
	    } finally {
	    	try {
		    	if (null != rcdFoFConnection) {
		    		rcdFoFConnection.close();
		    	}
		    	if (null != rcdFoFStatement) {
		    		rcdFoFStatement.close();
		    	}
		    	if (null != rcdFoFResultSet) {
		    		rcdFoFResultSet.close();
		    	}
	    	} catch (SQLException e) {
	    		e.printStackTrace();
	    	}
	    }
		
		return rcdFoFList;
	}
	
	public static void main(String[] args) {
		int hostId = 256404211;
		int limit = 0;
		try {
//		Scanner scan = new Scanner(System.in);
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in)); 
		System.out.println("Input the hostId: ");
		String in1 = br.readLine();
//		int hostId = scan.next();
		hostId = Integer.parseInt(in1);
		System.out.println("Input the limit: ");
		String in2 = br.readLine();
//		int limit = scan.next();
		limit = Integer.parseInt(in2);
		} catch (IOException e) {
			e.printStackTrace();
		}

		List<RcdFoFData> result = RcdFoFUtil.getInstance().getRcdFoFData(hostId, limit);
		for (RcdFoFData record : result) {
			System.out.println(record.getHostId() + " : " + record.getFoFId() + " : " 
					+ record.getRank() + " : " + record.getMutualFriendNumber() + " : " 
					);
			String ids = "";
			for (Integer id : record.getMutualFriendIds()) {
				ids += id + " ";
			}
			System.out.println(ids);
		}
		System.out.println("result size: " + result.size());
		System.exit(0);
	}
}

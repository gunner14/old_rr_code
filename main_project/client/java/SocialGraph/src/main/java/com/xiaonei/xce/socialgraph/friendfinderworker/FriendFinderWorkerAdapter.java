package com.xiaonei.xce.socialgraph.friendfinderworker;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Iterator;

import xce.socialgraph.ActionType;
import xce.socialgraph.Info;
import xce.socialgraph.Message;

import com.google.protobuf.InvalidProtocolBufferException;
import com.xiaonei.xce.XceAdapter;
import com.xiaonei.xce.socialgraph.SocialGraphBase;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;
import com.xiaonei.xce.buddybyidcache.BuddyByIdCacheAdapter;
/**
 * 二度好友adapter
 * @author zhangnan
 *
 */
public class FriendFinderWorkerAdapter extends SocialGraphBase {
	private static FriendFinderWorkerAdapter instance_ = null;
	private static String type_ = "FFW";
//add by yang
	public int binSearch(List<Integer> sortedList,Integer seachValue)
	{
		 // 左边界
        int leftBound=0;
        // 右边界
        int rightBound=sortedList.size()-1;
        // 当前下标位置
        int curr;
        
        while(true){
            // 定位在左边界和右边界中间
            curr=(leftBound+rightBound)/2;
            if(sortedList.get(curr).equals(seachValue)){
                // 找到值
                return curr;
            }
            else if(leftBound>rightBound){
                // 左边界大于右边界，已经找不到值
                return -1;
            }
            else{
                if(sortedList.get(curr).compareTo(seachValue)<0){
                    // 当当前下标对应的值小于查找的值时，缩短左边界
                    leftBound=curr+1;
                }
                else{
                    // 当当前下标对应的值大于查找的值时，缩短右边界
                    rightBound=curr-1;
                }
            }
          }
	}	
	private FriendFinderWorkerAdapter() {
		// TODO Auto-generated constructor stub
		super();
	}
	
	public static FriendFinderWorkerAdapter getInstance() {
		if (null == instance_) {
			synchronized (FriendFinderWorkerAdapter.class) {
				if (null == instance_) {
					try {
						instance_ = new FriendFinderWorkerAdapter();
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
						instance_ = null;
					}
				}
			}
		}
		return instance_;
	}
	
	/**
	 * 获取用户二度好友数据
	 * @param userId 用户id
	 * @param limit 返回数据的limit限制，limit<0时为取全部数据
	 * @return
	 */
	public List<Common> getCommonFriends(int userId, int limit) {
		List<Common> result = new ArrayList<Common>();
		byte[] data = getData(type_, userId);
		System.out.println("FriendFinderWorkerAdapter data size:" + data.length);
		if (null != data) {
			try {
				int count = 0;
				Items commResult = Items.parseFrom(data);
				if (commResult.getItemsCount() < limit || limit < 0) {
					count = commResult.getItemsCount();
				} else {
					count = limit;
				}
				
				for (Item comm : commResult.getItemsList()) {
					if (--count < 0) {
						break;
					}
					result.add(new Common(comm));
				}
			} catch (InvalidProtocolBufferException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return result;
	}
	
	/**
	 * 从数据库中获取二度好友数据
	 * @param host 用户id
	 * @param limit 返回的个数，-1时取全部100个共同好友数据
	 * @return
	 */
	public List<CommonFriend> getCommonFriendsFromDB(int host, int limit) {
		List<CommonFriend> commList = new ArrayList<CommonFriend>();
		List<CommonFriend> listNew = new ArrayList<CommonFriend>();
		final String dbSourceName = "friendfinder_common_friends_new";
		Connection commonFriendsConn = null;
		Statement stmt = null;
		ResultSet result = null;
		try {
			String pattern = getPattern("common_friends_", host, 100);
			commonFriendsConn = XceAdapter.getInstance().getReadConnection(dbSourceName, pattern);
			stmt = commonFriendsConn.createStatement();
			String sql = "";
			if (limit <= 0) {
				sql = "SELECT friend_id, weight, shares FROM "
						+ getCommonFriendsTableName(host) + " WHERE owner_id = " + host
						+ " ORDER BY position DESC" + " LIMIT " + 100;
			} else {
				sql = "SELECT friend_id, weight, shares FROM "
					+ getCommonFriendsTableName(host) + " WHERE owner_id = " + host
					+ " ORDER BY position DESC" + " LIMIT " + limit;
			}
			result = stmt.executeQuery(sql);
			if (null == result) {
				return commList;
			}
			while (result.next()) {
				CommonFriend obj = new CommonFriend();
				if (obj.Initialize(result)) {
					commList.add(obj);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (null != commonFriendsConn) 		//clean
					commonFriendsConn.close();
				if (null != stmt)
					stmt.close();
				if (null != result)
					result.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		try{
			List<Integer> userFriend = BuddyByIdCacheAdapter.getInstance().getFriendListAsc(host, -1);
			for(CommonFriend cf : commList) {
				int index = binSearch(userFriend, cf.getUserId());
//				System.out.println(cf.getUserId());
//				System.out.println(index);
				
				if(-1 == index) {
					listNew.add(cf);
				}
			}
		}catch (Exception e) {
                        e.printStackTrace();
		}
//		return commList;
		return listNew;
	}
	
	/**
	 * 删除某个二度好友接口
	 * @param userId 用户id
	 * @param guestId 删除的推荐用户的id
	 */
	public void removeCommonFriends(int userId, int guestId) {
		Info information = new Info();
		information.hostId = userId;
		information.type = type_;
		information.operation = ActionType.DELETE;
		information.friendIds = new int[1];
		information.friendIds[0] = guestId;
		
		Message message = new Message();
		message.content = new HashMap<String, Info>();
		message.content.put(type_, information);
		
		report(userId, message);
	}
	
	/**
	 * 根据散表获取相应的表名
	 * @param host
	 * @return
	 */
	private String getCommonFriendsTableName(int host) {
		return "common_friends_" + (host %100);
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
		int userId = 301179245;
		List<CommonFriend> commList = FriendFinderWorkerAdapter.getInstance().getCommonFriendsFromDB(userId, -1);
		for (CommonFriend comm : commList) {
			System.out.println(comm.getUserId() + " : " + comm.getsharesCount());
			for (Integer e : comm.getShares()) {
				System.out.print(e + " ");
			}
			System.out.println();
			System.out.println();
		}
//		System.out.println("ok..........................");
//		List<Integer> userFriend = BuddyByIdCacheAdapter.getInstance().getFriendListAsc(301179245, -1);
//		System.out.println(userFriend);
//		List<Common> result = FriendFinderWorkerAdapter.getInstance().getCommonFriends(userId, -1);
//		for (Common r : result) {
//			System.out.println(r.getUserId() + " : " + r.getShareCount());
//			for (Integer e : r.getShares()) {
//				System.out.print(e + " ");
//			}
//			System.out.println();
//			System.out.println();
//		}
		
//		FriendFinderWorkerAdapter.getInstance().removeCommonFriends(userId, 221314588);
		System.exit(0);
	}
}

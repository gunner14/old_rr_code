package xce.socialgraph.recommend.page;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import xce.util.tools.IntSeqSerialize;

import com.xiaonei.xce.XceAdapter;

/**
 * 获取page推荐数据类
 * @author zhangnan
 * @email zhangnan@renren-inc.com
 * Change: Optimizie user's page recommendation data storage: delete page's name and description items
             from original rcd DB table and fetch from page DB directly, XiongJunwu, 2012-03-05. 
 */
public class PageRecommendUtil {
	private static PageRecommendUtil instance = null;
	private static String rcdPageDB = "socialgraph_recommend_page_new";
	private static String pageDB = "page";
	
	private PageRecommendUtil() {
	}

	public static PageRecommendUtil getInstance() {
		if (null == instance) {
			synchronized (PageRecommendUtil.class) {
				if (null == instance) {
					try {
						instance = new PageRecommendUtil();
					} catch (Exception e) {
						e.printStackTrace();
						instance = null;
					}
				}
			}
		}
		return instance;
	}
	
	//Sort recommended pages by page_value
	class SortByPageValue implements Comparator {
		public int compare(Object o1, Object o2) {
			// TODO Auto-generated method stub
			PageRecommendData UserPage1 = (PageRecommendData)o1;
			PageRecommendData UserPage2 = (PageRecommendData)o2;
			return (UserPage1.getPageValue() < UserPage2.getPageValue() ? 1 : 0);
		}
	}
	
	/**
	 * 获取推荐page列表的关系属性数据
	 * @param hostId 用户id
	 * @param limit 数据个数 <=0时，取100个，其他取真实limit
	 * @return
	 */
	public List<PageRecommendData> getPageRecommend(int hostId, int limit) {
		List<PageRecommendData> pageRecommendList = new ArrayList<PageRecommendData>();
		Connection rcdPageConn = null;
		Statement stmt = null;
		ResultSet result = null;
		
		Connection pageInfoConn = null;
		Statement pageInfoState = null;
		ResultSet pageInfoResult = null;

		try {
			rcdPageConn = XceAdapter.getInstance().getReadConnection(rcdPageDB);
			stmt = rcdPageConn.createStatement();
			String sql = "";
			
			pageInfoConn = XceAdapter.getInstance().getReadConnection(pageDB);
			pageInfoState = pageInfoConn.createStatement();
			String pageInfoSql = "";
			if (limit <= 0) {
//				sql = "SELECT * FROM recommend_by_page WHERE user_id = " + hostId + " LIMIT " + 100;
				sql = "SELECT * FROM recommend_by_page_new WHERE user_id = " + hostId + " LIMIT " + 100;

			} else {
//				sql = "SELECT * FROM recommend_by_page WHERE user_id = " + hostId + " LIMIT " + limit;
				sql = "SELECT * FROM recommend_by_page_new WHERE user_id = " + hostId + " LIMIT " + limit;

			}
			result = stmt.executeQuery(sql);
			if (null == result) {
				return pageRecommendList;
			}
			
			while (result.next()) {
				PageRecommendData pageData = new PageRecommendData();
				
				pageData.setUserId(result.getInt("user_id"));
				int pageId = result.getInt("page_id");
				pageData.setPageId(pageId);
				pageData.setPageValue(result.getDouble("page_value"));				
//				pageData.setPageName(result.getString("page_name"));
//				pageData.setDescription(result.getString("description"));
				try {
					pageInfoSql = "SELECT name, description FROM page WHERE id = " + pageId;
					pageInfoResult = pageInfoState.executeQuery(pageInfoSql);					
					if(null == pageInfoResult) {
						System.out.println("query no"); 
						pageData.setPageName("");
						pageData.setDescription("");
					} else {
						pageInfoResult.next();
					    pageData.setPageName(pageInfoResult.getString("name"));
					    pageData.setDescription(pageInfoResult.getString("description"));
					    if (null == pageInfoResult.getString("description")) {
					        pageData.setDescription("");
					    }
					}
				} catch (SQLException e) {
					e.printStackTrace();
				}
				
				pageData.setFansNum(result.getInt("fans_counter"));
				pageData.setCommonSize(result.getInt("common_friend_number"));
				int commSize = result.getInt("common_friend_number");
				
				List<Integer> tmp = IntSeqSerialize.unserialize(result.getBytes("common_friend_ids"));
				int realSize = commSize > tmp.size() ? tmp.size() : commSize;		//判断共同好友的截断位置
				pageData.setCommonFriends(tmp.subList(0, realSize));
				
				pageRecommendList.add(pageData);
			}
			
			Collections.sort(pageRecommendList, new SortByPageValue());
			
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			try {
				if (null != rcdPageConn)			//clean
					rcdPageConn.close();			
				if (null != stmt)
					stmt.close();
				if (null != result)
					result.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
			
			try {
				if (null != pageInfoConn) {
					pageInfoConn.close();
				}
				if (null != pageInfoState) {
					pageInfoState.close();
				}
				if (null != pageInfoResult) {
					pageInfoResult.close();
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		
		return pageRecommendList;
	}
	
	public static void main(String[] args) {
		//int userId = 256404211;
		int userId = 238111132;
		List<PageRecommendData> result = PageRecommendUtil.getInstance().getPageRecommend(userId, -1);
		for (PageRecommendData data : result) {
//			System.out.println(data.getPageId() + " : " + data.getPageValue() + " : " + data.getPageName());
			System.out.println(data.getUserId() + " : " + data.getPageId() + " : "
					+ data.getPageName() + " : " + data.getPageValue() + " : "
					+ data.getFansNum() + " : " + data.getDescription() + " : "
					+ data.getCommonSize());
//			System.out.println(data.getUserId() + " : " + data.getPageId() + " : "
//					+ data.getPageValue() + " : " + data.getFansNum() + " : "
//					+ data.getCommonSize());
			String detail = "";
			for (Integer ii : data.getCommonFriends()) {
				detail += ii + " ";
			}
			System.out.println(detail);
		}
		System.out.println("result size:" + result.size());
		System.exit(0);
	}
}

package xce.socialgraph.recommend.forum;

import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;

import xce.util.tools.IntSeqSerialize;

/**
 * 推荐Forum具体数据类
 * @author zhangnan
 *
 */
public class ForumData {
	private Integer userId = 0;		//用户id
	private Integer appId = 0;		//应用id
	private Integer sharesCount = 0;	//共同好友数
	private List<Integer> shares = null;	//共同好友 
	
	public ForumData() {
		shares = new ArrayList<Integer>();
	}
	
	/**
	 * 使用数据库中的数据初始化结构体
	 * @param item
	 * @return
	 */
	public boolean Initialize(ResultSet item) {
		try {
			this.userId = item.getInt("userid");
			this.appId = item.getInt("appid");
			this.sharesCount = item.getInt("sharescount");
			this.shares = IntSeqSerialize.unserialize(item.getBytes("shares"));
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return false;
		}

		return true;
	}
	
	/**
	 * 返回用户id
	 * @return
	 */
	public Integer getUserId() {
		return this.userId;
	}
	
	/**
	 * 返回应用id
	 * @return
	 */
	public Integer getAppId() {
		return this.appId;
	}
	
	/**
	 * 返回共同好友数
	 * @return
	 */
	public Integer getSharesCount() {
		return this.sharesCount;
	}
	
	/**
	 * 返回共同好友
	 * @return
	 */
	public List<Integer> getShareList() {
		return this.shares; 
	}
}

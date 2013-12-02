package xce.socialgraph.recommend.shoppingmaster;

import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;

import xce.util.tools.IntSeqSerialize;

/**
 * 推荐ShoppingMster具体数据类
 * @author yangxiaoyun
 *
 */
public class ShoppingMasterData {
	private Integer userId = 0;		//用户id
	private Integer masterId = 0;		//达人id
	private Integer sharesCount = 0;	//共同好友数
	private List<Integer> shares = null;	//共同好友 
	
	public ShoppingMasterData() {
		shares = new ArrayList<Integer>();
	}
	
	/**
	 * 使用数据库中的数据初始化结构体
	 * @param item
	 * @return
	 */
	public boolean Initialize(ResultSet item) {
		try {
			this.userId = item.getInt("user_id");
			this.masterId = item.getInt("master_id");
			this.sharesCount = item.getInt("share_count");
			this.shares = IntSeqSerialize.unserialize(item.getBytes("commonfriendsid"));
	//		this.sharesCount = this.shares.size();
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
	public Integer getShoppingMasterId() {
		return this.masterId;
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


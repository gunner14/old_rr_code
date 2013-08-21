package com.xiaonei.xce.socialgraph.puk;

import java.util.ArrayList;
import java.util.List;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.RcdMessageServicePrx;
import xce.socialgraph.RcdMessageServicePrxHelper;
import xce.util.channel.Channel;

import com.google.protobuf.InvalidProtocolBufferException;
import com.xiaonei.xce.socialgraph.BusinessCacheAdapter;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;

/**
 * Get Puk data from PUK business cache
 * @author zhangnan
 *
 */
public class PukRecommendAdapter extends ReplicatedClusterAdapter {
	private static PukRecommendAdapter instance = null;
	private static final String biz = "PUK";
	private static final String ctrEndpoints = "ControllerSocialGraphRcdMessageService";
	private static int interval = 120;	
	
	private PukRecommendAdapter() {
		super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
	}
	
	public static PukRecommendAdapter getInstance() {
		if (null == instance) {
			synchronized (PukRecommendAdapter.class) {
				if (null == instance) {
					try {
						instance = new PukRecommendAdapter();
					} catch (Exception e) {
						e.printStackTrace();
						instance = null;
					}
				}
			}
		}
		return instance;
	}
	
//	/**
//	 * Get hot puk friends data which sorted by user footprint
//	 * @param userId
//	 * @param limit
//	 * @return
//	 */
//	public List<Common> getHotPukFriends(int userId, int limit) {
//		List<Common> result = new ArrayList<Common>();
//		byte[] data = BusinessCacheAdapter.getInstance().get(biz, userId);
//		System.out.println(data.length);
//		if (null != data) {
//			try {
//				int count = 0;
//				Items commResult = Items.parseFrom(data);
//				if (commResult.getItemsCount() < limit || limit < 0) {
//					count = commResult.getItemsCount();
//				} else {
//					count = limit;
//				}
//				
//				for (Item comm : commResult.getItemsList()) {
//					if (--count < 0) {
//						break;
//					}
//					result.add(new Common(comm));
//				}
//			} catch (InvalidProtocolBufferException e) {
//				e.printStackTrace();
//			}
//		}
//		return result;
//	}
	
//	/**
//	 * Get PukFriends Data which shuffle result got from PUK business cache
//	 * @param userId	
//	 * @param limit
//	 * @return
//	 */
//	public List<PukFriend> getPukFriends(int userId, int limit) {
//		List<PukFriend> result = new ArrayList<PukFriend>();
//		byte[] data = BusinessCacheAdapter.getInstance().get(biz, userId);
//		if (null != data) {
//			try {
//				int count = 0;
//				Items commResult = Items.parseFrom(data);
//				if (commResult.getItemsCount() < limit || limit < 0) {
//					count = commResult.getItemsCount();
//				} else {
//					count = limit;
//				}
//				
//				for (Item comm : commResult.getItemsList()) {
//					if (--count < 0) {
//						break;
//					}
//					result.add(new PukFriend(comm));
//				}
//			} catch (InvalidProtocolBufferException e) {
//				e.printStackTrace();
//			}
//		}
//		Collections.shuffle(result);			//可能认识人的模块
//		return result;
//	}
	
	/**
	 * Get PukFriends Data
	 * @param userId
	 * @param begin
	 * @param limit
	 * @return
	 */
	public List<PukFriend> getPukFriends(int userId, int begin, int limit) {
		List<PukFriend> result = new ArrayList<PukFriend>();
		byte[] data = BusinessCacheAdapter.getInstance().get(biz, userId);
		if (null != data) {
			try {
				Items commResult = Items.parseFrom(data);

				for (Item comm : commResult.getItemsList()) {
					result.add(new PukFriend(comm));
				}
			} catch (InvalidProtocolBufferException e) {
				e.printStackTrace();
			}
		}
		
		int start = 0;
		int end = 0;
		
		if (begin <= 0) {		//begin<0, start=0; begin>0, start=begin; begin>length, start=length 
			start = 0;
		} else if (begin > 0 && begin < result.size()) {
			start = begin;
		} else {
			start = result.size();
		}
		
		if (limit <= 0) {		// limit<0, get total items of list; start+limit>max_size, get total; start_limit<max_size, end=start+limit
			end = result.size();
		} else if (start + limit > result.size()) {
			end = result.size();
		} else {
			end = start + limit;
		}
		
		return result.subList(start, end);
	}
	
	/**
	 * Remove interface
	 * @param source
	 * @param block
	 */
	public void removeBizCache(long source, long block) {
		getRcdMessageServicePrxOneWay(source).removeBizCache(source, block, biz);  //删除对应的业务cache数据
		getRcdMessageServicePrxOneWay(source).addSgBlock(source, block, biz);	//加入socialgraph block表
	}
	
	/**
	 * 获取twoway代理
	 * @param source
	 * @return
	 */
	private RcdMessageServicePrx getRcdMessageServicePrxTwoWay(long source) {
		Ice.ObjectPrx prx0 = getValidProxy((int)source, 300);
		if (null != prx0) {
			RcdMessageServicePrx prx = RcdMessageServicePrxHelper.uncheckedCast(prx0
					.ice_timeout(300).ice_twoway());
			return prx;
		}
		return null;
	}
	
	/**
	 * 获取oneway代理
	 * @param source
	 * @return
	 */
	private RcdMessageServicePrx getRcdMessageServicePrxOneWay(long source) {
		Ice.ObjectPrx prx0 = getValidProxy((int)source, 300);
		if (null != prx0) {
			RcdMessageServicePrx prx = RcdMessageServicePrxHelper.uncheckedCast(prx0
					.ice_timeout(300).ice_oneway());
			return prx;
		}
		return null;
	}
	
	public static void main(String[] args) {
		int userId = 238111132;
		List<PukFriend> result = PukRecommendAdapter.getInstance().getPukFriends(userId, 11, 10);
		System.out.println("result size:" + result.size());
		
		for (PukFriend r : result) {
			System.out.println(r.getUserId() + " : " + r.getShareCount() + " : " + r.getType());
			for (Integer e : r.getShares()) {
				System.out.print(e + " ");
			}
			System.out.println(r.getSchoolName());
			System.out.println();
			System.out.println();
		}

////		PukRecommendAdapter.getInstance().removeBizCache(userId, blockId);
		System.exit(0);
	}
}
